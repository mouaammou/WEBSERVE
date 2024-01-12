<?php
// Start or resume a session
session_start();

// Set a session variable
$_SESSION['user'] = 'JohnDoe';

// Set a cookie with the session ID
$sessionCookieParams = session_get_cookie_params();
setcookie(session_name(), session_id(), time() + $sessionCookieParams['lifetime'], $sessionCookieParams['path'], $sessionCookieParams['domain'], $sessionCookieParams['secure'], $sessionCookieParams['httponly']);

// Output some content to demonstrate
echo "Session and cookie set successfully. User: {$_SESSION['user']}";

// Close the session
session_write_close();
?>
