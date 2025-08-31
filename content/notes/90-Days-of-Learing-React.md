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

## Day 5: Virtual DOM and Reconciliation

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

## Day 6: Props for  Component Communication  
Props (short for "properties") are a way to pass data from a parent component to a child component in React. They are read-only and cannot be modified by the child component.

#### Function and Class Components

The simplest way to define a component is to write a JavaScript function:

```js 
function Welcome(props) {
  return <h1>Hello, {props.name}</h1>;
}
```

This function is a valid React component because it accepts a single “props” (which stands for properties) object argument with data and returns a React element. `We call such components “function components” because they are literally JavaScript functions.`

You can also use an `ES6` class to define a component:
```js 
class Welcome extends React.Component {
  render() {
    return <h1>Hello, {this.props.name}</h1>;
  }
}
```

The above two components are equivalent from React’s point of view.

How to create Parents and Child components using props 

`ParentCard.jsx`

```js 
import Card from "./Card";

function ParentCard() {
  const products = [
    {
      name: "Apple Watch",
      price: "$899",
      stock: "In Stock",
      img: "https://files.refurbed.com/ii/apple-watch-series-7-titan-45mm-1705929512.jpg"
    },
    {
      name: "Samsung Galaxy Watch",
      price: "$699",
      stock: "Only 3 Left",
      img: "https://m.media-amazon.com/images/I/71Swqqe7XAL._AC_UL600_FMwebp_QL65_.jpg"
    },
    {
      name: "Noise Smartwatch",
      price: "$199",
      stock: "Out of Stock",
      img: "https://sell.gameloot.in/wp-content/uploads/sites/4/2022/05/Samsung-Galaxy-Watch-4-Classic-LTE-42mm.jpg"
    }
  ];

  return (
    <div className="container mx-auto grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-6 p-5">
      {products.map((product, index) => (
        <Card key={index} productData={product} />
      ))}
    </div>
  );
}

export default ParentCard;
```

`Card.jsx`

```js 
function Card(props) {
  const obj = props.productData;

const { name= "default Watch ", price= "$899", stock= "Out of Stock", img="https://files.refurbed.com/ii/apple-watch-series-7-titan-45mm-1705929512.jpg" } = obj;

    return (
        <div className="max-w-sm rounded overflow-hidden shadow-lg">
        <img className="w-full h-64 object-cover" src={img} alt={name} />
        <div className="px-6 py-4">
            <div className="font-bold text-xl mb-2">{name}</div>
            <p className="text-gray-700 text-base">Price: {price}</p>
            <p className={`text-base ${stock === "In Stock" ? "text-green-500" : stock === "Only 3 Left" ? "text-yellow-500" : "text-red-500"}`}>
            {stock}
            </p>
        </div>
        <div className="px-6 pt-4 pb-2">
            <button className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded">
            Buy Now
            </button>
        </div>
        </div>
    );
    }

```
## Day 7: useState Hook in Depth
The `useState` hook is a fundamental building block in React that allows you to add state to functional components. It provides a way to manage and update state within your components without the need for class components.
### Syntax
```js 
const [state, setState] = useState(initialState);
```
- `state`: The current state value.
- `setState`: A function that allows you to update the state.
- `initialState`: The initial value of the state.

### Example of useState Hook for Background Color Change on Button Click 
```js 
import { useState } from 'react'

function App() {
  const [colorbg, setColor] = useState("Black");

  return (
    <>
	  <div className="App" style={{ backgroundColor: colorbg, transition: 'background-color 0.5s ease', height: '100vh' }}>
	<h1 className="text-3xl font-bold text-center min-h-screen">
	  Background Color Change on Click
	</h1>
	  
	  <div className="flex flex-row items-top justify-center p-10 gap-5 absolute top-10 w-full bg-white/10 ">
	  	<button className="bg-green hover:bg-green-700 text-white  font-bold py-2 px-4 rounded border" onClick={()=> {setColor("Green")}} >Green</button>
	  	<button className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Blue")}>Blue</button>
	  	<button className="bg-red-500 hover:bg-red-700 text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Red")}>Red</button>
	  	<button className="bg-yellow-500 hover:bg-yellow-700 text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Yellow")}>Yellow</button>
	  	<button className="bg-purple-500 hover:bg-purple-700 text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Purple")}>Purple</button>
	  	<button className="bg-pink-500 hover:bg-pink-700 text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Pink")}>Pink</button>
	  	<button className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Gray")}>Gray</button>
	  	<button className="bg-black hover:bg-black text-white font-bold py-2 px-4 rounded"onClick={() => setColor("Black")}>Black</button>
	  	<button className="bg-white hover:bg-white text-black font-bold py-2 px-4 rounded border"onClick={() => setColor("White")}>White</button>

	  	</div>
	  </div>
    </>
  )
}

export default App
```
### Explanation
- We import the `useState` hook from React.
- We define a functional component `App`.
- We use the `useState` hook to create a state variable `colorbg` with an initial value of "Black".
- We create buttons that, when clicked, update the `colorbg` state to different colors.
- The background color of the `div` changes based on the current value of `colorbg`.
### Key points
- The `useState` hook can be called multiple times in a single component to manage different pieces of state.
- State updates are asynchronous, so the new state value may not be immediately available after calling the `setState` function.
- You can pass a function to the `setState` function if you need to update the state based on the previous state.
### Example of updating state based on previous state
```js
setCount(prevCount => prevCount + 1);
```
This ensures that you are working with the most up-to-date state value, especially when multiple state updates are queued.
### Conclusion
The `useState` hook is a powerful and essential tool for managing state in React functional Components. It allows you to create dynamic and interactive user interfaces by enabling stateful behavior in your components.

## Day 8: useEffect, useRef and useCallback Hooks 

### useEffect Hook
The `useEffect` hook allows you to perform side effects in your functional components. Side effects can include data fetching, subscriptions, or manually changing the DOM. The `useEffect` hook runs after the component renders.

### Syntax
```js 
useEffect(() => {
  // Side effect code here
}, [dependencies]);
```
- The first argument is a function that contains the side effect code.
- The second argument is an optional array of dependencies. The effect will only re-run if one of the dependencies has changed.

### useRef Hook 
The `useRef` hook allows you to create a mutable reference that persists across renders. It can be used to access DOM elements directly or to store any mutable value that does not cause a re-render when updated.
### Syntax
```js 
const ref = useRef(initialValue);
```
- `ref`: The mutable reference object.
- `initialValue`: The initial value of the reference.

### useCallback Hook 
The `useCallback` hook returns a memoized version of a callback function that only changes if one of the dependencies has changed. It is useful for optimizing performance by preventing unnecessary re-creations of functions.
### Syntax 
```js 
const memoizedCallback = useCallback(() => {
  // Callback code here
}, [dependencies]);
```
- The first argument is the callback function. 
- The second argument is an array of dependencies. The callback will only be re-created if one of the dependencies has changed.

### Example using useEffect, useRef, and useCallback
```js 








