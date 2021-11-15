// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWords.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    //setup game
    SetupGame();

}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }   
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    //Intro
    PrintLine(TEXT("Hey, Welcome to Bulls and Cows"));
    PrintLine(TEXT("Each Bull count is right number of letter\nplaced at right position"));
    PrintLine(TEXT("Each Cow count is right number of letter\nplaced at wrong position"));
    PrintLine(TEXT("You need to find %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Press enter to continue"));

    //Debug Line
    //PrintLine(TEXT("The Hidden Word: %s"), *HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
}

//to pick words from HiddenWords.h file
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
    //create new Array of type FString
    TArray<FString> ValidWords;

    //by using for range loop
    for (FString Word : Words)
    {
        if (Word.Len()>=4 && Word.Len() <= 10 && IsIsogram(Word)) 
        {
            ValidWords.Emplace(Word);
        }
    }

    //return array
    return ValidWords;
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    //loop to check isogram
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            //check for repeated word letter by letter
            if (Word[Index] == Word[Comparison])
                return false;
        }
    }
    //return true if the word is isogram
    return true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    //if Guess goes Right
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("%s \npress enter to play again!"), *WinWords[FMath::RandRange(0,WinWords.Num() -1)]);
        EndGame();
        return;
    }

    //remove a life
    PrintLine(TEXT("You have lost a life"));
    --Lives;

    //show the lives left
    PrintLine(TEXT("You have %i lives left"), Lives);

    //if there are no lives left
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("Game Over! \nYou have no lives left\nThe Hidden word was %s\npress enter to play again"), *HiddenWord);
        EndGame();
        return;
    }

    //promt guess again
    if (Guess.Len() != HiddenWord.Len())
    {
        //prints out total letters
        PrintLine(TEXT("Its a %i letters word"), HiddenWord.Len());
        return;
    }

    //check for isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("The hidden word is an Isogram"));
        return;
    }

    //count bulls and cows
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
            }
        }
    }
    return Count;
}

