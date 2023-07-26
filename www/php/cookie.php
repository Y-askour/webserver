<?php
session_start();

$cookies = array(
    "FirsName" => "amine",
    "LastName" => "rakibe"
);

foreach ($cookies as $name => $result) {
    setcookie($name, $result, time() + (86400 * 30));
}

?>

<html>
<head>
    <title>set Cookie</title>
</head>
<body>
	<?php
	foreach ($cookies as $name => $result) {
		if (!isset($_COOKIE[$name]))
			echo "<h3>Cookie = '" . $name . "' is not  set!</h3><br>";
		else {
			echo "<h1>Cookie = '" . $name . "' is set!</h1><br>";
			echo "<h1>value is: " . $_COOKIE[$name] . "</h1>";
		}
	}
	?>
</body>

</html>