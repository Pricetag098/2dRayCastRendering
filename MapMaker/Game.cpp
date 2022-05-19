#include "Game.h"
#include <fstream>
Game::Game()
{
}

Game::~Game()
{
}


void Game::Run()
{
	InitWindow(windowWidth, windowHeight, "Map maker");
	SetTargetFPS(60);

	Load();

	while (!WindowShouldClose())
	{
		Update(GetFrameTime());
		Draw();
	}

	Unload();
}

void Game::Load()
{
	tex[0] = LoadTexture("pics/eagle.png");
	tex[1] = LoadTexture("pics/redbrick.png");
	tex[2] = LoadTexture("pics/purplestone.png");
	tex[3] = LoadTexture("pics/greystone.png");
	tex[4] = LoadTexture("pics/bluestone.png");
	tex[5] = LoadTexture("pics/mossy.png");
	tex[6] = LoadTexture("pics/wood.png");
	tex[7] = LoadTexture("pics/colorstone.png");
	tex[8] = LoadTexture("pics/barrel.png");
	tex[9] = LoadTexture("pics/pillar.png");
	std::ifstream file("../Level.lvl", std::ios::binary);
	file.read((char*)&spawnLocation, sizeof(Vector2));
	file.read((char*)&endLocation, sizeof(Vector2));
	file.read((char*)&map, sizeof(int) * mapWidth * mapHeight);
	file.close();

}

void Game::Unload()
{
	std::ofstream file("../Level.lvl", std::ios::binary);
	file.write((char*)&spawnLocation, sizeof(Vector2));
	file.write((char*)&endLocation, sizeof(Vector2));
	file.write((char*)*map, sizeof(int) * mapHeight * mapWidth);
	file.close();
}

void Game::Update(const float deltaTime)
{
	if (IsKeyPressed(KEY_ZERO))
	{
		paint = 0;
	}
	if (IsKeyPressed(KEY_ONE))
	{
		paint = 1;
	}
	if (IsKeyPressed(KEY_TWO))
	{
		paint = 2;
	}
	if (IsKeyPressed(KEY_THREE))
	{
		paint = 3;
	}
	if (IsKeyPressed(KEY_FOUR))
	{
		paint = 4;
	}
	if (IsKeyPressed(KEY_FIVE))
	{
		paint = 5;
	}
	if (IsKeyPressed(KEY_SIX))
	{
		paint = 6;
	}
	if (IsKeyPressed(KEY_SEVEN))
	{
		paint = 7;
	}
	if (IsKeyPressed(KEY_EIGHT))
	{
		paint = 8;
	}
	if (IsKeyPressed(KEY_NINE))
	{
		paint = 9;
	}
	if (IsKeyPressed(KEY_E))
	{
		paint =10;
	}
	if (IsMouseButtonDown(0))
	{
		Vector2 mousePos = GetMousePosition();
		if (mousePos.x > windowWidth || mousePos.y > windowHeight || mousePos.x < 0 || mousePos.y < 0)
		{
			return;
		}
		int x = int(mousePos.x / boxSize);
		int y = int(mousePos.y / boxSize);
		if (x > mapWidth || y > mapHeight)
		{
			return;
		}
		if (paint == 9)
		{
			
			Vector2 a = { x,y };
			spawnLocation = a;
			map[(int)spawnLocation.x][(int)spawnLocation.y] = 0;
			return;
		}
		if (paint == 10)
		{

			Vector2 a = { x ,y};
			endLocation = a;
			map[(int)endLocation.x][(int)endLocation.y] = 0;
			return;
		}
		map[x][y] = paint;
		
	}
	std::cout << spawnLocation.x << ", " << spawnLocation.y << "\n";
	std::cout << endLocation.x << ", " << endLocation.y << "\n";
}
void Game::Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);

	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			int index = y * mapHeight + x;


			float xPos = x * (boxSize);
			float yPos = y * (boxSize);
			
			if (map[x][y] <= 0)
			{
				DrawRectangle(xPos, yPos, boxSize, boxSize, BLACK);
			}

			
			else
			{
				
				Vector2 pos = { xPos,yPos };
				DrawTextureEx(tex[map[x][y] - 1], pos, 0, boxSize/ tex[map[x][y] - 1].width, WHITE);
				
			}
			//draw spawn Point;
			

		}
	}
	Vector2 pos = { (spawnLocation.x)*boxSize,(spawnLocation.y)*boxSize };
	DrawTextureEx(tex[8], pos, 0, boxSize / tex[8].width, WHITE);

	pos = { (endLocation.x) * boxSize,(endLocation.y) * boxSize };
	DrawTextureEx(tex[9], pos, 0, boxSize / tex[9].width, WHITE);


	EndDrawing();
}