#pragma once

enum GamePiece
{
	BLANK = 0, Px, Po
};

enum Difficulty
{
	Easy = 0, Medium, Impossible
};

struct Coords
{
	int row;
	int col;
};