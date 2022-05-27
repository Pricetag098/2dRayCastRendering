#include "Game.h"
#include <fstream>
#include <list>
#include <iostream>
#include <vector>
#include <cmath>
Game::Game()
{
}

Game::~Game()
{
}

void Game::Run()
{
	InitWindow(windowWidth, windowHeight, "Maze");
	SetTargetFPS(60);

	Load();

	while (!WindowShouldClose())
	{
		Update(GetFrameTime());
		Draw();
	}

	Unload();
}

void Game::Load() // load the stuff
{
	tex[0] = LoadImage("pics/eagle.png");
	tex[1] = LoadImage("pics/redbrick.png");
	tex[2] = LoadImage("pics/purplestone.png");
	tex[3] = LoadImage("pics/greystone.png");
	tex[4] = LoadImage("pics/bluestone.png");
	tex[5] = LoadImage("pics/mossy.png");
	tex[6] = LoadImage("pics/wood.png");
	tex[7] = LoadImage("pics/colorstone.png");

	// read from the level file
	std::ifstream file("Level.lvl", std::ios::binary);
	file.read((char*)&spawnPos, sizeof(Vector2));
	posX = (spawnPos.x + .5);
	posY = (spawnPos.y + .5);
	file.read((char*)&endPos, sizeof(Vector2));
	file.read((char*)&worldMap, sizeof(int) * mapWidth * mapHeight);
	file.close();
}

void Game::Unload()
{

}
void Game::Update(const float deltaTime)
{
	switch (gameState)
	{
	case 0:
		if (IsKeyDown(KEY_SPACE))
		{
			Load();
			gameState = 1;
		}
		break;

	default:
		double ms = deltaTime * movespeed;
		if (IsKeyDown(KEY_W))// walk forward checking for collision
		{
			if (worldMap[int(posX + dirX * ms)][int(posY)] == false) { posX += dirX * ms; }
			if (worldMap[int(posX)][int(posY + dirY * ms)] == false) { posY += dirY * ms; }
		}
		if (IsKeyDown(KEY_S)) // walk back checking for collision
		{
			if (worldMap[int(posX - dirX * ms)][int(posY)] == false) { posX -= dirX * ms; }
			if (worldMap[int(posX)][int(posY - dirY * ms)] == false) { posY -= dirY * ms; }
		}
		double rs = deltaTime * rotationSpeed;
		if (IsKeyDown(KEY_A)) // rotate the plane and direction at the same time
		{

			double oldDirX = dirX;
			dirX = dirX * cos(rs) - dirY * sin(rs);
			dirY = oldDirX * sin(rs) + dirY * cos(rs);

			double oldPlaneX = planeX;
			planeX = planeX * cos(rs) - planeY * sin(rs);
			planeY = oldPlaneX * sin(rs) + planeY * cos(rs);
		}
		if (IsKeyDown(KEY_D)) // same
		{

			double oldDirX = dirX;
			dirX = dirX * cos(-rs) - dirY * sin(-rs);
			dirY = oldDirX * sin(-rs) + dirY * cos(-rs);

			double oldPlaneX = planeX;
			planeX = planeX * cos(-rs) - planeY * sin(-rs);
			planeY = oldPlaneX * sin(-rs) + planeY * cos(-rs);
		}

		//check to end game
		if (int(posX) == endPos.x && int(posY) == endPos.y)
		{
			gameState = 0;
		}
		break;
	}

	
}


//raycasting tutorial from https://lodev.org/cgtutor/raycasting.html
void Game::Draw()
{

	BeginDrawing();
	switch (gameState)
	{
	case 0:
		ClearBackground(BLACK);
		DrawText("Maze", 150, 10, 200, RAYWHITE);
		DrawText("Press Space To Start", 100, 400, 50, RAYWHITE);
		break;
	default:
		ClearBackground(BLACK);
		int w = windowWidth;//no clue what this is
		for (int x = 0; x < w; x++)
		{

			//prepare variables for raycast;
			double camX = 2 * x / (double)w - 1;
			double rayDirX = dirX + planeX * camX;
			double rayDirY = dirY + planeY * camX;

			int mapX = int(posX);
			int mapY = int(posY);

			double sideDistX, sideDistY;


			double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX); // prevent divide by zero
			double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
			double perpWallDist; // distance to the hit wall

			int stepX, stepY;

			bool hit = false;
			int side;

			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1 - posX) * deltaDistX;
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1 - posY) * deltaDistY;
			}

			//raycast

			while (!hit)
			{
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				if (worldMap[mapX][mapY])
				{
					hit = true;
				}
			}
			if (side == 0) { perpWallDist = (sideDistX - deltaDistX); }
			else { perpWallDist = (sideDistY - deltaDistY); }
			int lineHeight = (int)(windowHeight / perpWallDist);
			int start = -lineHeight / 2 + windowHeight / 2;
			if (start < 0) { start = 0; }
			int end = lineHeight / 2 + windowHeight / 2;
			if (end >= windowHeight) { end = windowHeight - 1; }

			Color col;
			int texNum = worldMap[mapX][mapY] - 1;


			//calculate position on the wall
			double wallX;
			if (side == 0) { wallX = posY + perpWallDist * rayDirY; }
			else { wallX = posX + perpWallDist * rayDirX; }
			wallX -= floor((wallX));

			//calculate the equivilent position on the texture
			int texX = int(wallX * double(tex[texNum].width));
			if (side == 0 && rayDirX > 0) texX = tex[texNum].width - texX - 1;
			if (side == 1 && rayDirX < 0) texX = tex[texNum].width - texX - 1;


			//set how much to move next check
			double step = 1.0 * tex[texNum].height / lineHeight;
			double texPos = (start - windowHeight / 2 + lineHeight / 2) * step;

			for (int y = start; y < end; y++)
			{
				int texY = (int)texPos & (tex[texNum].height - 1);
				texPos += step;

				col = GetImageColor(tex[texNum], texX, texY);
				if (side == 1)
				{
					col.r = col.r / 2;
					col.g = col.g / 2;
					col.b = col.b / 2;
				}

				//fog time
				double density = 0.5;
				double fogVal = 1 / std::exp(std::pow(perpWallDist * density, 2));
				col.r = col.r * fogVal;
				col.g = col.g * fogVal;
				col.b = col.b * fogVal;

				DrawPixel(x, y, col);
			}

			
		}



		break;
	}
	
	EndDrawing();
}

void Game::End()
{
	
}
