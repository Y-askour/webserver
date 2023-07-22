<?php
echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<title> CGI </title>";
echo "</head>";
echo "<body>";

if ($_SERVER['REQUEST_METHOD'] == 'GET')
    echo "<h1>username GET: ". $_GET["username"] . " password: " . $_GET["password"] . "</h1>";
else
    echo "<h1>username POST: ". $_POST["username"] . " password: " . $_POST["password"] . "</h1>";

echo "</body>";
echo "</html>";
?>

