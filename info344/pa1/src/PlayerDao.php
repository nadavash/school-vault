<?php
include_once "Player.php";

define("MAX_DISTANCE_FULL", 5);
define("MAX_DISTANCE_PART", 2);

// Represents a data accessor object that retrieves player information
// from the database.
class PlayerDao {
	private $conn;

	/**
	 * Creates a new PlayerDao with the given settings.
	 * @param Array $settings An associative array of settings to initialize PDO object.
	 */
	public function __construct($settings) {
		try {
			$conn = new PDO("mysql:host={$settings['host']};dbname={$settings['db']}",
				$settings["username"], $settings["pass"]);
			$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			$conn->setAttribute(PDO::ATTR_PERSISTENT, true);
			$this->conn = $conn;
		} catch(PDOException $e) {
			print "Error!: " . $e->getMessage() . "<br/>";
    		die();
		}
	}

	/**
	 * Searches for players with the given name. Implements a fuzzy-search algorithm
	 * to return closest matches.
	 * @param  String $name The name to search for in the database.
	 * @return Array       An array of Player objects with a close match to the given
	 *                     name query.
	 */
	public function getPlayersByName($name) {
		try {
			$rows = $this->conn->query("SELECT * FROM Players");
			$players = Array();

			foreach ($rows as $row) {
				$full = $this->fullMatchDist($name, $row["PlayerName"]);
				$partial = $this->partialMatchDist($name, $row["PlayerName"]);
				if ($full === 0) {
					// Put exact match in the beginning.
					$players = Array($this->playerFromRow($row));
				} elseif ($full < MAX_DISTANCE_FULL || $partial < MAX_DISTANCE_PART) {
					array_push($players, $this->playerFromRow($row));
				}
			}

			return $players;
		} catch(PDOException $e) {
			print "Error!: " . $e->getMessage() . "<br/>";
    		die();
		}
	}

	/**
	 * Gets the player data for players with the exact name provided.
	 * @param  String $name The search string.
	 * @return Array       An array of Player objects that match the name.
	 */
	public function getPlayersByExactName($name) {
		$name = strtolower(trim($name));
		try {
			$stmt = $this->conn->prepare("SELECT * FROM Players WHERE PlayerName = 'LeBron James'");
			//$stmt->bindValue(":player_name", $name, PDO::PARAM_STR);
			$stmt->execute();
			$results = $stmt->fetchAll();

			$players = array();
			foreach ($results as $row) {
				array_push($players, $this->playerFromRow($row)->ToAssociativeArray());
			}

			return $players;
		} catch (PDOException $e) {
			print "Error!: " . $e->getMessage() . "<br/>";
    		die();	
		}
	}

	/**
	 * Returns a player object for the given PlayerId. Returns null if PlayerId does not
	 * exist in the database.
	 * @param  int $id The id of the player to return.
	 * @return Player     The player object.
	 */
	public function getPlayerById($id) {
		try {
			$stmt = $this->conn->perpare("SELECT * FROM Players WHERE PlayerId = :player_id");
			$stmt->bind("player_id", $id);

			$results = $stmt->execute();
			if ($results) {
				return $this->playerFromRow($results[0]);
			}

			return null;
		} catch(PDOException $e) {
			print "Error!: " . $e->getMessage() . "<br/>";
    		die();
		}
	}

	/**
	 * Returns the levenshtein distance for a full name match.
	 * @param  String $query      Name to search for
	 * @param  String $playerName The actual player name
	 * @return int             The levenshtein distance.
	 */
	private function fullMatchDist($query, $playerName) {
		return levenshtein(strtolower($query), strtolower($playerName));
	}

	/**
	 * Returns the lowest levenshtein distance for a partial match (only first/mid/last name).
	 * @param  String $query      Name to search for
	 * @param  String $playerName The actual player name
	 * @return int             The lowest levenshtein distance of the name part.
	 */
	private function partialMatchDist($query, $playerName) {
		$query = strtolower($query);
		$splitName = explode(" ", strtolower($playerName));

		$smallest = PHP_INT_MAX;
		foreach ($splitName as $namePart) {
			$dist = levenshtein($query, $namePart);
			if ($dist < $smallest) {
				$smallest = $dist;
			}
		}

		return $smallest;
	}

	/**
	 * Creates a player object from the given SQL row.
	 * @param  Object $row Sequel row
	 * @return Player      The player object
	 */
	private function playerFromRow($row) {
		$player = new Player();
		$player->setPlayerName($row["PlayerName"]);
		$player->setGamesPlayed($row["GP"]);
		$player->setFieldGoalPercentage($row["FGP"]);
		$player->setThreePointPercentage($row["TPP"]);
		$player->setFreeThrowPercentage($row["FTP"]);
		$player->setPointsPerGame($row["PPG"]);
		return $player;
	}
}