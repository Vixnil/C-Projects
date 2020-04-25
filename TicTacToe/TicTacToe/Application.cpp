#include <iostream>
#include "GameBoard.h"

typedef GameBoard::GamePiece GamePiece;
typedef GameBoard::Coords Coords;
typedef void(*AiDifficulty)(GameBoard&, GamePiece&);
typedef void(*NumMod)(int&);
typedef Coords(*BoardOrientation)(GameBoard&, int, int, GamePiece);

Coords promptUserForCoord(GameBoard&);
Coords getWinningMove(GameBoard& gb, GamePiece playerPiece, int acceptableBlanks);
template<NumMod mod>
Coords getDiagMove(GameBoard& gb, GamePiece piece, int rowInit, int acceptableBlanks);
template<BoardOrientation board>
Coords getStraightMove(GameBoard& gb, GamePiece piece, int rowNum, int colNum, int acceptableBlanks);
bool isWinningPiece(GameBoard&, GamePiece);
template<NumMod modifier>
bool isDiagonalWin(GameBoard&, GamePiece, int rowInit);
template<BoardOrientation board>
bool isStraightWin(GameBoard&, GamePiece, int rowNum, int colNum);
void run2PlayerGame();
void run1PlayerGame();
void displayBoard(GameBoard&);
void doPlayerTurn(GameBoard&, GamePiece);
void increamentNum(int&);
void decreamentNum(int&);
Coords rowTop(GameBoard&, int rowIndex, int colIndex, GamePiece);
Coords colTop(GameBoard&, int rowIndex, int colIndex, GamePiece);
void doAiImpossible(GameBoard&, GamePiece&);
void doAiMedium(GameBoard&, GamePiece&);
void doAiEasy(GameBoard&, GamePiece&);
char promptUser(const char* message, const char* acceptableValues, bool clearbeforePrompt);
int promptUser(const char* message, const int minRange, const int maxRange, bool clearbeforePrompt);
void notifyUser(const char*);

const Coords BAD_MOVE;

int main()
{
	bool playAgain = false;

	do
	{
		playAgain = false;
		
		system("CLS");

		notifyUser("Welcome to TicTacToe!\n=====================");
		
		int option = promptUser("Please select a gamemode: Single Player (1), or Two player (2)", 1, 2, false);

		if (option == 1)
		{
			run1PlayerGame();
		} 
		else if (option == 2)
		{
			run2PlayerGame();
		}

		char response = promptUser("Play Again?", "YyNn", false);

		playAgain = (response == 'Y' || response == 'y');
	} while (playAgain);

	return 0;
}

bool isWinningPiece(GameBoard& gb, GamePiece piece)
{
	bool isWin = isStraightWin<rowTop>(gb, piece, gb.GetNumRow(), gb.GetNumCol());
	isWin = isWin || isStraightWin<colTop>(gb, piece, gb.GetNumCol(), gb.GetNumRow());
	isWin = isWin || isDiagonalWin<increamentNum>(gb, piece, 0);
	isWin = isWin || isDiagonalWin<decreamentNum>(gb, piece, gb.GetNumRow() - 1);

	return isWin;
}

template<NumMod mod>
Coords getDiagMove(GameBoard& gb, GamePiece piece, int rowInit, int acceptableBlanks)
{
	Coords winMove;

	int rowIndex = rowInit;
	int colIndex = 0;
	int numBlank = 0;

	do
	{
		if (gb.returnBoard()[rowIndex][colIndex] == GamePiece::BLANK)
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
		else if (gb.returnBoard()[rowIndex][colIndex] != piece)
		{
			return BAD_MOVE;
		}
		mod(rowIndex);
	} while (rowIndex < gb.GetNumRow() && ++colIndex < gb.GetNumCol());

	return winMove;
}

