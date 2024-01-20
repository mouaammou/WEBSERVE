<?php
session_start();

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    // Perform authentication (in a real scenario, you'd check against a database)
    // For simplicity, let's assume username is "admin" and password is "password"
    if ($username && $password) {
        // Set session variable
        $_SESSION["user"] = $username;

        // Set a cookie that expires in 1 hour (3600 seconds)
        setcookie("user", $username, time() + 3600, "/");
        
        echo "Login successful! Welcome, $username!";
    } else {
        echo "Invalid username or password.";
    }
} else {
    // Redirect back to the login form if accessed directly
    header("Location: index.html");
    exit();
}
?>
