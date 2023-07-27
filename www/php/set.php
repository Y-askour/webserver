<?php
$cookies = array(
    "firstname" => "amine",
    "lastname" => "rakibe"
);
foreach ($cookies as $name => $value) {
    setcookie($name, $value, time() + (86400 * 30));
}

?>

<html>
<head>
    <title> test cookie </title>
</head>
<body>
    <?php
    foreach ($cookies as $name => $value) {
        if (!isset($_COOKIE[$name]))
            echo "<h2>Cookie = " . $name . " is not  set!</h2><br>";
        else {
            echo "<h1>Cookie = " . $name . " is set </h1>" .  "<h2>Value = " . $_COOKIE[$name] . "</h2><br>";
        }
    }
    ?>
</body>
</html>
