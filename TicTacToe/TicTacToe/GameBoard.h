#pragma once
#include "TTTDataTypes.h"


class GameBoard
{
private:

	int** board;
	const int rows;
	const int cols;
	const int defaultValue;

public:
	GameBoard(int numRows, int numCols, GamePiece newDefault);
	int** returnBoard() const;
	int GetNumRow() const;
	int GetNumCol() const;
	bool isValidCoords(Coords) const;
	bool isFull() const;
	void clearBoard();
	void placePiece(Coords, GamePiece);
	~GameBoard();
};
