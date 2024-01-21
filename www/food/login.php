<?php
session_start();
// Check if the login form is submitted
if (isset($_SESSION['user']))//USER
{
    header('Location: /');
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

    // Set a cookie with the username (optional)
    // setcookie('user', $username, time() + 3600); // Cookie expires in 1 hour

    header('Location: /');
    exit();
}
else
{
    $file_content = file_get_contents('html/login.html');
    echo $file_content;
    exit();
}
?>

<!-- <form method="post" action="">
    <label for="username">Username:</label>
    <input type="text" name="username" required>
    <button type="submit">Login</button>
</form> -->
