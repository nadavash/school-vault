<!DOCTYPE html>
<?php
require_once "PlayerDao.php";
$settings = parse_ini_file("../default.ini");
$dao = new PlayerDao($settings);

if (isset($_GET["q"])) {
	$query = $_GET["q"];
}
?>
<html>
<head>
	<title>NBA Players</title>
</head>
<body>
	<div>
		<h1>Results for '<?php echo $query ?>'</h1>
		<?php
		foreach ($dao->getPlayersByName($query) as $player) {
			echo $player->getPlayerName() . "<br/>";
		}

		?>
	</div>
</body>
</html>