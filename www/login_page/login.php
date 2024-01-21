<?php
session_start();

// Check if the login form is submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = $_POST['username'];

    // Store the username in a session variable
    $_SESSION['user'] = $username;

    // Set a cookie with the username (optional)
    setcookie('user', $username, time() + 3600); // Cookie expires in 1 hour

    header('Location: index.php');
    exit();
}
?>

<form method="post" action="">
    <label for="username">Username:</label>
    <input type="text" name="username" required>
    <button type="submit">Login</button>
</form>
