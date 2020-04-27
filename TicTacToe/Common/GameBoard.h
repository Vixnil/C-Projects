#pragma once

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

template <typename T>
class GameBoard
{
public:

	GameBoard(char numRows, char numCols, T newDefault) :
		rows(numRows), cols(numCols), defaultValue(newDefault)
	{
		board = new T * [rows];

		for (int rowIndex = 0; rowIndex < rows; rowIndex++)
		{
			board[rowIndex] = new T[cols];
		}

		clearBoard();
	}

	void placePiece(Coords input, T piece)
	{
		board[input.row][input.col] = piece;
	}

	virtual bool isValidCoords(Coords input) const
	{
		bool isValid = (-1 < input.row && input.row < rows);
		isValid = (isValid && (-1 < input.col && input.col < cols));

		return isValid;
	}

	bool isFull(T piece) const
	{
		for (int ri = 0; ri < rows; ri++)
		{
			for (int ci = 0; ci < cols; ci++)
			{
				if (board[ri][ci] == piece)
				{
					return false;
				}
			}
		}

		return true;
	}

	void clearBoard()
	{
		for (int rowIndex = 0; rowIndex < rows; rowIndex++)
		{
			for (int colIndex = 0; colIndex < cols; colIndex++)
			{
				board[rowIndex][colIndex] = defaultValue;
			}
		}
	}

	int GetNumRow() const
	{
		return rows;
	}

	int GetNumCol() const
	{
		return cols;
	}

	T** returnBoard() const
	{
		return board;
	}

	~GameBoard()
	{
		for (int rowIndex = 0; rowIndex < rows; rowIndex++)
		{
			delete[] board[rowIndex];
		}

		delete[] board;
	}

protected:

	T** board;
	const int rows;
	const int cols;
	const T defaultValue;

};
