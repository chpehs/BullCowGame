// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FBullCowCount Count;

    Isograms = GetValidWords(Words);

    //PrintLine(TEXT("%i"), FMath::RandRange(0, 10));

    SetupGame();//setting up game

    PrintLine(TEXT("the number of possible words is %i."), Words.Num());
    PrintLine(TEXT("number of valid words is %i."), GetValidWords(Words).Num());
    PrintLine(TEXT("ValidWords -1 is: %i"), GetValidWords(Words).Num() - 1);
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //Checking player guess
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    //Welcome the player
    PrintLine(TEXT("Hi there\n Press TAB to play"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //setting Hidden Word
    Lives = HiddenWord.Len(); //setting lives
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());//Debug Line
    PrintLine(TEXT("You have %i lives"), Lives);

    PrintLine(TEXT("Hidden word is: %s."), *HiddenWord);

    //const TCHAR HW[] = TEXT("booby");
    //PrintLine(TEXT("Character 1 of the hidden word is: %c"), HiddenWord[0]); //print first character 
    //PrintLine(TEXT("4th character of HW is: %c"), HW[4]); // prints "b"
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You Won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Hidden word is %i character long"), HiddenWord.Len());
        PrintLine(TEXT("Try guessing again\nYou have %i lives remaining"), Lives);
        return;
    }

    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("no repeating letters, guess again"));
        return;
    }
    // Remove Life
    PrintLine(TEXT("You lost a life!"));
    --Lives;

    // Check If Lives > 0
    // If Yes GuessAgain
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("Hidden word was:"), *HiddenWord);
        EndGame();
        return;
    }
    
    //Show player bull and cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    // Show Lives Left
    PrintLine(TEXT("Guess again you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
 }

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
  TArray<FString> ValidWords;

  for (FString Word : WordList)
  {
      if (Word.Len() >=4 && Word.Len() <= 8 && IsIsogram(Word))
      {
          if (!IsIsogram(Word))
          {
            ValidWords.Emplace(Word);
          }
      }
  }
  return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    //for every the index of Guess is same as HiddenWord, BullCount++
        //for wasn't a Bull, if yes CowCount++
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls ++ ;
            continue;
        }
        for (int32 HiddenIndex = 0 ; HiddenIndex < HiddenWord.Len(); HiddenIndex++ )
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
