# Azure Deployment Guide

This document describes all the Azure resources, GitHub Secrets, and one-time setup steps required to run the CD pipeline defined in [`.github/workflows/cd-azure.yml`](../.github/workflows/cd-azure.yml).

The pipeline deploys:
| Component | Azure service | Internal port |
|-----------|---------------|---------------|
| **Backend** (ASP.NET Core 8) | Azure Web App for Containers | 8080 |
| **Frontend** (Next.js) | Azure Web App for Containers | 3000 |
| **Database** | Azure Database for PostgreSQL – Flexible Server | 5432 |

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Required Azure Resources](#required-azure-resources)
3. [One-Time Setup](#one-time-setup)
   - [1. Resource Group](#1-resource-group)
   - [2. Azure Container Registry (ACR)](#2-azure-container-registry-acr)
   - [3. PostgreSQL Flexible Server](#3-postgresql-flexible-server)
   - [4. App Service Plan](#4-app-service-plan)
   - [5. Backend Web App for Containers](#5-backend-web-app-for-containers)
   - [6. Frontend Web App for Containers](#6-frontend-web-app-for-containers)
   - [7. OIDC / Federated Identity for GitHub Actions](#7-oidc--federated-identity-for-github-actions)
4. [GitHub Secrets](#github-secrets)
5. [How the Pipeline Works](#how-the-pipeline-works)
6. [Troubleshooting](#troubleshooting)

---

## Prerequisites

- [Azure CLI](https://learn.microsoft.com/en-us/cli/azure/install-azure-cli) installed locally
- An Azure subscription
- The GitHub repository you are deploying from (needed for OIDC subject claim)
- Docker installed locally (for local testing)

---

## Required Azure Resources

| Resource | Purpose |
|----------|---------|
| Resource Group | Logical container for all resources |
| Azure Container Registry (ACR) | Stores backend and frontend Docker images |
| Azure Database for PostgreSQL – Flexible Server | Managed PostgreSQL database |
| App Service Plan (Linux, at least B1) | Compute for both Web Apps |
| Web App for Containers – Backend | Runs the ASP.NET Core API container |
| Web App for Containers – Frontend | Runs the Next.js container |
| Azure Entra ID App Registration | Enables OIDC login from GitHub Actions |

---

## One-Time Setup

The following commands use the Azure CLI. Adjust the variable values (names, regions, SKUs) to match your project.

```bash
# ── Variables ────────────────────────────────────────────────────────────────
RESOURCE_GROUP="taskflow-rg"
LOCATION="eastus"
ACR_NAME="taskflowacr"                     # must be globally unique, alphanumeric
PLAN_NAME="taskflow-plan"
BACKEND_APP="taskflow-backend-app"         # must be globally unique
FRONTEND_APP="taskflow-frontend-app"       # must be globally unique
PG_SERVER="taskflow-pg"                    # must be globally unique
PG_ADMIN_USER="pgadmin"
PG_ADMIN_PASSWORD="<strong-password>"     # change this
PG_DATABASE="taskflow"
GITHUB_ORG="mukund58"
GITHUB_REPO="taskflow-dotnet"
```

### 1. Resource Group

```bash
az group create --name "$RESOURCE_GROUP" --location "$LOCATION"
```

### 2. Azure Container Registry (ACR)

```bash
az acr create \
  --resource-group "$RESOURCE_GROUP" \
  --name "$ACR_NAME" \
  --sku Basic \
  --admin-enabled false

# Retrieve the login server (used as ACR_REGISTRY secret)
az acr show --name "$ACR_NAME" --query loginServer --output tsv
# → taskflowacr.azurecr.io
```

### 3. PostgreSQL Flexible Server

```bash
az postgres flexible-server create \
  --resource-group "$RESOURCE_GROUP" \
  --name "$PG_SERVER" \
  --location "$LOCATION" \
  --admin-user "$PG_ADMIN_USER" \
  --admin-password "$PG_ADMIN_PASSWORD" \
  --sku-name Standard_B1ms \
  --tier Burstable \
  --storage-size 32 \
  --version 15 \
  --public-access None

# Create the application database
az postgres flexible-server db create \
  --resource-group "$RESOURCE_GROUP" \
  --server-name "$PG_SERVER" \
  --database-name "$PG_DATABASE"

# Allow the backend Web App's outbound IPs (add after creating the Web App)
# az postgres flexible-server firewall-rule create ...
```

Build the connection string for the `CONNECTION_STRING` GitHub Secret:

```
Host=<PG_SERVER>.postgres.database.azure.com;Database=taskflow;Username=pgadmin;Password=<strong-password>;SslMode=Require
```

### 4. App Service Plan

```bash
az appservice plan create \
  --resource-group "$RESOURCE_GROUP" \
  --name "$PLAN_NAME" \
  --is-linux \
  --sku B2
```

### 5. Backend Web App for Containers

```bash
az webapp create \
  --resource-group "$RESOURCE_GROUP" \
  --plan "$PLAN_NAME" \
  --name "$BACKEND_APP" \
  --deployment-container-image-name "mcr.microsoft.com/appsvc/staticsite:latest"

# Tell App Service which port the container listens on
az webapp config appsettings set \
  --resource-group "$RESOURCE_GROUP" \
  --name "$BACKEND_APP" \
  --settings WEBSITES_PORT=8080

# Grant the Web App pull access to ACR (system-assigned identity)
az webapp identity assign \
  --resource-group "$RESOURCE_GROUP" \
  --name "$BACKEND_APP"

BACKEND_PRINCIPAL=$(az webapp identity show \
  --resource-group "$RESOURCE_GROUP" \
  --name "$BACKEND_APP" \
  --query principalId --output tsv)

ACR_ID=$(az acr show --name "$ACR_NAME" --query id --output tsv)

az role assignment create \
  --assignee "$BACKEND_PRINCIPAL" \
  --role AcrPull \
  --scope "$ACR_ID"

# Configure ACR-based continuous deployment authentication
az webapp config container set \
  --resource-group "$RESOURCE_GROUP" \
  --name "$BACKEND_APP" \
  --docker-registry-server-url "https://${ACR_NAME}.azurecr.io"
```

### 6. Frontend Web App for Containers

```bash
az webapp create \
  --resource-group "$RESOURCE_GROUP" \
  --plan "$PLAN_NAME" \
  --name "$FRONTEND_APP" \
  --deployment-container-image-name "mcr.microsoft.com/appsvc/staticsite:latest"

az webapp config appsettings set \
  --resource-group "$RESOURCE_GROUP" \
  --name "$FRONTEND_APP" \
  --settings WEBSITES_PORT=3000 PORT=3000

FRONTEND_PRINCIPAL=$(az webapp identity assign \
  --resource-group "$RESOURCE_GROUP" \
  --name "$FRONTEND_APP" \
  --query principalId --output tsv)

az role assignment create \
  --assignee "$FRONTEND_PRINCIPAL" \
  --role AcrPull \
  --scope "$ACR_ID"

az webapp config container set \
  --resource-group "$RESOURCE_GROUP" \
  --name "$FRONTEND_APP" \
  --docker-registry-server-url "https://${ACR_NAME}.azurecr.io"
```

### 7. OIDC / Federated Identity for GitHub Actions

Using OIDC avoids storing long-lived Azure credentials as GitHub Secrets.

```bash
# Create an App Registration
APP_ID=$(az ad app create --display-name "taskflow-github-actions" --query appId --output tsv)

# Create the service principal
az ad sp create --id "$APP_ID"
SP_OBJECT_ID=$(az ad sp show --id "$APP_ID" --query id --output tsv)

# Assign Contributor on the resource group
az role assignment create \
  --assignee "$APP_ID" \
  --role Contributor \
  --scope "/subscriptions/$(az account show --query id --output tsv)/resourceGroups/$RESOURCE_GROUP"

# Also assign AcrPush so the pipeline can push images
az role assignment create \
  --assignee "$APP_ID" \
  --role AcrPush \
  --scope "$ACR_ID"

# Add federated credential for the default branch
az ad app federated-credential create \
  --id "$APP_ID" \
  --parameters '{
    "name": "github-oidc-main",
    "issuer": "https://token.actions.githubusercontent.com",
    "subject": "repo:'"$GITHUB_ORG"'/'"$GITHUB_REPO"':ref:refs/heads/master",
    "audiences": ["api://AzureADTokenExchange"]
  }'

# If you also want workflow_dispatch to work from other branches, add:
# "subject": "repo:<org>/<repo>:ref:refs/heads/<branch>"

echo "AZURE_CLIENT_ID = $APP_ID"
echo "AZURE_TENANT_ID = $(az account show --query tenantId --output tsv)"
echo "AZURE_SUBSCRIPTION_ID = $(az account show --query id --output tsv)"
```

---

## GitHub Secrets

Add the following secrets in **Settings → Secrets and variables → Actions** of your repository.

> All secrets marked **Required** must be set before the workflow can succeed.

| Secret name | Required | Description |
|---|---|---|
| `AZURE_CLIENT_ID` | ✅ | App Registration client ID (from OIDC setup above) |
| `AZURE_TENANT_ID` | ✅ | Azure AD tenant ID |
| `AZURE_SUBSCRIPTION_ID` | ✅ | Azure subscription ID |
| `ACR_REGISTRY` | ✅ | Full ACR login server, e.g. `taskflowacr.azurecr.io` |
| `ACR_NAME` | ✅ | Short ACR name, e.g. `taskflowacr` (used for `az acr login`) |
| `AZURE_BACKEND_WEBAPP_NAME` | ✅ | Backend Web App name, e.g. `taskflow-backend-app` |
| `AZURE_FRONTEND_WEBAPP_NAME` | ✅ | Frontend Web App name, e.g. `taskflow-frontend-app` |
| `CONNECTION_STRING` | ✅ | PostgreSQL connection string (Npgsql format) |
| `JWT_SECRET` | ✅ | Secret key for JWT signing (≥ 32 characters) |
| `JWT_ISSUER` | ✅ | JWT issuer value, e.g. `TaskFlow` |
| `JWT_AUDIENCE` | ✅ | JWT audience value, e.g. `TaskFlowUsers` |
| `NEXT_PUBLIC_API_URL` | ✅ | Full URL of the backend API, e.g. `https://taskflow-backend-app.azurewebsites.net` |
| `REDIS_CONNECTION_STRING` | optional | Redis connection string if using a Redis cache. Leave empty or omit if not using Redis — the backend skips Redis registration when this value is blank. |

### PostgreSQL connection string format

```
Host=<server>.postgres.database.azure.com;Database=taskflow;Username=pgadmin;Password=<password>;SslMode=Require
```

---

## How the Pipeline Works

```
push to master
       │
       ├─► build-backend ─► deploy-backend
       │
       └─► build-frontend ─► deploy-frontend
```

1. **build-backend** and **build-frontend** run in parallel.
   - Each job logs into Azure via OIDC, then uses `az acr login` to authenticate to ACR.
   - `docker/build-push-action` builds the image from the respective context (`./Backend` or `./frontend`) and pushes two tags: `<sha>` and `latest`.
   - The SHA-based tag is passed as a job output to the deploy job so the exact immutable digest is deployed.

2. **deploy-backend** and **deploy-frontend** each:
   - Log into Azure via OIDC.
   - Configure App Settings on the Web App (connection strings, JWT config, ports).
   - Pull the new image from ACR and restart the Web App using `azure/webapps-deploy@v3`.

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---------|-------------|-----|
| `Container didn't respond to HTTP pings on port 8080` | `WEBSITES_PORT` not set | Add `WEBSITES_PORT=8080` to Backend App Settings |
| `Container didn't respond to HTTP pings on port 3000` | `WEBSITES_PORT` not set | Add `WEBSITES_PORT=3000` to Frontend App Settings |
| `OIDC: Request was not successful` | Subject claim mismatch | Verify the branch name in the federated credential matches the push branch |
| `unauthorized: authentication required` | ACR pull permission missing | Assign AcrPull role to the Web App's managed identity |
| `Database connection string is missing` | `CONNECTION_STRING` secret not set | Add secret in GitHub Settings |
| Frontend shows API errors | `NEXT_PUBLIC_API_URL` wrong | Verify the secret points to the correct backend URL and the backend app is running |
