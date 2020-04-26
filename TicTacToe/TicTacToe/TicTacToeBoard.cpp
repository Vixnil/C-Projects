#include "TicTacToeBoard.h"

TTTBoard::TTTBoard(char numRow, char numCol, GamePiece defaultPiece)
	: GameBoard(numRow, numCol, defaultPiece)
{
}

bool TTTBoard::isValidCoords(Coords input) const
{
	bool isValid = GameBoard::isValidCoords(input);

	if (isValid)
	{
		isValid = board[input.row][input.col] == BLANK;
	}

	return isValid;
}

bool TTTBoard::isFull() const
{
	return GameBoard::isFull(BLANK);
}
