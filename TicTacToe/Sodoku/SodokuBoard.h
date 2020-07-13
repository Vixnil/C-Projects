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
	typedef GamePiece(*GetAtCheck)(char, char);

	SodokuBoard();

	GamePiece getValue(char, char);
	bool isInRange(char, char);

	template<GetAtCheck get>
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

	bool isValidRow(char rowNum) const;
	bool isValidCol(int colNum) const;
	bool isValidSquare() const;

};
