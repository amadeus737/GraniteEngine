#pragma once

#include <GraniteEngine.h>
#include <random>
#include "Tetrimino.h"
#include <stdio.h>

class Tetris : GraniteGame
{
public:
	// Constructor
	Tetris();

	// Flow control
	void Start();
	void ResetGame();
	void Input();
	void Update();
	void Render();

	// Game logic
	void SpawnNewTetrimino();
	void ProcessRotation();
	void ProcessLateralMovement();
	void ProcessVerticalMovement();
	int GetLowestEmptyY(int x);
	bool CheckDirection(const Coordinates& direction);
	bool CheckGhostDirection(const Coordinates& direction);
	void LockTetrimino();
	void SetCellContents(const Coordinates& location, Granite::Color color);
	Granite::Color GetCellContents(const Coordinates& location) const;
	bool IsCellEmpty(const Coordinates& location);
	void MarkRowsForDeletion();
	void DeleteRows();

	// Graphics
	void DrawPlayAreaBorder();
	void DrawLockedTetriminos();

private:
	bool _gameOver = false;
	bool _wantsToPause = false;
	bool _paused = false;
	bool _updateGhost = true;
	Map _map;
	std::mt19937 _rng;
	Tetrimino _currentTetrimino;
	Coordinates _TetriminoPosition;
	Coordinates _ghostTetriminoPosition;
	std::vector<Granite::Color> _cellContents;
	std::vector<bool> _shouldDeleteRow;
	Coordinates _delta = { 0,0 };
	Granite::Color _deletionRowColor = Granite::Color::White();
	int _currentRotation = 0;
	int _rotationIncrement = 0;
	int _gameOverDrawCounts = 0;
	float _rotationCount = 0.0f;
	float _moveCount = 0.0f;
	float _rowDeleteCount = 0.0f;
	float _colorChangeCount = 0.0f;
	float _dropCount = 0.0f;
	float _pauseCount = 0.0f;
	static constexpr float _rotationWait = 0.1f;        // 20
	static constexpr float _moveWait = 0.1f;	         // 20
	float _pauseWait = 0.1f;                            // 25
	static constexpr float _rowDeleteWait = 1.0f;       // 200
	static constexpr float _rowDeleteLerpSpeed = 40.0f;   // 5
	float _colorChangeTime = 0.1f;                       // 33
	float _dropWait = 0.7f;	                             // 70
	float _defaultDropWait = 0.7f;                       // 70	
};
