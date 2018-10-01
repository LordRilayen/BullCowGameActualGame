/*This is the console executable that makes
	use of the FBullCowGame class.

	This acts as the View in a MVC pattern, 
	and is responsible for all user interactin.
	For game logic see the FBullCowGame class.
*/

#pragma once

#include <iostream>
#include <string>

#include "FBullCowGame.h"

//to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

//function prototypes as outside a class
void PrintIntro();
void PlayGame();
FText GetValidGuess();
void PrintGuess(FText);
bool AskToPlayAgain();

FBullCowGame BCGame; //instantiate a new game which we reuse across plays

//entry point for our application
int main() 
{
	bool bWantsToPlayAgain = false;
	do
	{
		PrintIntro();
		PlayGame();
		bWantsToPlayAgain = AskToPlayAgain();
	} while (bWantsToPlayAgain);
	return 0;
}

//introduce the game
void PrintIntro()
{
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

//plays a single game to completion
void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	//loop asking for guess while the game is NOT won
	//and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();
		
		//Submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	BCGame.PrintGameSummary();

	return;
}

//loop until we get a valid guess from the user
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {

		int32 MyCurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << MyCurrentTry << " of " << BCGame.GetMaxTries() << ". Please enter a guess! ";
		getline(std::cin, Guess);

		//check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter an isogram.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter only lower case letters.\n\n";
			break;
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		default:
			//assuming the guess is valid, as we have exhausted all checks
			break;
		}
		
	} while (Status != EGuessStatus::OK); //keep looping until we get no errors
	
	return Guess;
}

void PrintGuess(FText Guess)
{
	//repeat the guess back from them
	std::cout << "Your guess was: " << Guess << std::endl;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again with the same hidden word (y/n)? ";
	FText Response = "";
	getline(std::cin, Response);

	return (Response[0] == 'y') || (Response[0] == 'Y');
}
