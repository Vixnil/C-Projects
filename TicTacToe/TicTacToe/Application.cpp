#include <iostream>
#include "TicTacToeBoard.h"
#include "Instrumentor.h"

typedef void(*AiDifficulty)(TTTBoard&, GamePiece&);
typedef void(*NumMod)(int&);
typedef Coords(*BoardOrientation)(TTTBoard&, int, int, GamePiece);

Coords promptUserForCoord(TTTBoard&);
Coords getWinningMove(TTTBoard& gb, GamePiece playerPiece, int acceptableBlanks);
template<NumMod mod>
Coords getDiagMove(TTTBoard& gb, GamePiece piece, int rowInit, int acceptableBlanks);
template<BoardOrientation board>
Coords getStraightMove(TTTBoard& gb, GamePiece piece, int rowNum, int colNum, int acceptableBlanks);
bool isWinningPiece(TTTBoard&, GamePiece);
template<NumMod modifier>
bool isDiagonalWin(TTTBoard&, GamePiece, int rowInit);
template<BoardOrientation board>
bool isStraightWin(TTTBoard&, GamePiece, int rowNum, int colNum);
void run2PlayerGame(char);
void run1PlayerGame(char);
void displayBoard(TTTBoard&);
void doPlayerTurn(TTTBoard&, GamePiece);
void increamentNum(int&);
void decreamentNum(int&);
Coords rowTop(TTTBoard&, int rowIndex, int colIndex, GamePiece);
Coords colTop(TTTBoard&, int rowIndex, int colIndex, GamePiece);
void doAiImpossible(TTTBoard&, GamePiece&);
void doAiMedium(TTTBoard&, GamePiece&);
void doAiEasy(TTTBoard&, GamePiece&);
char promptUser(const char* message, const char* acceptableValues, bool clearbeforePrompt);
int promptUser(const char* message, const int minRange, const int maxRange, bool clearbeforePrompt);
void notifyUser(const char*);

const Coords BAD_MOVE;

int main()
{
	Instrumentor::Get().BeginSession("TicTacToeLifetime");
	PROFILE_FUNCTION();
	bool playAgain = false;

	do
	{
		playAgain = false;
		 
		system("CLS");

		notifyUser("Welcome to TicTacToe!\n=====================");
		
		char sizeOption = promptUser("Please select a board size: Regular, Large, or Extreme", "RrLlEe", false);
		char size = 0;
		int option = promptUser("Please select a gamemode: Single Player (1), or Two player (2)", 1, 2, false);

		if (sizeOption == 'R' || sizeOption == 'r')
		{
			size = 3;
		}
		else if (sizeOption == 'L' || sizeOption == 'l')
		{
			size = 5;
		}
		else if (sizeOption == 'E' || sizeOption == 'e')
		{
			size = 9;
		}

		if (option == 1)
		{
			run1PlayerGame(size);
		} 
		else if (option == 2)
		{
			run2PlayerGame(size);
		}

		char response = promptUser("Play Again?", "YyNn", false);

		playAgain = (response == 'Y' || response == 'y');
	} while (playAgain);

	Instrumentor::Get().EndSession();
	return 0;
}

bool isWinningPiece(TTTBoard& gb, GamePiece piece)
{
	PROFILE_FUNCTION();
	bool isWin = isStraightWin<rowTop>(gb, piece, gb.GetNumRow(), gb.GetNumCol());
	isWin = isWin || isStraightWin<colTop>(gb, piece, gb.GetNumCol(), gb.GetNumRow());
	isWin = isWin || isDiagonalWin<increamentNum>(gb, piece, 0);
	isWin = isWin || isDiagonalWin<decreamentNum>(gb, piece, gb.GetNumRow() - 1);

	return isWin;
}

template<NumMod mod>
Coords getDiagMove(TTTBoard& gb, GamePiece piece, int rowInit, int acceptableBlanks)
{
	PROFILE_FUNCTION();
	Coords winMove;

	int rowIndex = rowInit;
	int colIndex = 0;
	int numBlank = 0;

	do
	{
		if (gb.getAt(rowIndex, colIndex) == BLANK)
		{
			if (numBlank < acceptableBlanks)
			{
				winMove.row = rowIndex;
				winMove.col = colIndex;
				numBlank++;
			}
			else
			{
				return BAD_MOVE;
			}
		}
		else if (gb.getAt(rowIndex, colIndex) != piece)
		{
			return BAD_MOVE;
		}
		mod(rowIndex);
	} while (rowIndex < gb.GetNumRow() && ++colIndex < gb.GetNumCol());

	return winMove;
}

