#pragma once

class GameBoard
{
public:
	enum GamePiece : char { BLANK = '-', Px = 'X', Po = 'O' };

	struct Coords
	{
		int row = -1;
		int col = -1;

		bool operator==(Coords other)
		{
			return (this->row == other.row) && (this->col == other.col);
		}

		bool operator!=(Coords other)
		{
			return !(*this == other);
		}
	};

	GameBoard(int numRows, int numCols, GamePiece newDefault);
	GamePiece** returnBoard() const;
	int GetNumRow() const;
	int GetNumCol() const;
    bool isValidCoords(Coords) const;
	bool isFull() const;
	void clearBoard();
	void placePiece(Coords, GamePiece);
	~GameBoard();

protected:

	GamePiece** board;
	const int rows;
	const int cols;
	const GamePiece defaultValue;
};
