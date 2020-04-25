#include "GameBoard.h"

GameBoard::GameBoard(int numRows, int numCols, GamePiece newDefault) :
	rows(numRows), cols(numCols), defaultValue(newDefault)
{
	board = new char*[rows];

	for (int rowIndex = 0; rowIndex < rows; rowIndex++)
	{
		board[rowIndex] = new char[cols];
	}

	clearBoard();
}

void GameBoard::placePiece(Coords input, GamePiece piece)
{
	board[input.row][input.col] = piece;
}

bool GameBoard::isValidCoords(Coords input) const
{
	bool isValid = (-1 < input.row && input.row < rows);
	isValid = (isValid && (-1 < input.col && input.col < cols));

	if (isValid)
	{
		isValid = board[input.row][input.col] == BLANK;
	}

	return isValid;
}

bool GameBoard::isFull() const
{
	for (int ri = 0; ri < rows; ri++)
	{
		for (int ci = 0; ci < cols; ci++)
		{
			if (board[ri][ci] == BLANK)
			{
				return false;
			}
		}
	}

	return true;
}

void GameBoard::clearBoard()
{
	for (int rowIndex = 0; rowIndex < rows; rowIndex++)
	{
		for (int colIndex = 0; colIndex < cols; colIndex++)
		{
			board[rowIndex][colIndex] = defaultValue;
		}
	}
}

int GameBoard::GetNumRow() const
{
	return rows;
}

int GameBoard::GetNumCol() const
{
	return cols;
}

char** GameBoard::returnBoard() const
{
	return board;
}

GameBoard::~GameBoard()
{
	for (int rowIndex = 0; rowIndex < rows; rowIndex++)
	{
		delete[] board[rowIndex];
	}

	delete[] board;
}