template<BoardOrientation boardCheck>
Coords getStraightMove(TTTBoard& gb, GamePiece piece, int rowNum, int colNum, int acceptableBlanks)
{
	PROFILE_FUNCTION();
	Coords winMove;

	for (int rowIndex = 0; rowIndex < rowNum; rowIndex++)
	{
		int numFound = 0;
		bool foundEnemy = false;

		for (int colIndex = 0; colIndex < colNum; colIndex++)
		{
			Coords tempMove = boardCheck(gb, rowIndex, colIndex, piece);

			if (tempMove != BAD_MOVE)
			{
				numFound++;
			}
			else
			{
				winMove = boardCheck(gb, rowIndex, colIndex, BLANK);

				if (winMove == BAD_MOVE)
				{
					foundEnemy = true;
					break;
				}
			}
		}

		if (!foundEnemy && numFound == (colNum - acceptableBlanks))
		{
			return winMove;
		}
	}

	return BAD_MOVE;
}

template<NumMod mod>
bool isDiagonalWin(TTTBoard& gb, GamePiece piece, int rowInit)
{
	PROFILE_FUNCTION();
	int rowIndex = rowInit;
	int colIndex = 0;
	int numFound = 0;

	do
	{
		numFound = (gb.getAt(rowIndex, colIndex) == piece) ? (numFound + 1) : numFound;
		mod(rowIndex);
	} while (rowIndex < gb.GetNumRow() && ++colIndex < gb.GetNumCol());

	return numFound == gb.GetNumRow();
}

template<BoardOrientation board>
bool isStraightWin(TTTBoard& gb, GamePiece piece, int rowNum, int colNum)
{
	PROFILE_FUNCTION();
	for (int rowIndex = 0; rowIndex < rowNum; rowIndex++)
	{
		int numFound = 0;

		for (int colIndex = 0; colIndex < colNum; colIndex++)
		{
			numFound = (board(gb, rowIndex, colIndex, piece) != BAD_MOVE)? (numFound + 1) : numFound;
		}

		if (numFound == colNum)
		{
			return true;
		}
	}

	return false;
}

void displayBoard(TTTBoard& gb)
{
	PROFILE_FUNCTION();

	system("CLS");

	std::cout << "|=";
	for (int ci = 0; ci < gb.GetNumCol(); ci++)
	{
		if (ci + 1 == gb.GetNumCol())
		{
			std::cout << "=" << "=";
		}
		else
		{
			std::cout << "=" << "=|=";
		}

	}
	std::cout << "|";
	std::cout << std::endl;

	for (int ri = 0; ri < gb.GetNumRow(); ri++)
	{
		std::cout << "| ";
		for (int ci = 0; ci < gb.GetNumCol(); ci++)
		{
			if (ci + 1 == gb.GetNumCol())
			{
				std::cout << (gb.getAt(ri, ci)) << " ";
			}
			else
			{
				std::cout << (gb.getAt(ri, ci)) << " | ";
			}
		}
		std::cout << "|";
		std::cout << std::endl;
		std::cout << "|=";
		for (int ci = 0; ci < gb.GetNumCol(); ci++)
		{
			if (ci + 1 == gb.GetNumCol())
			{
				std::cout << "=" << "=";
			}
			else
			{
				std::cout << "=" << "=|=";
			}

		}
		std::cout << "|";
		std::cout << std::endl;
	}
}

Coords promptUserForCoord(TTTBoard& gb)
{
	PROFILE_FUNCTION();
	Coords response;

	response.row = promptUser("Enter row you want to place your piece.", 1, gb.GetNumRow(), false) - 1;
	response.col = promptUser("Enter column you want to place your piece.", 1, gb.GetNumRow(), false) - 1;

	return response;
}

