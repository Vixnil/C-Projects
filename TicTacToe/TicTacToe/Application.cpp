#include <iostream>
#include "GameBoard.h"
#include "TTTDataTypes.h"

Coords promptUserForCoord();

bool isWinningPiece(GameBoard&, GamePiece);
bool isDiagonalWin(GameBoard&, GamePiece);
bool isHorizontalWin(GameBoard&, GamePiece);
bool isVerticleWin(GameBoard&, GamePiece);

void run2PlayerGame();
void run1PlayerGame();
void displayBoard(GameBoard&);
void doPlayerTurn(GameBoard&, GamePiece);

void doAiImpossible(GameBoard&, GamePiece);
void doAiMedium(GameBoard&, GamePiece);
void doAiEasy(GameBoard&, GamePiece);
void doAiTurn(Difficulty, GameBoard&, GamePiece);

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
	bool isWin = isHorizontalWin(gb, piece);
	isWin = isWin || isVerticleWin(gb, piece);
	isWin = isWin || isDiagonalWin(gb, piece);

	return isWin;
}

bool isDiagonalWin(GameBoard& gb, GamePiece piece)
{
	int rowIndex = 0;
	int colIndex = 0;
	int numFound = 0;

	do
	{
		numFound = (gb.returnBoard()[rowIndex][colIndex] == piece) ? (numFound + 1) : numFound;
	} while (++rowIndex < gb.GetNumRow() && ++colIndex < gb.GetNumCol());

	if (numFound == gb.GetNumRow())
	{
		return true;
	}

	rowIndex--;
	colIndex = 0;
	numFound = 0;

	do
	{
		numFound = (gb.returnBoard()[rowIndex][colIndex] == piece) ? (numFound + 1) : numFound;
	} while (--rowIndex > -1 && ++colIndex < gb.GetNumCol());

	if (numFound == gb.GetNumRow())
	{
		return true;
	}

	return false;
}

bool isHorizontalWin(GameBoard& gb, GamePiece piece)
{
	for (int rowIndex = 0; rowIndex < gb.GetNumRow(); rowIndex++)
	{
		int numFound = 0;

		for (int colIndex = 0; colIndex < gb.GetNumCol(); colIndex++)
		{
			numFound = (gb.returnBoard()[rowIndex][colIndex] == piece)? (numFound + 1) : numFound;
		}

		if (numFound == gb.GetNumCol())
		{
			return true;
		}
	}

	return false;
}

bool isVerticleWin(GameBoard& gb, GamePiece piece)
{
	for (int colIndex = 0; colIndex < gb.GetNumCol(); colIndex++)
	{
		int numFound = 0;

		for (int rowIndex = 0; rowIndex < gb.GetNumRow(); rowIndex++)
		{
			numFound = (gb.returnBoard()[rowIndex][colIndex] == piece) ? (numFound + 1) : numFound;
		}

		if (numFound == gb.GetNumRow())
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
			const char* displayPiece = (board[ri][ci] == BLANK) ? "-" : (board[ri][ci] == Px) ? "X" : "O";

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

		std::cout << "Player " << ((playerPiece == Px) ? "X" : "O") << " place your piece." << std::endl;

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

void doAiTurn(Difficulty brainSize, GameBoard& gb, GamePiece playerPiece)
{
	if (brainSize == Easy)
	{
		doAiEasy(gb, playerPiece);
	}
	else if (brainSize == Medium)
	{
		doAiMedium(gb, playerPiece);
	}
	else if (brainSize == Impossible)
	{
		doAiImpossible(gb, playerPiece);
	}
}

void doAiImpossible(GameBoard& gb, GamePiece playerPiece)
{

}

void doAiMedium(GameBoard& gb, GamePiece playerPiece)
{

}

void doAiEasy(GameBoard& gb, GamePiece playerPiece)
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

Difficulty promptUserForDifficulty()
{
	bool notValid = false;

	do
	{
		std::cout << "Please select AI difficulty: " << std::endl << "Easy (E), Medium (M), Impossible (I)" << endl;
		char input = std::cin.get();

		if (input == 'E' || input == 'e')
		{
			return Easy;
		}
		else if (input == 'M' || input == 'm')
		{
			return Medium;
		}
		else if (input == 'I' || input == 'i')
		{
			return Impossible;
		}

	} while (notValid);
}

void run2PlayerGame()
{
	GameBoard gb = GameBoard(3, 3, BLANK);
	GamePiece currPlayer = Px;
	bool runAgain = true;

	do
	{
		doPlayerTurn(gb, currPlayer);

		if (isWinningPiece(gb, currPlayer))
		{
			displayBoard(gb);
			std::cout << "Player " << ((currPlayer == Px) ? "X" : "O") << " won!" << std::endl;
			runAgain = false;
		}

		if (gb.isFull())
		{
			displayBoard(gb);
			std::cout << "Match ends in a tie!" << std::endl;
			runAgain = false;
		}

		currPlayer = (currPlayer == Px) ? Po : Px;
	} while (runAgain);
}

void run1PlayerGame()
{
	GameBoard gb = GameBoard(3, 3, BLANK);
	GamePiece currPlayer = Px;
	GamePiece human = ((rand() % 2) == 1)? Px : Po;
	Difficulty brainSize = promptUserForDifficulty();
	bool runAgain = true;

	do
	{
		if (currPlayer == human)
		{
			doPlayerTurn(gb, currPlayer);
		}
		else
		{
			doAiTurn(brainSize, gb, currPlayer);
		}

		if (isWinningPiece(gb, currPlayer))
		{
			displayBoard(gb);
			std::cout << "Player " << ((currPlayer == Px) ? "X" : "O") << " won!" << std::endl;
			runAgain = false;
		}

		if (gb.isFull())
		{
			displayBoard(gb);
			std::cout << "Match ends in a tie!" << std::endl;
			runAgain = false;
		}

		currPlayer = (currPlayer == Px) ? Po : Px;
	} while (runAgain);
}