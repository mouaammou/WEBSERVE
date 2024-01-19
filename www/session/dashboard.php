<?php
session_start();

if (isset($_SESSION["user"]) && isset($_COOKIE["user"])) {
    $username = $_SESSION["user"];
    echo "Welcome back, $username!<br>";
    echo "Your username from session: $username<br>";
    echo "Your username from cookie: {$_COOKIE["user"]}<br>";
} else {
    echo "You are not logged in.";
}
?>