void doPlayerTurn(TTTBoard& gb, GamePiece playerPiece)
{
	PROFILE_FUNCTION();
	bool promptAgain = true;

	do
	{
		displayBoard(gb);

		std::cout << "Player " << playerPiece << " place your piece." << std::endl;

		Coords selection = promptUserForCoord(gb);

		if (!gb.isValidCoords(selection))
		{
			notifyUser("Provided coordinates are not valid!");
		}
		else
		{
			gb.placePiece(selection, playerPiece);
			promptAgain = false;
		}
	} while (promptAgain);
}

Coords getWinningMove(TTTBoard& gb, GamePiece playerPiece, int acceptableBlanks)
{
	PROFILE_FUNCTION();
	Coords winMove = getDiagMove<increamentNum>(gb, playerPiece, 0, acceptableBlanks);

	if (winMove == BAD_MOVE)
	{
		winMove = getDiagMove<decreamentNum>(gb, playerPiece, gb.GetNumRow() - 1, acceptableBlanks);

		if (winMove == BAD_MOVE)
		{
			winMove = getStraightMove<rowTop>(gb, playerPiece, gb.GetNumRow(), gb.GetNumCol(), acceptableBlanks);

			if (winMove == BAD_MOVE)
			{
				winMove = getStraightMove<colTop>(gb, playerPiece, gb.GetNumCol(), gb.GetNumRow(), acceptableBlanks);
			}
		}
	}

	return winMove;
}

void doAiImpossible(TTTBoard& gb, GamePiece& playerPiece)
{
	PROFILE_FUNCTION();
	Coords move = getWinningMove(gb, playerPiece, 1);

	if (move == BAD_MOVE)
	{
		move = getWinningMove(gb, (playerPiece == Px) ? Po : Px, 1);

		if (move == BAD_MOVE)
		{
			int middleRow = ((gb.GetNumRow() - 1) / 2);
			int middleCol = ((gb.GetNumCol() - 1) / 2);
			if (gb.getAt(middleRow, middleCol) == BLANK)
			{
				move.col = middleCol;
				move.row = middleRow;
			}
			else
			{
				move.row = 0;
				move.col = 0;

				if (!gb.isValidCoords(move))
				{
					move.row = gb.GetNumRow() - 1;

					if (!gb.isValidCoords(move))
					{
						move.col = gb.GetNumCol() - 1;

						if (!gb.isValidCoords(move))
						{
							move.row = 0;

							if (!gb.isValidCoords(move))
							{
								move = BAD_MOVE;
							}
						}
					}
				}

				int numBlanks = 2;

				while(numBlanks < gb.GetNumRow() && move == BAD_MOVE)
					move = getWinningMove(gb, playerPiece, numBlanks++);

				if (move == BAD_MOVE)
				{
					move = getWinningMove(gb, (playerPiece == Px) ? Po : Px, 2);

					if (move == BAD_MOVE)
					{
						doAiEasy(gb, playerPiece);
						return;
					}
				}
			}
		}
	}

	gb.placePiece(move, playerPiece);
}

void doAiMedium(TTTBoard& gb, GamePiece& playerPiece)
{
	PROFILE_FUNCTION();
	Coords move = getWinningMove(gb, playerPiece, 1);

	if (move == BAD_MOVE)
	{
		doAiEasy(gb, playerPiece);

		return;
	}

	gb.placePiece(move, playerPiece);
}

void doAiEasy(TTTBoard& gb, GamePiece& playerPiece)
{
	PROFILE_FUNCTION();
	Coords aiMove;
	bool needToPlace = true;

	do
	{
		aiMove.row = rand() % gb.GetNumRow();
		aiMove.col = rand() % gb.GetNumCol();

		if (gb.isValidCoords(aiMove))
		{
			gb.placePiece(aiMove, playerPiece);
			needToPlace = false;
		}

	} while (needToPlace);
}

AiDifficulty promptUserForDifficulty()
{
	PROFILE_FUNCTION();
	bool notValid = false;

	do
	{
		char input = promptUser("Please select AI difficulty: Easy (E), Medium (M), Impossible (I)", "EeMmIi", true);

		if (input == 'E' || input == 'e')
		{
			return doAiEasy;
		}
		else if (input == 'M' || input == 'm')
		{
			return doAiMedium;
		}
		else
		{
			return doAiImpossible;
		}

	} while (notValid);
}

