<?php
session_start();

$firsName = "amine";
$lastName = "rakibe";


setcookie("firstname", $firsName, time() + (86400 * 30), "/");
setcookie("lastname", $lastName, time() + (86400 * 30) , "/");


echo "<html>";
echo "<body>";
echo "<h1>Cookie</h1>";

if (isset($_COOKIE[$firsName]) && isset($_COOKIE[$lastName]))
{
    // echo "<h3>Cookie = '" . $firstname . "' is set!</h3><br>";
    echo "<h3>Cookie = '" . $lastname . "' is set!</h3><br>";
    echo "<h4>firstname is " . $_COOKIE[$firstname] . "</h4>";
    // echo "<h4>lastname is " . $_COOKIE[$lastName] . "</h4>";
}
else 
{
    echo "<h3>" . $firsName . "is not  set!</h3><br>";
}
echo "</body>";
echo "</html>";
?>