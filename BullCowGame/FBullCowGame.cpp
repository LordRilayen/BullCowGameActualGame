#pragma once

#include <iostream>
#include <map>

//to make syntax Unreal friendly
#define TMap std::map

#include "FBullCowGame.h"

//to make syntax Unreal friendly
using int32 = int;

FBullCowGame::FBullCowGame(){ Reset(); }

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7,20} };
	return WordLengthToMaxTries[MyHiddenWord.length()]; 
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; //this MUST be an isogram

	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

void FBullCowGame::PrintGameSummary()
{
	if (bGameIsWon)
	{
		std::cout << "CONGRATULATIONS! YOU WON!!1!\n";
	}
	else
	{
		std::cout << "Better luck next time...\n";
	}
	std::cout << std::endl;
}

EGuessStatus FBullCowGame::CheckGuessValidity (FString Guess) const
{
	//if the guess isn't an isogram
	if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram; 
	}
	//if the guess isn't all lowercase
	else if (!IsLowercase(Guess))
	{
		return EGuessStatus::Not_Lowercase; 
	}
	//if the guess length is wrong
	else if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::Wrong_Length;
	}
	//otherwise
	else
	{
		return EGuessStatus::OK;
	}
}

//receives a valid guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	FBullCowCount BullCowCount;
	
	MyCurrentTry++;
	int32 WordLength = MyHiddenWord.length(); //assuming same length as guess

	//loop through all letters in the hidden world
	
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		//compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			//if they match
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				//if they're in the same place
				if (MHWChar == GChar)
				{
					//increment bulls
					BullCowCount.Bulls++;
				}
				//if they're not
				else if (MHWChar != GChar)
				{
					//increment cows
					BullCowCount.Cows++;
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Guess) const
{
	//treat 0 and 1 letter words as isograms
	if (Guess.length() <= 1) { return true; }

	//create a map
	TMap<char, bool> LetterSeen;

	//loop through the letters in the guess
	for (auto Letter : Guess)
	{
		Letter = tolower(Letter);
		//if a letter is in the map
		if (LetterSeen[Letter])	{ return false; } //we do NOT have an isogram
		else
		{
			//add it to the map
			LetterSeen[Letter] = true;
		}
	}

	return true; //for example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(FString Guess) const
{
	if (Guess.length() < 1) { return true; }

	for (auto Letter : Guess)
	{
		if (!islower(Letter) || isspace(Letter))
		{
			return false;
		}
	}

	return true;
}


