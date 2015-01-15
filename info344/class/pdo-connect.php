<!DOCTYPE html>
<html>
<head>
	<title></title>
</head>
<body>
	<p>
		<?php
		$username = "info344mysqlpdo";
		$password = "chrispaul";
		$conn = new PDO('mysql:host=uwinfo344.chunkaiw.com;dbname=info344mysqlpdo', $username, $password);
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

		foreach ($conn->query("SELECT * FROM Books WHERE price > 5") as $row) {
			print_r($row);
			?> <br /> <?php
		}

		?>
	</p>
</body>
</html>