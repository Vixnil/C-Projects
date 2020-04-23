#include <iostream>
#include "GameBoard.h"

typedef GameBoard::GamePiece GamePiece;
typedef GameBoard::Coords Coords;
typedef void(*AiDifficulty)(GameBoard&, GamePiece&);
typedef void(*NumMod)(int&);
typedef Coords(*BoardOrientation)(GameBoard&, int, int, GamePiece);

Coords promptUserForCoord();

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

const Coords BAD_MOVE;

int main()
{
	bool playAgain = false;

	do
	{
		playAgain = false;

		system("CLS");

		std::cout << "Welcome to TicTacToe!" << std::endl
			<< "=====================" << std::endl
			<< "Please select a gamemode:" << std::endl
			<< "Single Player (1), or Two player (2)" << std::endl;
		char option = std::cin.get();

		std::cin.get();

		if (option == '1')
		{
			run1PlayerGame();
		} 
		else if (option == '2')
		{
			run2PlayerGame();
		}
		else
		{
			std::cout << "Invalid option. Please use '1' or '2'" << std::endl << "..." << std::endl;
			std::cin.get();
			playAgain = true;
			continue;
		}

		std::cout << "Play again?" << std::endl << "(Y/N)" << std::endl;
		char response = std::cin.get();
		std::cin.get();

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
	int** board = gb.returnBoard();

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
			const char* displayPiece = (board[ri][ci] == GamePiece::BLANK) ? "-" : (board[ri][ci] == GamePiece::Px) ? "X" : "O";

			if (ci + 1 == gb.GetNumCol())
			{
				std::cout << displayPiece << " ";
			}
			else
			{
				std::cout << displayPiece << " | ";
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

Coords promptUserForCoord()
{
	Coords response;

	std::cout << "Enter row you want to place your piece." << std::endl;
	char row = std::cin.get();

	//Capture the escape character before getting new input.
	std::cin.get();

	std::cout << "Enter column you want to place your piece." << std::endl;
	char col = std::cin.get();

	//Capture the escape character
	std::cin.get();

	response.row = row - '1';
	response.col = col - '1';

	return response;
}

void doPlayerTurn(GameBoard& gb, GamePiece playerPiece)
{
	bool promptAgain = true;

	do
	{
		displayBoard(gb);

		std::cout << "Player " << ((playerPiece == GamePiece::Px) ? "X" : "O") << " place your piece." << std::endl;

		Coords selection = promptUserForCoord();

		if (!gb.isValidCoords(selection))
		{
			std::cout << "Provided coordinates are not valid!" << std::endl << "..." << std::endl;
			std::cin.get();
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
		system("CLS");

		std::cout << "Please select AI difficulty: " << std::endl << "Easy (E), Medium (M), Impossible (I)" << std::endl;
		char input = std::cin.get();

		std::cin.get();

		if (input == 'E' || input == 'e')
		{
			return doAiEasy;
		}
		else if (input == 'M' || input == 'm')
		{
			return doAiMedium;
		}
		else if (input == 'I' || input == 'i')
		{
			return doAiImpossible;
		}
		else
		{
			std::cout << "Not a valid input!" << std::endl << "..." << std::endl;
			std::cin.get();
			notValid = true;
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
			std::cout << "Player " << ((currPlayer == GamePiece::Px) ? "X" : "O") << " won!" << std::endl;
			runAgain = false;
		}

		if (gb.isFull())
		{
			displayBoard(gb);
			std::cout << "Match ends in a tie!" << std::endl;
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
			std::cout << "Player " << ((currPlayer == GamePiece::Px) ? "X" : "O") << " won!" << std::endl;
			runAgain = false;
		}
		else if (gb.isFull())
		{
			displayBoard(gb);
			std::cout << "Match ends in a tie!" << std::endl;
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