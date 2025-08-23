+++
title = "90 Days of Learning React"
description = "A comprehensive guide to mastering React in 90 days, covering essential concepts, best practices, and advanced techniques."
date = 2025-07-19
[taxonomies]
tags = ["React", "JavaScript", "Web Development", "Frontend Development"]
[extra]
toc = true
in_search_index = true
image = "/images/react-logo.png"
+++

This blog is where I will document my journey of learning React over the next 90 days. Each day, I will focus on a specific topic or concept, and share my notes, code snippets, and resources.

{{image_url(url="/images/react-logo.png", alt="React Logo")}}
## Day 1: Introduction to React
- **What is React?**
  - A JavaScript library for building user interfaces.
  - Developed by Facebook and released in 2013.
  - Focuses on building reusable UI components.
- **Key Features:**
    - Virtual DOM for efficient rendering.
    - Component-based architecture.
    - Single Page Application (SPA) capabilities.
#### Setup and Installation
- Install Node.js and npm.
- Create a new React app using Create React App:
```bash
npx create-react-app my-app
cd my-app
npm start
```
- Alternatively, you can set up a React project using Vite for faster development:

```bash
npm create vite@latest my-app --template React
cd my-app
npm install 
npm run dev 
```

Structure of a React App:
```plaintext
├── eslint.config.js
├── index.html
├── package.json
├── package-lock.json
├── public
│   └── vite.svg
├── README.md
├── src
│   ├── App.jsx
│   ├── main.jsx
│   └── Nav.jsx
└── vite.config.jsx
```
## Day 2: JSX and Rendering Elements 
### JSX (JavaScript XML)
App.jsx
```js
import React from 'react'
export default App
function App() {

  return (
	  <h1>Hello, World!</h1>

  )
}
```

Nav.jsx
```js
import React from "react";
export default Nav 
function Nav(){
	return(
		<h1>Nav Bar</h1>
	)
}
```

main.jsx
```js 
import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import App from './App.jsx'
import Nav from './Nav.jsx'

createRoot(document.getElementById('root')).render(
  <StrictMode>
    <App />
    <Nav />
  </StrictMode>,
)
```
### Key Points
Component should  start with an uppercase letter, and the file name should match the component name. This is a convention in React to differentiate components from regular HTML elements.

 We can name both App.js & App.jsx but in vite development environment we have to use jsx

We can return only one component by a jsx function so it's good to wrap in <></> or <div></div>

## Day 3: Create own Custom components
### Creating Custom components
script.js 
```js 
let rootElement = document.getElementById('root');

const reactElement = {
	type: 'a',
	props: {
		href: 'https://mukund.xyz',
		_target: '_blank',
	},
	children: 'Visit mukund.xyz',
};


CreateReactElement(reactElement, rootElement);


function CreateReactElement(element, container) {
	const domElement = document.createElement(element.type);
	domElement.textContent = element.children;
        for (const prop in element.props) {
			if (prop === 'children') continue;
		domElement.setAttribute(prop, element.props[prop]);
	}
container.appendChild(domElement);
}
```

index.html
```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<title></title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
	
		<div id="root">
			
		</div>
	</body>
		<script src="script.js"></script>
</html>
```

Create a custom component in React that renders an anchor tag with a link to your website.

main.jsx
```js 
import React from 'react'
import { createRoot } from 'react-dom/client'
const reactElement = React.createElement(
  'a',
  {
    href: 'https://mukund.xyz',
    target: '_blank',
  },
  'Visit mukund.xyz'
)



createRoot(document.getElementById('root'))
.render(
reactElement
)
```

## Days 4 Hooks and State Management
Hooks are functions that let you use state and other React features in functional components. They allow you to manage state, lifecycle events, and side effects without writing class components.

- **useState Hook**
  - Allows you to add state to functional components.
  - Returns an array with the current state and a function to update it.
  - Hooks can't be called inside loops, conditions, or nested functions. They should be called at the top level of your component.
  

```js 
import { useState } from 'react'
import './App.css'

function App() {
  const [count, setCount] = useState(0)
function counter() {
    setCount(count + 1)
  }
function counterDecrease() {
	    if (count <= 0) {
      return
    }
    setCount(count - 1)
  }
  return (
    <>
	  
	<button className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded" onClick={counter}>Click To Increase count : {count}</button>
	  	<button className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded" onClick={counterDecrease}>Click To Decrease count : {count}</button>
	  
	  <h1 className="text-3xl font-bold underline">{count}</h1>

    </>
  )
}

export default App
```

#Day 5: Virtual DOM and Reconciliation

React Fiber Architecture is an ongoing reimplementation of React's core algorithm. It is the culmination of over two years of research by the React team.

### Virtual dom 
Virtual DOM (VDOM) is a programming concept where a virtual representation of the UI is kept in memory and synced with the real DOM by a library such as ReactDOM. This process is called reconciliation.

### What is reconciliation?
    The algorithm React uses to diff(differentiate) one tree with another to determine which parts need to be changed.

**reconciliation** is the process through which React updates the DOM. When a component's state or props change, React creates a new Virtual DOM tree and compares it with the previous one. It then calculates the most efficient way to update the real DOM to match the new Virtual DOM.


### Key Terms

Other key features include the 

**ability to pause**, 
**abort**, or **reuse work as new updates come in**
**the ability to assign priority to different types of updates**
and new concurrency primitives.

The algorithm React uses to diff one tree with another to determine which parts need to be changed.

{{ image_url(url="https://media.geeksforgeeks.org/wp-content/uploads/20241212235246933476/Browser-DOM-Virtual-DOM.webp", alt="Online Image Example") }}

**Reconciliation is the algorithm behind what is popularly understood as the "virtual DOM."**

A high-level description goes something like this: when you render a React application, a tree of nodes that describes the app is generated and saved in memory. This tree is then flushed to the rendering environment — for example, in the case of a browser application, it's translated to a set of DOM operations. When the app is updated (usually via setState), a new tree is generated. The new tree is diffed with the previous tree to compute which operations are needed to update the rendered app.

- Although Fiber is a ground-up rewrite of the reconciler, the high-level algorithm described in the React docs will be largely the same. 

The key points are:

   
- Different component types are assumed to generate substantially different trees. React will not attempt to diff them, but rather replace the old tree completely.
  
- Diffing of lists is performed using keys. Keys should be "stable, predictable, and unique."

### In Array Why keys should be introduced?
- **In Fiber algorithm if we want to impove performance of list when itrating over array of items  we should use keys.**

When React sees a list of elements, it needs to determine which items have changed, been added, or been removed. Keys help React identify which items in the list correspond to which elements in the previous render.

Scheduling
   - the process of determining when work should be performed.

The key points are:

- In a UI, it's not necessary for every update to be applied immediately; in fact, doing so can be wasteful, causing frames to drop and degrading the user experience.
- Different types of updates have different priorities — an animation update needs to complete more quickly than, say, an update from a data store.
- A push-based approach requires the app (you, the programmer) to decide how to schedule work. A pull-based approach allows the framework (React) to be smart and make those decisions for you.

### What is a fiber?
We've established that a primary goal of Fiber is to enable React to take advantage of scheduling. Specifically, we need to be able to

- pause work and come back to it later.
- assign priority to different types of work.
- reuse previously completed work.
- abort work if it's no longer needed.
### Structure of a Fiber
In concrete terms, a fiber is a JavaScript object that contains information about a component, its input, and its output.

A fiber corresponds to a stack frame, but it also corresponds to an instance of a component.

Here are some of the important fields that belong to a fiber. (This list is not exhaustive.)
`type` and `key`
 
---
