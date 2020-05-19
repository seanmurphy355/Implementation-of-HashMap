#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MIN(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
/**
 * Implementation of the Levenshtein distance used to find the distance between two words which indicates the min number of single character edits
 * required to change one word into another.Used to help find word distances in the dictionary when we have a missspelled word!
 *
 * @return
 */
 // Source: https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance Used the C implementation given on this webpage
int Mylevenshtein(char* s1, char* s2) {
    unsigned int MyLengthOne, MyLengthTwo, vOne, vTwo, newD, oldD;
    MyLengthOne = strlen(s1);
    MyLengthTwo = strlen(s2);
    unsigned int column[MyLengthOne + 1];
    for (vTwo = 1; vTwo <= MyLengthOne; vTwo++)
        column[vTwo] = vTwo;
    for (vOne = 1; vOne <= MyLengthTwo; vOne++) {
        column[0] = vOne;
        for (vTwo = 1, newD = vOne - 1; vTwo <= MyLengthOne; vTwo++) {
            oldD = column[vTwo];
            column[vTwo] = MIN(column[vTwo] + 1, column[vTwo - 1] + 1, newD + (s1[vTwo - 1] == s2[vOne - 1] ? 0 : 1));
            newD = oldD;
        }
    }
    return(column[MyLengthOne]);
}



/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    assert(map != NULL);
    assert(file != NULL);
    //check to make sure that map is null and that our dictionary dile is not null either!

    char* insertWord = nextWord(file);
    // load the entire dictionary and check to see if there is nothing else in the dictionary 
    while (insertWord != NULL) {
        hashMapPut(map, insertWord, -1);
        free(insertWord);
        insertWord = nextWord(file);
    }
}



/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    HashMap* map = hashMapNew(1000);
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    // tells you the time that it takes for the dictionary to load!
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    int quit = 0;
         
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");                  
        printf("\n");
        scanf("%s", inputBuffer);
        // Implement the spell checker code here
        // converts the string to lower case
        char Mychar = 'sup';
        for (int i = 0; i < strlen(inputBuffer); i++)
        {
            inputBuffer[i] = tolower(inputBuffer[i]);
            //if (inputBuffer[i] == NULL) {
              //  Mychar = NULL;
           //}
        }
        //printf("%c", Mychar)
        //Check user input wont work ...... becuase of how white space is being read...... no idea how to fix this to be honest 
       // while (Mychar == NULL) {
           // printf("Please enter a valid input ");
          //  printf("Enter a word or \"quit\" to quit: ");
           // printf("\n");
           // scanf("%s", inputBuffer);
        //}
        //quit if the user enters quit
        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
        //yikes we didnt get the correct word lets give some suggestions
        if ((hashMapContainsKey(map, inputBuffer)) == 0) {
            printf("(%s) -> Opps was spelled incorrectly.\n", inputBuffer);
            int Value = 0;
            int countNumbers;
            int cap = map->capacity;
            int k;
            for (int i = cap; i > 0; i--)
            {
                //seach the reverse of the table with dec for loop
                struct HashLink* TempLink = map->table[i];
                while (TempLink != 0) {
                    if (Value == 6) {
                        break;
                    }
                    int lev = Mylevenshtein(inputBuffer, TempLink->key);
                    hashMapPut(map, map->table[i]->key, lev);
                    if (Mylevenshtein(inputBuffer, TempLink->key) == 1 && Value < 5)
                    {
                        countNumbers += 1;
                        Value += 1;
                        printf("(%d) Suggestion- %s\n", Value, TempLink->key);
                        //printf("");

                    }
                    TempLink = TempLink->next;
                }
            }
            for (int i = 0; i < cap; i++)
            {
                //seach the reverse of the table with dec for loop
                //If i add another map for loop here I can then grab a few closer items but since I used a dec for loop these 
                //items could be further away I believe........
                //check further distance if neeeded.......... though with my imp this could be endless........probs need a why to set
                //(Mylevenshtein(inputBuffer, myLink->key) == x were is is being incremented based off of passes of 1,2,3,4,5 etc.. so we always get 5 suggestions
                struct HashLink* TempLink = map->table[i];
                while (TempLink != 0) {
                    if (Value == 6) {
                        break;
                    }
                    int lev = Mylevenshtein(inputBuffer, TempLink->key);
                    hashMapPut(map, map->table[i]->key, lev);
                    if (Mylevenshtein(inputBuffer, TempLink->key) == 2 && Value < 5 && Value == countNumbers)
                    {
                        countNumbers += 1;
                        Value += 1;
                        printf("(%d) Suggestion- %s\n", Value, TempLink->key);

                    }
                    else if (Mylevenshtein(inputBuffer, TempLink->key) == 3 && Value < 5 && Value < countNumbers) {
                        Value += 1;
                        printf("(%d) Suggestion- %s\n", Value, TempLink->key);
                        //printf("");
                    }
                    TempLink = TempLink->next;
                }
            }
          
        }
        //print that the word was spelled correctly and has a matching key
        else if (hashMapContainsKey(map, inputBuffer)) {
            printf("(%s) -> inputed word was spelled correctly.\n", inputBuffer);
        }
    }
    hashMapDelete(map);
    return 0;
}
