<?php
session_start();

// Check if the user is already logged in
if (isset($_SESSION['user'])) {
    echo "Hello, " . $_SESSION['user'] . "!<br>";
    echo '<a href="logout.php">Logout</a>';
} else {
    echo 'Welcome, Guest!<br>';
    echo '<a href="login.php">Login</a>';
}
?>
