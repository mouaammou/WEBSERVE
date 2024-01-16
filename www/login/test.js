#!/usr/bin/env node

const fs = require('fs');

// Content-Type header indicating JSON response
console.log("Content-Type: application/json");


// Function to parse and retrieve cookies from the HTTP headers
function parseCookies() {
    const cookies = {};
    if (process.env.HTTP_COOKIE) {
        process.env.HTTP_COOKIE.split(';').forEach(cookie => {
            const parts = cookie.split('=');
            cookies[parts[0].trim()] = (parts[1] || '').trim();
        });
    }
    return cookies;
}

// Function to generate a random session ID
function generateSessionId() {
    return Math.random().toString(36).substring(2, 15);
}

// Function to read session data from a file
function readSessionData() {
    try {
        const data = fs.readFileSync('./session_data.json', 'utf8');
        return JSON.parse(data);
    } catch (error) {
        return {};
    }
}

// Function to write session data to a file
function writeSessionData(sessionData) {
    fs.writeFileSync('./session_data.json', JSON.stringify(sessionData), 'utf8');
}

// Function to authenticate a user
function authenticateUser(username, password) {
    // Hardcoded user list (replace with a database in a real-world scenario)
    const users = {
        "user1": "password1",
        "user2": "password2"
    };

    // Check if the provided username and password match
    return users[username] === password;
}

// Function to handle user authentication and session
function handleAuthentication(username, password) {
    const cookies = parseCookies();
    let sessionData = readSessionData();

    // Check if the user is already authenticated
    let sessionId = cookies["SESSION_ID"];
    if (sessionId && sessionData[sessionId] && sessionData[sessionId].authenticated) {
        // User is already authenticated
        console.log();
        console.log(JSON.stringify({ message: "User is already authenticated" }));
    } else {
        // Authenticate the user
        if (authenticateUser(username, password)) {
            // If authentication is successful, generate or retrieve the session ID
            sessionId = sessionId || generateSessionId();
            sessionData[sessionId] = { authenticated: true, username: username };

            // Save session data to the file
            writeSessionData(sessionData);

            // Set the SESSION_ID cookie
            console.log(`Set-Cookie: SESSION_ID=${sessionId}; Path=/`);
            console.log();
            // JSON response indicating successful authentication
            console.log(JSON.stringify({ message: "Authentication successful", username: username }));
        } else {
            console.log();
            // JSON response indicating authentication failure
            console.log(JSON.stringify({ message: "Authentication failed" }));
        }
    }
}

// Extract username and password from the query string
const query = process.env.QUERY_STRING || '';
const params = new URLSearchParams(query);
const username = params.get('username') || '';
const password = params.get('password') || '';

// Execute the authentication and session handling logic
handleAuthentication(username, password);