template<BoardOrientation boardCheck>
Coords getStraightMove(GameBoard& gb, GamePiece piece, int rowNum, int colNum, int acceptableBlanks)
{
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
				winMove = boardCheck(gb, rowIndex, colIndex, GamePiece::BLANK);

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
bool isDiagonalWin(GameBoard& gb, GamePiece piece, int rowInit)
{
	int rowIndex = rowInit;
	int colIndex = 0;
	int numFound = 0;

	do
	{
		numFound = (gb.returnBoard()[rowIndex][colIndex] == piece) ? (numFound + 1) : numFound;
		mod(rowIndex);
	} while (rowIndex < gb.GetNumRow() && ++colIndex < gb.GetNumCol());

	return numFound == gb.GetNumRow();
}

template<BoardOrientation board>
bool isStraightWin(GameBoard& gb, GamePiece piece, int rowNum, int colNum)
{
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

void displayBoard(GameBoard& gb)
{
	char** board = gb.returnBoard();

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
				std::cout << (board[ri][ci]) << " ";
			}
			else
			{
				std::cout << (board[ri][ci]) << " | ";
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

Coords promptUserForCoord(GameBoard& gb)
{
	Coords response;

	response.row = promptUser("Enter row you want to place your piece.", 1, gb.GetNumRow(), false) - 1;
	response.col = promptUser("Enter column you want to place your piece.", 1, gb.GetNumRow(), false) - 1;

	return response;
}

void doPlayerTurn(GameBoard& gb, GamePiece playerPiece)
{
	bool promptAgain = true;

	do
	{
		displayBoard(gb);

		std::cout << "Player " << (char)playerPiece << " place your piece." << std::endl;

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

Coords getWinningMove(GameBoard& gb, GamePiece playerPiece, int acceptableBlanks)
{
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

void doAiImpossible(GameBoard& gb, GamePiece& playerPiece)
{
	Coords move = getWinningMove(gb, playerPiece, 1);

	if (move == BAD_MOVE)
	{
		move = getWinningMove(gb, (playerPiece == GamePiece::Px) ? GamePiece::Po : GamePiece::Px, 1);

		if (move == BAD_MOVE)
		{
			if (gb.returnBoard()[1][1] == GamePiece::BLANK)
			{
				move.col = 1;
				move.row = 1;
			}
			else
			{
				move = getWinningMove(gb, playerPiece, 2);

				if (move == BAD_MOVE)
				{
					move = getWinningMove(gb, (playerPiece == GamePiece::Px) ? GamePiece::Po : GamePiece::Px, 2);

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

void doAiMedium(GameBoard& gb, GamePiece& playerPiece)
{
	Coords move = getWinningMove(gb, playerPiece, 1);

	if (move == BAD_MOVE)
	{
		doAiEasy(gb, playerPiece);

		return;
	}

	gb.placePiece(move, playerPiece);
}

void doAiEasy(GameBoard& gb, GamePiece& playerPiece)
{
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

void run2PlayerGame()
{
	GameBoard gb = GameBoard(3, 3, GamePiece::BLANK);
	GamePiece currPlayer = GamePiece::Px;
	bool runAgain = true;

	do
	{
		doPlayerTurn(gb, currPlayer);

		if (isWinningPiece(gb, currPlayer))
		{
			displayBoard(gb);
			std::cout << "Player " << (char)currPlayer << " won!" << std::endl;
			runAgain = false;
		}

		if (gb.isFull())
		{
			displayBoard(gb);
			notifyUser("Match ends in a tie!");
			runAgain = false;
		}

		currPlayer = (currPlayer == GamePiece::Px) ? GamePiece::Po : GamePiece::Px;
	} while (runAgain);
}

void run1PlayerGame()
{
	GameBoard gb = GameBoard(3, 3, GamePiece::BLANK);
	GamePiece currPlayer = GamePiece::Px;
	GamePiece human = ((rand() % 2) == 1)? GamePiece::Px : GamePiece::Po;
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

		currPlayer = (currPlayer == GamePiece::Px) ? GamePiece::Po : GamePiece::Px;
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

Coords rowTop(GameBoard& gb, int rowIndex, int colIndex, GamePiece value)
{
	Coords position;

	if (gb.returnBoard()[rowIndex][colIndex] == value)
	{
		position.row = rowIndex;
		position.col = colIndex;
	}

	return position;
}

Coords colTop(GameBoard& gb, int rowIndex, int colIndex, GamePiece value)
{
	Coords position;

	if (gb.returnBoard()[colIndex][rowIndex] == value)
	{
		position.row = colIndex;
		position.col = rowIndex;
	}

	return position;
}

char promptUser(const char* message, const char* acceptableValues, bool clearbeforePrompt)
{
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
	std::cout << message << std::endl;
	std::cout << "..." << std::endl;
	std::cin;
	std::cin.ignore();
}