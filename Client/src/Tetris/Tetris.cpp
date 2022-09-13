#include "Tetris.h"

Tetris::Tetris()
	:
	_map(),
	_rng(std::random_device()()),
	_currentTetrimino(),
	_TetriminoPosition(),
	_ghostTetriminoPosition(),
	_cellContents(_map.width()* _map.height(), Granite::Color::Black()),
	_shouldDeleteRow(_map.height(), false)
{ }

#pragma region FLOW CONTROL

void Tetris::Start()
{
	// Setup 16x25 play area with cells that are 20 pixels wide and padded by 1 pixel. Don't apply a default border and
	// offset the map within the window by 70 pixels left and 50 pixels down
	_map.SetWidth(16);
	_map.SetHeight(25);
	_map.SetBorderProperties(0, 0);
	_map.SetCellProperties(20, 1);
	_map.SetOffset_x(70);
	_map.SetOffset_y(50);

	// Start the game by spawing a new Tetrimino
	SpawnNewTetrimino();
}

void Tetris::ResetGame()
{
	// Reset count variables
	_rotationCount = 0.0f;
	_moveCount = 0.0f;
	_rowDeleteCount = 0.0f;
	_dropCount = 0.0f;

	// Clear all map tiles of any contents
	_cellContents.clear();

	// Re-initialize vetor arrays to default values
	_cellContents = std::vector<Granite::Color>(_map.width() * _map.height(), Granite::Color::Black());
	_shouldDeleteRow = std::vector<bool>(_map.height(), false);

	// Make sure gameOver boolean isn't set!
	_gameOver = false;
	_gameOverDrawCounts = 0;

	// Spawn a new Tetrimino
	SpawnNewTetrimino();
}

#pragma endregion

#pragma region LOGIC

void Tetris::SpawnNewTetrimino()
{
	// Pick a random Tetrimino and spawn it at the top of the screen
	_currentTetrimino.Spawn_Random(_rng);
	//_currentTetrimino.Spawn_I(0);
	_TetriminoPosition = { 6, 1 };
	_ghostTetriminoPosition = { 6, 1 };
	_dropWait = _defaultDropWait;
	_currentRotation = 0.0f;

	_updateGhost = true;
}

void Tetris::Input()
{
	if (!_gameOver)
	{
		// If up arrow or W is pressed, rotate Tetrimino 90 degrees
		if (_window.keyboard.KeyIsPressed(VK_UP) || _window.keyboard.KeyIsPressed('W'))
			_rotationIncrement = 90.0f;

		// Update delta movement based on left or right key input
		if (_window.keyboard.KeyIsPressed(VK_LEFT) || _window.keyboard.KeyIsPressed('A')) _delta = { -1, 0 };
		if (_window.keyboard.KeyIsPressed(VK_RIGHT) || _window.keyboard.KeyIsPressed('D')) _delta = { 1, 0 };

		// If the down arrow or S key is pressed, speed up the Tetrimino drop time by a factor of 10
		if (_window.keyboard.KeyIsPressed(VK_DOWN) || _window.keyboard.KeyIsPressed('S')) _dropWait = _defaultDropWait / 10;

		if (_pauseCount >= _pauseWait)
		{
			// Pause is toggled with Escape key
			if (_window.keyboard.KeyIsPressed(VK_ESCAPE))
				_paused = !_paused;
		}
	}
	else
	{
		if (_window.keyboard.KeyIsPressed(VK_RETURN))
			ResetGame();
		else
			return;
	}
}

void Tetris::Update()
{
	// If game is over, allow user to reset by pressing Return. Otherwise, skip updating game
	// logic when game is over
	if (_gameOver)
	{
		return;
	}

	// If enough frames have passed to allow determination of game pause update...
	if (_pauseCount >= _pauseWait)
	{
		// Reset frame timer for pause update
		_pauseCount = 0.0f;
	}
	else
	{
		// Increment frame timer for pause update
		_pauseCount += _deltaTime;
	}

	// If paused, skip updating game logic
	if (_paused)
		return;

	// For the current Tetrimino, process rotation, lateral, and vertical movement. Also
	// check whether rows are filled and need to be deleted.
	ProcessRotation();
	ProcessLateralMovement();
	ProcessVerticalMovement();
	MarkRowsForDeletion();

	// Reset delta movement in case there are no keypresses next frame
	_delta = { 0, 0 };
}

void Tetris::ProcessRotation()
{
	// If enough frames have passed to allow for rotation update...
	if (_rotationCount >= _rotationWait)
	{
		// Increase current rotation, ensuring not to exceed 270 degrees
		if (_currentRotation == 270 && _rotationIncrement == 90)
			_currentRotation = 0;
		else
			_currentRotation += _rotationIncrement;

		// Apply the rotation to the current Tetrimino
		_currentTetrimino.Rotate(_currentRotation);

		// Rest the frame timer for the next rotation update
		_rotationCount = 0.0f;
	}
	else
	{
		// Increment rotation frame count
		_rotationCount += _deltaTime;
	}

	// Reset rotation increment in case up or W is not pressed next frame
	_rotationIncrement = 0;
}