void run2PlayerGame(char size)
{
	PROFILE_FUNCTION();
	TTTBoard gb(size, size, BLANK);
	GamePiece currPlayer = Px;
	bool runAgain = true;

	do
	{
		doPlayerTurn(gb, currPlayer);

		if (isWinningPiece(gb, currPlayer))
		{
			displayBoard(gb);
			std::cout << "Player " << currPlayer << " won!" << std::endl;
			runAgain = false;
		}

		if (gb.isFull())
		{
			displayBoard(gb);
			notifyUser("Match ends in a tie!");
			runAgain = false;
		}

		currPlayer = (currPlayer == Px) ? Po : Px;
	} while (runAgain);
}

void run1PlayerGame(char size)
{
	PROFILE_FUNCTION();
	TTTBoard gb = TTTBoard(size, size, BLANK);
	GamePiece currPlayer = Px;
	GamePiece human = ((rand() % 2) == 1)? Px : Po;
	AiDifficulty doAiTurn = promptUserForDifficulty();
	bool runAgain = true;

	do
	{
		if (currPlayer == human)
		{
			doPlayerTurn(gb, currPlayer);
		}
		else
		{
			doAiTurn(gb, currPlayer);
		}

		if (isWinningPiece(gb, currPlayer))
		{
			displayBoard(gb);
			std::cout << "Player " << (char)currPlayer << std::endl;
			runAgain = false;
		}
		else if (gb.isFull())
		{
			displayBoard(gb);
			notifyUser("Match ends in a tie!");
			runAgain = false;
		}

		currPlayer = (currPlayer == Px) ? Po : Px;
	} while (runAgain);
}

void increamentNum(int& number)
{
	number++;
}

void decreamentNum(int& number)
{
	number--;
}

Coords rowTop(TTTBoard& gb, int rowIndex, int colIndex, GamePiece value)
{
	Coords position;

	if (gb.getAt(rowIndex, colIndex) == value)
	{
		position.row = rowIndex;
		position.col = colIndex;
	}

	return position;
}

Coords colTop(TTTBoard& gb, int rowIndex, int colIndex, GamePiece value)
{
	Coords position;

	if (gb.getAt(colIndex, rowIndex) == value)
	{
		position.row = colIndex;
		position.col = rowIndex;
	}

	return position;
}

char promptUser(const char* message, const char* acceptableValues, bool clearbeforePrompt)
{
	PROFILE_FUNCTION();
	char userValue;

	do
	{
		if (clearbeforePrompt)
			system("CLS");

		std::cout << message << std::endl;
		std::cout << "Acceptable values: (";

		for(int index = 0; acceptableValues[index] != '\0'; index++)
		{
			if (acceptableValues[index + 1] == '\0')
			{
				std::cout << acceptableValues[index] << ")" << std::endl;
			}
			else
			{
				std::cout << acceptableValues[index] << ", ";
			}
		}

		userValue = std::cin.get();
		std::cin.ignore();

		for (int index = 0; acceptableValues[index] != '\0'; index++)
		{
			if (acceptableValues[index] == userValue)
			{
				return userValue;
			}
		}

		notifyUser("Invalid user input!");
		
	} while (true);
}

int promptUser(const char* message, const int minRange, const int maxRange, bool clearbeforePrompt)
{
	PROFILE_FUNCTION();
	int userValue;

	do
	{
		if (clearbeforePrompt)
			system("CLS");

		std::cout << message << std::endl;
		std::cout << "Acceptable values: (";

		for (int index = minRange; index < maxRange; index++)
		{
			std::cout << index << ", ";
		}

		std::cout << maxRange << ")" << std::endl;
		std::cin >> userValue;
		std::cin.ignore();

		if (minRange - 1 < userValue && userValue < maxRange + 1)
		{
			return userValue;
		}
		else
		{
			notifyUser("Invalid user input!");
		}

	} while (true);
}

void notifyUser(const char* message)
{
	PROFILE_FUNCTION();
	std::cout << message << std::endl;
	std::cout << "..." << std::endl;
	std::cin;
	std::cin.ignore();
}