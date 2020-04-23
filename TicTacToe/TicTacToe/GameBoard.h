#pragma once

class GameBoard
{
private:

	int** board;
	const int rows;
	const int cols;
	const int defaultValue;

public:
	enum GamePiece
	{
		BLANK = 0, Px, Po
	};

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
	int** returnBoard() const;
	int GetNumRow() const;
	int GetNumCol() const;
	bool isValidCoords(Coords) const;
	bool isFull() const;
	void clearBoard();
	void placePiece(Coords, GamePiece);
	~GameBoard();
};
