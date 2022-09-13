#include "Snek.h"

Snek::Snek()
	:
	_map(),
	_rng(std::random_device()()),
	_snake({ 2,2 }),
	_food(_rng, _map, _snake)
{ }

void Snek::Start()
{
	_map.SetWidth(25);
	_map.SetHeight(25);
	_map.SetOffset_x(70);
	_map.SetOffset_y(50);
	_map.SetBorderProperties(4, 2);
	_map.SetCellProperties(20, 1);
}

void Snek::RestartGame()
{
	_gameOver = false;
	_gameOverDrawCounts = 0;
	_snake.Reset({ 2, 2 });
	_delta = { 1, 0 };
}

void Snek::Input()
{
	if (!_gameOver)
	{
		if (_window.keyboard.KeyIsPressed(VK_UP) || _window.keyboard.KeyIsPressed('W')) _delta = { 0, -1 };
		if (_window.keyboard.KeyIsPressed(VK_LEFT) || _window.keyboard.KeyIsPressed('A')) _delta = { -1, 0 };
		if (_window.keyboard.KeyIsPressed(VK_DOWN) || _window.keyboard.KeyIsPressed('S')) _delta = { 0, 1 };
		if (_window.keyboard.KeyIsPressed(VK_RIGHT) || _window.keyboard.KeyIsPressed('D')) _delta = { 1, 0 };
	}
	else
	{
		if (_window.keyboard.KeyIsPressed(VK_RETURN))
			RestartGame();
	}
}

void Snek::Update()
{
	if (!_gameOver)
	{
		_moveCounter += _deltaTime;
		if (_moveCounter >= _movePeriod)
		{
			_moveCounter -= _movePeriod;
			const Coordinates next = _snake.GetNextHeadLocation(_delta);
			_gameOver = !_map.InMap(next) || _snake.InTileExcludeEnd(next);

			if (!_gameOver)
			{
				const bool eating = next == _food.coords();
				if (eating || _window.keyboard.KeyIsPressed(VK_CONTROL))
				{
					_snake.Grow();
				}

				_snake.Move(_delta);

				if (eating)
				{
					_food.Respawn(_rng, _map, _snake);
				}
			}
		}
	}
}

void Snek::Render()
{
	if (!_gameOver)
	{
		_frameBuffer.Clear(Granite::Color(0x33, 0x33, 0x33));
		_snake.Draw(_map);
		_food.Draw(_map);
		_map.DrawBorder();
	}
	else
	{
		if (_gameOverDrawCounts == 0)
			_frameBuffer.Clear(Granite::Color::Red());

		_gameOverDrawCounts++;
	}
}