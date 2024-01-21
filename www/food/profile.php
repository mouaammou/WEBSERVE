<?php
session_start();

$file_path = 'html/profile.html';

// Read the file content
$file_content = file_get_contents($file_path);

// Replace the string
// Check if the user is already logged in
if (isset($_SESSION['user']))//USER
{
    $file_content = str_replace('<a href="/login.php">', '<a href="/logout.php">', $file_content);
    $file_content = str_replace('<span>Login</span>', '<span>Logout</span>', $file_content);
    
    $file_content = str_replace('Guest', 'Welcome ' . $_SESSION['user'], $file_content);
    $file_content = str_replace('username','username: ' . $_SESSION['user'], $file_content);
    $file_content = str_replace('firstname','firstname: ' . $_SESSION['firstname'], $file_content);
    $file_content = str_replace('lastname','lastname: ' . $_SESSION['lastname'], $file_content);
    echo $file_content;
    exit();
    // echo "Hello, " . $_SESSION['user'] . "!<br>";
    // echo '<a href="logout.php">Logout</a>';
} else {//GUEST
    // echo 'Welcome, Guest!<br>';
    // echo '<a href="login.php">Login</a>';
    header('Location: /');
}

?>
