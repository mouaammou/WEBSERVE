<?php
session_start();

$file_path = 'html/about.html';

// Read the file content
$file_content = file_get_contents($file_path);

// Replace the string
// Check if the user is already logged in
if (isset($_SESSION['user']))//USER
{
    $safe_username = htmlspecialchars($_SESSION['user'], ENT_QUOTES, 'UTF-8');
    $file_content = str_replace('<a href="/cgi/login.php">', '<a href="/cgi/logout.php">', $file_content);
    $file_content = str_replace('Guest', 'Welcome ' . $safe_username, $file_content);
    $file_content = str_replace('<span>Login</span>', '<span>Logout</span>', $file_content);
    // echo "Hello, " . $_SESSION['user'] . "!<br>";
    // echo '<a href="logout.php">Logout</a>';
} else {//GUEST
    // echo 'Welcome, Guest!<br>';
    // echo '<a href="login.php">Login</a>';
}
echo $file_content;
?>

