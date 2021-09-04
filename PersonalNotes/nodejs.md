# Node.js notes

## Why?
Node.js is a JS runtime environment that runs on Google Chrome's V8 engine.
It uses non-blocking/event-driven IO that allows for concurrent threads instead of single threads for each user/request to the web server.

<image>

Web servers handle IO requests, this could be calls to APIs, writing or reading files etc. Traditional web-servers create threads for each request/user; use blocking IO where each thread cannot proceed and the web server cannot handle another thread until the IO is fully recieved. Nodejs uses non-blocking IO hence creates concurrent multi-threads for requests - which are queued right away and the function that calls it can immeditely return and allow the webserver to respond/handle more requests in parallel. The actual IO is processed at some later point by the kernel.



## JS with HTML and CSS

### Usage
A JS script can be incorporated into a HTML page externally (<script src="jsscriptname.js"></script>) or internally (<script> <SCRIPT-CONTENT-HERE> </script>)

JavaScript can "display" data in different ways:

* Writing into an HTML element, using innerHTML.
* Writing into the HTML output using document.write().
* Writing into an alert box, using window.alert().
* Writing into the browser console, using console.log().

JavaScript uses the keywords var, let and const to declare variables.
* Declaring variables with var allows the user to redeclare and use them globally but this can quickly get difficult to maintain.
* Using let only sets the value of an identifier within the block scope and never outside of it. You cannot redeclare the same variable within the same block with let. Variables defined with let cannot be redeclared and cannot be used out of its block scope.
* Constants declared with const cannot be redeclared and are locally accessible within their blocks only.

### Syntax Details
Arrays
const cars = ["Saab", "Volvo", "BMW"];

Objects
const person = {firstName:"John", lastName:"Doe", age:50, eyeColor:"blue"};


### Events
Events
An HTML event can be something the browser does, or something a user does.

Here are some examples of HTML events:

An HTML web page has finished loading
An HTML input field was changed
An HTML button was clicked

### DOM
With the HTML [DOM](https://www.w3schools.com/js/js_htmldom.asp), JavaScript can access and change all the elements of an HTML document.


### Web APIs

All browsers have a set of [built-in Web APIs](https://www.w3schools.com/js/js_validation_api.asp) to support complex operations, and to help accessing data.

### AJAX 

AJAX = Asynchronous JavaScript And XML.

AJAX is not a programming language.

AJAX just uses a combination of:

A browser built-in XMLHttpRequest object (to request data from a web server)
JavaScript and HTML DOM (to display or use the data)




## Project: Record audio on webpage and upload them to a server on request.

