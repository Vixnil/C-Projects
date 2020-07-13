#include "SodokuBoard.h"

SodokuBoard::SodokuBoard()
	:GameBoard(9, 9, BLANK)
{
}

bool SodokuBoard::isValidRow(char rowNum) const
{
	bool isValid = isValidLine<getValue>(rowNum, rows, cols);

	return isValid;
}

GamePiece SodokuBoard::getValue(char rowIndex, char colIndex)
{
	return this->getAt(rowIndex, colIndex);
}

bool SodokuBoard::isInRange(char value, char maxSize)
{
	return (-1 < value && value < maxSize);
}

bool SodokuBoard::isValidCol(int colNum) const
{

}

bool SodokuBoard::isValidSquare() const
{

}