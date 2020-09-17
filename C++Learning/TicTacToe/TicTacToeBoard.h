#pragma once
#include "GameBoard.h"

enum GamePiece : char { BLANK = '-', Px = 'X', Po = 'O' };

class TTTBoard : public GameBoard<GamePiece>
{
public:

	TTTBoard() = delete;

	TTTBoard(size_t numRow, size_t numCol, GamePiece defaultPiece)
		: GameBoard(numRow, numCol, defaultPiece)
	{
	}

	bool isFull() const
	{
		return GameBoard::isFull(defaultValue);
	}

	bool isValidCoords(Coords input) const override
	{
		bool isValid = GameBoard::isValidCoords(input);

		if (isValid)
		{
			isValid = (getAt(input.row, input.col) == defaultValue);
		}

		return isValid;
	}
};