void Tetris::ProcessLateralMovement()
{
	// If enough frames have passed to allow for lateral movement update...
	if (_moveCount >= _moveWait)
	{
		// Get the minimum tile x position in the current Tetrimino, add the TetriminoPosition and the delta offset that the Tetrimino intends to move
		// to...make sure it is >= 1 (i.e., to the right of the left play area border). 
		// 
		// Also make sure that the maximum tile x position in the current Tetrimino added to the TetriminoPosition offset and the intended delta position
		// is less than mapwidth - 2 (i.e., is to the left of the right play area border). It's "-2" because the tiles along a row go from 0 to 
		// _map.width() - 1...but the tile at _map.width() - 1 is a border tile so we need _map.width() - 2!
		//
		// Finally, also make sure that the position we are moving to actually contains empty tiles...that's what the CheckDirection() function does.
		//
		// If all three of these conditions are satisfied, we can go ahead and apply the intended delta movement to the Tetrimino.
		if (_TetriminoPosition.x + _currentTetrimino.xmin() + _delta.x >= 1 && _TetriminoPosition.x + _currentTetrimino.xmax() + _delta.x <= _map.width() - 2 && CheckDirection(_delta))
		{
			// Add the lateral offset to both the Tetrimino and the ghosted Tetrimino that indicates where it will land
			_TetriminoPosition.Add(_delta);
			_ghostTetriminoPosition.Add(_delta);
		}

		// Reset frame counter for the next lateral movement update
		_moveCount = 0.0f;
	}
	else
	{
		// Increment lateral movement frame counter
		_moveCount += _deltaTime;
	}
}

void Tetris::ProcessVerticalMovement()
{
	int down = _map.height() - 2;
	for (int i = 0; i < 4; i++)
	{
		Coordinates c = _currentTetrimino.GetCoordinate(i);
		c.x += _TetriminoPosition.x;
		c.y += GetLowestEmptyY(c.x) - _currentTetrimino.ymax();

		if (c.y < down) down = c.y;
	}

	_ghostTetriminoPosition.y = down;

	// If enough frames have passed to allow for vertical movement update...
	if (_dropCount >= _dropWait)
	{
		// If the vertical position of the Tetrimino hasn't yet hit the bottom border and the tiles below
		// the Tetrimino are empty...
		if (_TetriminoPosition.y + _currentTetrimino.ymax() + 1 <= _map.height() - 2 && CheckDirection({ 0,1 }))
		{
			// Drop the Tetrimino down by one tile
			_TetriminoPosition.Add({ 0,1 });
			_updateGhost = true;
		}
		else
		{
			// If we have either reached the bottom of the play area or the tiles below the Tetrimino are 
			// not empty, we need to lock this Tetrimino to the play map area and spawn a new Tetrimino
			LockTetrimino();

			if (!_gameOver)
				SpawnNewTetrimino();

			_updateGhost = false;
		}

		// Reset frame counter for the next vertical movement update
		_dropCount = 0.0f;
	}
	else
	{
		// Increment vertical movement frame counter
		_dropCount += _deltaTime;
	}
}

int Tetris::GetLowestEmptyY(int x)
{
	for (int y = _map.height() - 2; y >= 1; y--)
	{
		if (IsCellEmpty({ x, y }))
			return y;
	}

	return 1;
}

bool Tetris::CheckDirection(const Coordinates& direction)
{
	bool allowMove = true;
	for (int i = 0; i < 4; i++)
	{
		Coordinates nextCoords = ADD(_currentTetrimino.GetCoordinate(i), direction);
		nextCoords = ADD(nextCoords, _TetriminoPosition);
		allowMove = allowMove && IsCellEmpty(nextCoords);
	}

	return allowMove;
}

bool Tetris::CheckGhostDirection(const Coordinates& direction)
{
	bool allowMove = true;
	for (int i = 0; i < 4; i++)
	{
		Coordinates nextCoords;
		nextCoords.x = _TetriminoPosition.x + direction.x;
		nextCoords.y = direction.y;

		nextCoords = ADD(nextCoords, _ghostTetriminoPosition);
		allowMove = allowMove && IsCellEmpty(nextCoords);
	}

	return allowMove;
}

void Tetris::LockTetrimino()
{
	for (int i = 0; i < 4; i++)
	{
		Coordinates lockCoordinate = ADD(_TetriminoPosition, _currentTetrimino.GetCoordinate(i));
		SetCellContents(lockCoordinate, _currentTetrimino.Color());

		if (lockCoordinate.y == 1)
		{
			_gameOver = true;
		}
	}
}

void Tetris::SetCellContents(const Coordinates& location, Granite::Color color)
{
	_cellContents[location.y * _map.width() + location.x] = color;
}

