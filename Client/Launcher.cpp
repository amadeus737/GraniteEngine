#include "Launcher.h"
#include "../Client/src/Tests/ColorDemo.h"
#include "../Client/src/Snek/Snek.h"
#include "../Client/src/Tetris/Tetris.h"
#include "../Client/src/3D_Renderer/RotatingWireframeCube.h"
#include "../Client/src/Stein/Stein.h"

Granite::Game* Granite::LaunchGame()
{
	// Run the game
	Run(Stein);
}
