<html>
<head>
<title>Login page</title>
</head>

<body>

<?php
$file = fopen("clients.txt", 'r');

$success=0;
while( !feof($file)) {
	$line=fgets($file);

	if(strcmp(trim($line), $_GET['id'])==0) {
		$success=1;
	}
}

fclose($file);

if($success==1) {
	echo '<h1>Welcome, my client!</h1><br>';
} else {
	echo '<h1>Get out! You are not my client.</h1><br>';
}
?>

</body>
</html>