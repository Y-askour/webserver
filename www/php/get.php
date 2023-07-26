<?PHP
echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<title> CGI </title>";
echo "</head>";
echo "<body>";

if ($_SERVER['REQUEST_METHOD'] == 'GET')
{   
    if(isset($_GET["username"]) && isset($_GET["password"]));
        echo "<h1>Method GET: ". $_GET["username"] . " " . $_GET["password"] . "</h1>";
}
else
{
    if(isset($_POST["first_name"]) && isset($_POST["last_name"]));
        echo "<h1>Method POST: ". $_POST["username"] . " " . $_POST["password"] . "</h1>";
}
echo "</body>";
echo "</html>";
?>