<?PHP
echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<title> CGI </title>";
echo "</head>";
echo "<body>";

if ($_SERVER['REQUEST_METHOD'] == 'GET')
{   
    if(isset($_GET["first_name"]) && isset($_GET["last_name"]));
    echo "<h1>Method GET: ". $_GET["first_name"] . " " . $_GET["last_name"] . "</h1>";
}
else
{
    if(isset($_POST["first_name"]) && isset($_POST["last_name"]));
        echo "<h1>Method POST: ". $_POST["username"] . " " . $_POST["password"] . "</h1>";
}
echo "</body>";
echo "</html>";
?>