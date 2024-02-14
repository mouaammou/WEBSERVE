<?php
// Start or resume the session
session_start();

// Unset all session variables
if (isset($_COOKIE[session_name()]))
{
    unset($_COOKIE[session_name()]);
    setcookie(session_name(), '', time() - 3600);
}

session_unset();

// Destroy the session
session_destroy();

// Unset and expire the session cookie (if it exists)
// Redirect to the login page or any desired destination
header('Location: /login.php');
exit();
?>
