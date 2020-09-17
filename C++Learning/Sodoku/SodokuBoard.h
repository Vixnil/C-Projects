#pragma once
#include "GameBoard.h"

enum GamePiece : char
{
	BLANK = ' ', 
	ONE = '1', TWO, THREE, FOUR, FIVE, SIZE, SEVEN, EIGHT, NINE
};

class SodokuBoard : public GameBoard<GamePiece>
{
public:

	SodokuBoard()
		:GameBoard(9, 9, BLANK)
	{
	}

	GamePiece getValue(size_t rowIndex, size_t colIndex)
	{
		return this->getAt(rowIndex, colIndex);
	}

	bool isInRange(size_t value, size_t maxSize)
	{
		return (-1 < value && value < maxSize);
	}

/*	template<GetAtCheck get>
	bool isValidLine(char lineNum, char maxSize, char otherLineMax)
	{
		bool isValid = isInRange(lineNum, maxSize);

		if (isValid)
		{
			char counters[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			for (int oLIndex = 0; oLIndex < otherLineMax; oLIndex++)
			{
				char counterIndex = get(lineNum, oLIndex) - '1';

				if (-1 < counterIndex && counterIndex < otherLineMax)
				{
					counters[counterIndex]++;

					if (counters[counterIndex] > 1)
					{
						isValid = false;
						return;
					}
				}
			}
		}

		return isValid;
	}
	*/

	bool isValidRow(size_t rowNum) const
	{
		bool isValid = isInRowRange(rowNum);

		if (isValid)
		{
			char counters[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			for (size_t oLIndex = 0; oLIndex < cols; oLIndex++)
			{
				size_t counterIndex =  - '1';

				if (-1 < counterIndex && counterIndex < cols)
				{
					counters[counterIndex]++;

					if (counters[counterIndex] > 1)
					{
						isValid = false;
						return;
					}
				}
			}
		}

		return isValid;
	}

	bool isValidCol(size_t colNum) const
	{
		return false;
	}

	bool isValidSquare() const
	{
		return false;
	}

};