Granite::Color Tetris::GetCellContents(const Coordinates& location) const
{
	int index = location.y * _map.width() + location.x;
	if (index < _cellContents.size())
		return _cellContents[index];
	else
		return Granite::Color::Black();
}

bool Tetris::IsCellEmpty(const Coordinates& location)
{
	return GetCellContents(location).IsEqual(Granite::Color::Black());
}

void Tetris::MarkRowsForDeletion()
{
	for (int j = _map.height() - 2; j > 0; j--)
	{
		_shouldDeleteRow[j] = false;

		bool deleteRow = true;
		for (int i = 1; i < _map.width() - 1; i++)
		{
			deleteRow = deleteRow && !IsCellEmpty({ i,j });
		}

		if (deleteRow)
		{
			for (int i = 1; i < _map.width() - 1; i++)
			{
				_cellContents[j * _map.width() + i] = _deletionRowColor;
				_shouldDeleteRow[j] = true;
			}
		}
	}

	if (_rowDeleteCount > _rowDeleteWait)
	{
		DeleteRows();
		_rowDeleteCount = 0.0f;
	}
	else
	{
		_rowDeleteCount += _deltaTime;
	}
}

void Tetris::DeleteRows()
{
	for (int j = _map.height() - 2; j > 0; j--)
	{
		if (_shouldDeleteRow[j])
		{
			_shouldDeleteRow[j] = false;
			for (int jj = j; jj > 1; jj--)
			{
				for (int i = 1; i < _map.width() - 1; i++)
					_cellContents[(jj)*_map.width() + i] = _cellContents[(jj - 1) * _map.width() + i];
			}
		}
	}
}

#pragma endregion

#pragma region GRAPHICS

void Tetris::Render()
{
	if (_paused)
		return;

	if (_gameOver && _gameOverDrawCounts > 0)
	{
		return;
	}

	// Clear the screen each frame
	_frameBuffer.Clear(Granite::Color::Granite());

	// Draw locked Tetriminos
	DrawLockedTetriminos();

	// Draw the Tetrimino
	_currentTetrimino.Draw(_map, _TetriminoPosition, _gameOver ? Granite::Color::Red() : Granite::Color::Black());

	// Draw the Tetrimino ghost
	if (_updateGhost)
	{
		//_currentTetrimino.DrawGhost(_map, 1, _ghostTetriminoPosition);

		int xmin = (_TetriminoPosition.x + _currentTetrimino.xmin()) * _map.cellsize() + _map.offsetx();
		int xmax = (_TetriminoPosition.x + _currentTetrimino.xmax() + 1) * _map.cellsize() + _map.offsetx();
		int ymin = (_TetriminoPosition.y + _currentTetrimino.ymax() + 1) * _map.cellsize() + _map.offsety();
		int thickness = 1;

		int height = (_map.height() - 2 - (_TetriminoPosition.y + _currentTetrimino.ymax() + 1)) * _map.cellsize();
		Granite::Color color = _currentTetrimino.Color();

		int dashLength = 3;
		int gapLength = 10;
		bool draw = true;
		int totalLength = 0;
		while (totalLength <= height)
		{
			if (draw)
			{
				_frameBuffer.DrawRect(xmin, ymin + totalLength, thickness, dashLength, color);
				_frameBuffer.DrawRect(xmax, ymin + totalLength, thickness, dashLength, color);
				totalLength += dashLength;
			}
			else
			{
				totalLength += gapLength;
			}

			draw = !draw;
		}
	}

	// Draw border
	DrawPlayAreaBorder();

	if (_rowDeleteCount <= _rowDeleteWait)
	{
		float lerp = 0;

		if (lerp <= 0.05f)
			lerp = (_rowDeleteLerpSpeed * 255.0f / (_rowDeleteWait)*_rowDeleteCount);

		if (lerp >= 254.95f)
			lerp = 255 - (_rowDeleteLerpSpeed * 255.0f / (_rowDeleteWait)*_rowDeleteCount);

		_deletionRowColor = Granite::Color(255u, lerp, lerp);
	}

	if (_gameOver)
		_gameOverDrawCounts++;
}

void Tetris::DrawPlayAreaBorder()
{
	for (int i = 0; i < _map.width(); i++)
	{
		_map.DrawCell({ i, 0 }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
		_map.DrawCell({ i, _map.height() - 1 }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
	}

	for (int j = 0; j < _map.height(); j++)
	{
		_map.DrawCell({ 0, j }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
		_map.DrawCell({ _map.width() - 1, j }, _gameOver ? Granite::Color(80u, 0u, 0u) : Granite::Color::MediumGray());
	}
}

void Tetris::DrawLockedTetriminos()
{
	for (int i = 0; i < _map.width(); i++)
	{
		for (int j = 0; j < _map.height(); j++)
		{
			if (IsCellEmpty({ i,j }))
				continue;

			_map.DrawCell({ i,j }, _gameOver ? Granite::Color::Red() : _cellContents[j * _map.width() + i]);
		}
	}
}

#pragma endregion