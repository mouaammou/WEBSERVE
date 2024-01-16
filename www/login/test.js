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

// Function to handle the session
function handleSession() {
    const cookies = parseCookies();
    let sessionData = readSessionData();

    // Check if the session ID is present in cookies
    let sessionId = cookies["SESSION_ID"];
    if (!sessionId) {
        // If not, generate a new session ID
        sessionId = generateSessionId();
        console.log(`Set-Cookie: SESSION_ID=${sessionId}; Path=/`);
    }

    // Increment and display counter in the session
    let counter = sessionData[sessionId] || 0;
    counter++;
    sessionData[sessionId] = counter;

    // Save session data to the file
    writeSessionData(sessionData);

    // JSON response with session information
    const response = {
        sessionId: sessionId,
        counter: counter
    };
    console.log();
    console.log(JSON.stringify(response));
}

// Execute the session handling logic
handleSession();
