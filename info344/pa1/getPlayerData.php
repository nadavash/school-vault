<?php
require_once "src/PlayerDao.php";

header('content-type: application/json; charset=utf-8');

if (!isset($_GET["q"])) {
	die("No query provided.");
}

$settings = parse_ini_file("default.ini");
if (!$settings) {
	die("Failed to initialize nba search.");
}

$dao = new PlayerDao($settings);
if ($_GET["q"] !== "") {
	$query = $_GET["q"];
}

$results = array(
	"players" => $dao->getPlayersByExactName($query)
);

echo "callback(" . json_encode($results) . ")";
