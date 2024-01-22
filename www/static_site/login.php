<?php
session_start();
// Check if the login form is submitted
if (isset($_SESSION['user']))//USER
{
    header('Location: /profile.php');
    exit();
}

if ($_SERVER['REQUEST_METHOD'] === 'POST')
{
    $username = $_POST['username'];
    $firstname = $_POST['firstname'];
    $lastname = $_POST['lastname'];

    // Store the username in a session variable
    $_SESSION['user'] = $username;
    $_SESSION['firstname'] = $firstname;
    $_SESSION['lastname'] = $lastname;



    header('Location: /profile.php');
    exit();
}
else
{
    header('Location: /login.html');
    exit();
}
?>
