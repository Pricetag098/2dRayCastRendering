#pragma once
#include "raylib.h"
#include <iostream>
class Game
{
public:
	Game();
	~Game();
	void Run();
	void Load();
	void Unload();
	void Update(const float deltaTime);
	void Draw();
	void End();
	const static int windowWidth = 800, windowHeight = 800;
	const static int mapWidth = 24, mapHeight = 24;
	float boxSize = windowHeight / mapHeight;
	int map[mapHeight][mapWidth] = {};
	Texture2D tex[9];
	int paint = 0;
	
	Vector2 spawnLocation = { 0,0 };
	Vector2 endLocation = { 0,0 };
};

