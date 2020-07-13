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
		board = new T[rows * cols];

		clearBoard();
	}

	void placePiece(Coords input, T piece)
	{
		board[(input.row * rows) + input.col] = piece;
	}

	bool isInRowRange(char rowNum) const
	{
		return (-1 < rowNum && rowNum < rows);
	}

	bool isInColRange(char colNum) const
	{
		return (-1 < colNum && colNum < cols);
	}

	virtual bool isValidCoords(Coords input) const
	{
		return (isInRowRange(input.row) && isInColRange(input.col));;
	}

	bool isFull(T piece) const
	{
		for (int ri = 0; ri < rows; ri++)
		{
			for (int ci = 0; ci < cols; ci++)
			{
				if (getAt(ri, ci) == piece)
				{
					return false;
				}
			}
		}

		return true;
	}

	void clearBoard()
	{
		Coords loc;

		for (int rowIndex = 0; rowIndex < rows; rowIndex++)
		{
			loc.row = rowIndex;

			for (int colIndex = 0; colIndex < cols; colIndex++)
			{
				loc.col = colIndex; 
				placePiece(loc, defaultValue);
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

	T getAt(int row, int col) const
	{
		return board[(row * rows) + col];
	}

	~GameBoard()
	{
		delete[] board;
	}

protected:

	T* board;
	const int rows;
	const int cols;
	const T defaultValue;

};
