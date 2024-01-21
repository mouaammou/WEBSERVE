<?php
session_start();

// Unset all session variables
$_SESSION = array();

// Destroy the session
session_destroy();

// Unset and expire the cookie (if it exists)
if (isset($_COOKIE['user'])) {
    unset($_COOKIE['user']);
    setcookie('user', '', time() - 3600, '/'); // Expire the cookie (set to the past time)
}

// Redirect to the login page
header('Location: index.php');
exit();
?>
