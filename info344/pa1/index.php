<!DOCTYPE html>
<?php
require_once "src/PlayerDao.php";

define("IMAGE_URL", "http://i.cdn.turner.com/nba/nba/.element/img/2.0/sect/statscube/players/large/");

if (isset($_GET["q"])) {
	$settings = parse_ini_file("default.ini");
	if (!$settings) {
		die("Failed to initialize nba search.");
	}
	$dao = new PlayerDao($settings);
	if ($_GET["q"] !== "") {
		$query = $_GET["q"];
	}
}
?>
<html>
<head>
	<title>NBA Players</title>
	<!-- Latest compiled and minified CSS -->
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css">

	<!-- Optional theme -->
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap-theme.min.css">

	<link rel="stylesheet" href="/css/main.css">
</head>
<body>
	<div class="results-page-content">
		<img class="top-banner" src="/assets/nba-logo.jpg" alt="player profile" />
		<form method="GET" action="/">
			<div class="input-group">
				<?php
				if (isset($query)) {
					?>
					<input name="q" type="text" class="form-control" placeholder="Search for player..." 
						value="<?= $query ?>"/>
					<?php
		    		
				} else {
					?>
					<input name="q" type="text" class="form-control" placeholder="Search for player..." />
					<?php
				}
		    	?>
		    	<span class="input-group-btn">
		    		<button class="btn btn-default" type="submit">Go!</button>
		    	</span>
		    </div>	
		</form>
		<?php
		if (isset($query)) {
			$players = $dao->getPlayersByName($query);
			if (count($players) === 0) {
				?>
				<div class="page-header">
		  			<h2>No results for "<?= $query ?>"</h2>
				</div>
				<?php
			} else {
				?>
				<div class="page-header">
		  			<h2>Results for "<?= $query ?>"</h2>
				</div>
				<?php
				foreach ($players as $player) {
					printPlayerCard($player);
				}
			}
		}
		?>
	</div>
</body>
</html>
<?php

/**
 * Prints a player card for the given player object.
 * @param  Player $player The player object.
 */
function printPlayerCard($player) {
	// Compose the image url
	$imgUrl = IMAGE_URL . str_replace(" ", "_", strtolower($player->getPlayerName())) . ".png";
	?>
	<div class="player-card panel panel-default">
		<object class="player-img" data="<?= $imgUrl ?>" type="image/png">
    		<img class="player-img" src="/assets/stock.png" alt="player profile" />
		</object>
		<div class="player-info">
			<h3><?= $player->getPlayerName() ?></h3>
			<table class="player-stats table">
				<caption>Stats</caption>
				<thead>
					<tr class="player-stats-row">
						<th>GP</th>
						<th>FGP</th>
						<th>TPP</th>
						<th>FTP</th>
						<th>PPG</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td><?= $player->getGamesPlayed() ?></td>
						<td><?= $player->getFieldGoalPercentage() ?></td>
						<td><?= $player->getThreePointPercentage() ?></td>
						<td><?= $player->getFreeThrowPercentage() ?></td>
						<td><?= $player->getPointsPerGame() ?></td>
					</tr>
				</tbody>
			</table>
		</div>	
	</div>
	<?php
}

?>