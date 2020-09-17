#pragma once
#include <memory>

struct Coords
{
	size_t row = -1;
	size_t col = -1;

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

	GameBoard(size_t numRows, size_t numCols, T newDefault) :
		rows(numRows), cols(numCols), defaultValue(newDefault)
	{
		board = std::make_unique<T[]>(rows * cols);
		clearBoard();
	}

	void placePiece(Coords input, T piece)
	{
		board[(input.row * rows) + input.col] = piece;
	}

	bool isInRowRange(size_t rowNum) const
	{
		return (0 <= rowNum && rowNum < rows);
	}

	bool isInColRange(size_t colNum) const
	{
		return (0 <= colNum && colNum < cols);
	}

	virtual bool isValidCoords(Coords input) const
	{
		return (isInRowRange(input.row) && isInColRange(input.col));
	}

	bool isFull(T piece) const
	{
		for (size_t ri = 0; ri < rows; ri++)
		{
			for (size_t ci = 0; ci < cols; ci++)
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

		for (size_t rowIndex = 0; rowIndex < rows; rowIndex++)
		{
			loc.row = rowIndex;

			for (size_t colIndex = 0; colIndex < cols; colIndex++)
			{
				loc.col = colIndex; 
				placePiece(loc, defaultValue);
			}
		}
	}

	size_t GetNumRow() const
	{
		return rows;
	}

	size_t GetNumCol() const
	{
		return cols;
	}

	T getAt(size_t row, size_t col) const
	{
		return board[(row * rows) + col];
	}

	~GameBoard()
	{
		//delete[] board;
	}

protected:

	std::unique_ptr<T[]> board;
	const size_t rows;
	const size_t cols;
	const T defaultValue;

};
