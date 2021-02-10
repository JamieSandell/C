#include <stdio.h>

#define MAX_WORDS   10

//Prints a histogram of the lengths of words from its input
int main()
{
    int c, numberOfCharacters, numberOfWords, wordLengthsIndex, largestWordLength;
    c = numberOfCharacters = numberOfWords = wordLengthsIndex = largestWordLength = 0;
    int wordLengths[MAX_WORDS];

    //Read the words
    while ( ((c = getchar()) != EOF) && numberOfWords < MAX_WORDS)
    {
        if (c == ' ' || c == '\n' || c == '\t') //no longer in a word
        {
            if (numberOfCharacters > largestWordLength)
            {
                largestWordLength = numberOfCharacters;
            }
            wordLengths[wordLengthsIndex] = numberOfCharacters;
            numberOfCharacters = 0; //reset for the next word
            numberOfWords++;
            wordLengthsIndex++;
        }
        else //treat anything else as a character in a word
        {
            numberOfCharacters++;
        }        
    }

    //Print the histogram horizontally
    for (int numberOfWordsIndex = 0; numberOfWordsIndex < numberOfWords; ++numberOfWordsIndex)
    {
        for (int numberOfCharactersIndex = 0; numberOfCharactersIndex < wordLengths[numberOfWordsIndex]; ++numberOfCharactersIndex)
        {
            putchar('*');
        }
        putchar('\n');
        putchar('\n');
    }

    //Print the histogram vertically
   for (int character = largestWordLength; character > 0; --character) //cycle each row
   {
       for (int word = 0; word < numberOfWords; ++word) //cycle each column
       {
           if (wordLengths[word] >= character) //this particular word length falls within the current row
           {
               putchar('*');
               putchar('\t');
           }
           else
           {
               putchar(' ');
               putchar('\t');
           }           
       }
       putchar ('\n');
   }

    return 0;
}