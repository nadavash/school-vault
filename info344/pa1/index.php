<!DOCTYPE html>
<?php
require_once "src/PlayerDao.php";

define("IMAGE_URL", "http://i.cdn.turner.com/nba/nba/.element/img/2.0/sect/statscube/players/large/");

if (isset($_GET["q"])) {
	$settings = parse_ini_file("default.ini");
	$dao = new PlayerDao($settings);
	$query = $_GET["q"];
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
			?>
			<div class="page-header">
	  			<h2>Results for <small>"<?= $query ?>"</small></h2>
			</div>
			<?php
			foreach ($dao->getPlayersByName($query) as $player) {
				printPlayerCard($player);
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
		<img class="player-img" src="<?= $imgUrl ?>" alt="player profile" />
		<div class="player-stats panel">
			<h3><?= $player->getPlayerName() ?></h3>
			<table class="player-stats table">
				<caption>Stats</caption>
				<thead>
					<tr>
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