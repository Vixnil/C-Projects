#pragma once
#include "GameBoard.h"

enum GamePiece : char { BLANK = '-', Px = 'X', Po = 'O' };

template class GameBoard<GamePiece>;

class TTTBoard : public GameBoard<GamePiece>
{
public:

	TTTBoard() = delete;
	TTTBoard(char numRow, char numCol, GamePiece defaultPiece);

	bool isValidCoords(Coords) const override;
	bool isFull() const;
};

