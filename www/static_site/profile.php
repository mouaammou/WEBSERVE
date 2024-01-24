<?php
session_start();

if (isset($_SESSION['user']))//USER
{
    echo "welcome to your profile page!<br>";
    echo "Username: " . $_SESSION['user'] . "!<br>";
    echo "First Name: " . $_SESSION['firstname'] . "<br>";
    echo "Last Name: " . $_SESSION['lastname'] . "<br>";
    echo '<a href="logout.php">Logout</a>';
}
else
{
    header('Location: /login.php');
}

?>
