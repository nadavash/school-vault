<?php
// The Player class represents a simple data class that holds related nba
// statistics for a given player.
class Player {
	private $playerName;
	private $gamesPlayed;
	private $fieldGoalPercentage;
	private $threePointPercentage;
	private $freeThrowPercentage;
	private $pointsPerGame;

	public function getPlayerName() {
		if (!$this->playerName) {
			return "--";
		}
		return $this->playerName;
	}

	public function setPlayerName($playerName) {
		$this->playerName = $playerName;
	}

	public function getGamesPlayed() {
		if (!$this->gamesPlayed) {
			return "--";
		}
		return $this->gamesPlayed;
	}

	public function setGamesPlayed($gamesPlayed) {
		$this->gamesPlayed = $gamesPlayed;
	}

	public function getFieldGoalPercentage() {
		if (!$this->fieldGoalPercentage) {
			return "--";
		}
		return $this->fieldGoalPercentage;
	}

	public function setFieldGoalPercentage($newPercentage) {
		$this->fieldGoalPercentage = $newPercentage;
	}

	public function getThreePointPercentage() {
		if (!$this->threePointPercentage) {
			return "--";
		}
		return $this->threePointPercentage;
	}

	public function setThreePointPercentage($newPercentage) {
		$this->threePointPercentage = $newPercentage;
	}

	public function getFreeThrowPercentage() {
		if (!$this->freeThrowPercentage) {
			return "--";
		}
		return $this->freeThrowPercentage;
	}

	public function setFreeThrowPercentage($newPercentage) {
		$this->freeThrowPercentage = $newPercentage;
	}

	public function getPointsPerGame() {
		if (!$this->pointsPerGame) {
			return "--";
		}
		return $this->pointsPerGame;
	}

	public function setPointsPerGame($pointsPerGame) {
		$this->pointsPerGame = $pointsPerGame;
	}
}