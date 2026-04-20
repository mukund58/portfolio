# Stage 1: Build
FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build
WORKDIR /src

COPY *.csproj ./
RUN dotnet restore

COPY . .
RUN dotnet publish -c Release -o /app/publish

# Stage 2: Runtime
FROM mcr.microsoft.com/dotnet/aspnet:8.0 AS final
WORKDIR /app

# ASP.NET Core 8 defaults to port 8080 inside the container.
# Override by setting the ASPNETCORE_URLS environment variable at runtime.
# For Azure Web Apps for Containers, also set WEBSITES_PORT=8080 in App Settings.
ENV ASPNETCORE_URLS=http://+:8080

EXPOSE 8080

COPY --from=build /app/publish .

ENTRYPOINT ["dotnet", "Backend.dll"]
