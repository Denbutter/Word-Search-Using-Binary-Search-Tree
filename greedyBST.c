/*
CIS 3490
Assignment 4
Question 1.2
March 27th, 2023
Created by: John Denbutter
1056466
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Constants
*/

#define CHARACTER_COUNT 14685 //total characters in data_A4_Q1.txt
#define UNIQUE_WORD_INSTANCES 600
#define TOTAL_WORDS 2045
#define MAX_WORD_LENGTH 17 //the longest word responsibilities is 16 characters long

/*
Structs
*/

/*
A structure that saves the nodes of the Optimal Binary Search Tree.
Each node holds the key (search word), the count (number of times it appears in data_A4_Q1,
The probability (count / total words), and the index of the left and right child nodes.
*/
typedef struct word
{
	char key[MAX_WORD_LENGTH];
	int count;
	double probability;
	int leftNode, rightNode;
} Word;

/*
Function Definitions
*/

void saveWord( Word *wordArray, int *wordArrayCount, char newWord[MAX_WORD_LENGTH] );
void swapNodes( Word *wordArray, int lower, int upper );
int constructTree( Word *wordArray, int min, int max );

/*
Functions
*/

/*
A function that adds a given word to the wordArray.
Input: 	A pointer to the wordArray, a pointer to the counter holding the current number of words in wordArray,
		A string holding the new word to be added (or incremented).
Output: directly modifies wordArray. Does not return anything.
*/
void saveWord( Word *wordArray, int *wordArrayCount, char newWord[MAX_WORD_LENGTH] ){
	for(int x = 0; x < *wordArrayCount; x++){
		if(!strcmp(wordArray[x].key, newWord)){ //if the words already exists
			wordArray[x].count++;
			return;
		}
	}
	
	//if the word does not already exist
	strcpy(wordArray[*wordArrayCount].key, newWord);
	//printf("Added word: %s, at: %d\n", wordArray[*wordArrayCount].key, *wordArrayCount); //for debugging
	wordArray[*wordArrayCount].count = 1;
	(*wordArrayCount)++;
	
	return;
}

/*
A helper function that swaps two nodes in wordArray.
Input: 	A pointer to the wordArray, and an int that holds the indices that will be swapped.
Output: directly modifies wordArray. Does not return anything.
*/
void swapNodes( Word *wordArray, int lower, int upper ){
	Word temp;
	strcpy(temp.key, wordArray[lower].key);
	temp.count = wordArray[lower].count;
	
	strcpy(wordArray[lower].key, wordArray[upper].key);
	wordArray[lower].count = wordArray[upper].count;
	
	strcpy(wordArray[upper].key, temp.key);
	wordArray[upper].count = temp.count;
	
	return;
}

/*
A function that creates the tree by updating the wordArray (p[]) nodes to hold the index of the left and right child nodes.
Input: 	A pointer to the wordArray, and an integer holding the minimum and maximum indices of the subtree.
Output: directly modifies the wordArray. Does not return anything.
*/
int constructTree( Word *wordArray, int min, int max ){
	//printf("Min: %d, Max: %d\n", min, max); //for debugging
	if(min > UNIQUE_WORD_INSTANCES){
		//printf("Back: %d\n", UNIQUE_WORD_INSTANCES - 1); //for debugging
		return UNIQUE_WORD_INSTANCES - 1;
	}
	if(max < 0){
		//printf("Back: 0\n"); //for debugging
		return 0;
	}
	if(min == max){
		//printf("Back: %d\n", min); //for debugging
		return min;
	}
	int maxCount = -1;
	int index = -1;
	for(int i = min; i < max + 1; i++){
		if(wordArray[i].count > maxCount){
			index = i;
			maxCount = wordArray[i].count;
		}
	}
	if(index < 0){
		return -1;
	}
	//printf("Current index: %d\n", index);
	//printf("Left node\n"); //for debugging
	wordArray[index].leftNode = constructTree( wordArray, min, index - 1 ); //
	//printf("Right node\n"); //for debugging
	wordArray[index].rightNode = constructTree( wordArray, index + 1, max ); //
	
	return index; //returns the index of the current node
}

/*
main
*/
int main ( int argc, char* argv[] ){
	/*
	File processing
	*/
	char *fileData;
	long int fileSize = -1;
	FILE * fp;
	if ((fp = fopen("data_A4_Q1.txt", "r")) != NULL){
		/* copy file data into array *fileData */
		fseek(fp, 0L, SEEK_END);
		fileSize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		fileData = malloc(fileSize + 1); //malloc space for the entire file plus \0.
		if(!fileData){ //check successful malloc
			printf("Error: malloc unsuccessful.\nTerminating program.\n\n");
			exit(1);
		}
		if(fread(fileData, fileSize, 1, fp) < 1){ //check successful copy
			printf("Error: could not read from file.\nTerminating program.\n\n");
			exit(1);
		}
		fclose(fp); //close file
	}
	else{
		printf("Error: No data exists because \"data_A4_Q1.txt\" could not be opened successfully.\nTerminating program.\n\n");
		exit(1);
	}

	/*
	Save words and frequency to an array "wordArray" by individually saving the characters between spaces. (tokenization did not work).
	*/
	Word *wordArray = malloc(UNIQUE_WORD_INSTANCES * sizeof(Word)); //acts as the tree
	int wordArrayCount = 0;
	char newWord[MAX_WORD_LENGTH];
	int newWordCharCount = 0;
	for(int i = 0; i < CHARACTER_COUNT; i++){
		if(fileData[i] != ' ' && fileData[i] != '\n'){
			newWord[newWordCharCount] = fileData[i];
			newWordCharCount++;
		}
		else if(newWordCharCount > 0){
			newWord[newWordCharCount] = '\0';
			saveWord(wordArray, &wordArrayCount, newWord); //save word to array of words, or increment if it already exists.
			memset(&newWord[0], 0, newWordCharCount);
			newWordCharCount = 0;
		}
	}
	free(fileData);
	
	//printf("Words found: %d, out of: %d\n", wordArrayCount, UNIQUE_WORD_INSTANCES); //for debugging
	
	/*
	Sort the keys in alphabetic order using simple bubble sort
	*/
	for(int i = 0; i < UNIQUE_WORD_INSTANCES; i++){
		for(int j = 0; j < UNIQUE_WORD_INSTANCES - 1 - i; j++){
			if(strcmp(wordArray[j].key, wordArray[j+1].key) > 0){
				swapNodes(wordArray, j, j + 1);
			}
		}
	}
	
	/*
	Calculate probability
	*/
	for(int i = 0; i < UNIQUE_WORD_INSTANCES; i++){
		wordArray[i].probability = wordArray[i].count / (double)TOTAL_WORDS;
		wordArray[i].leftNode = -1;
		wordArray[i].rightNode = -1;
		//printf("Word: %s, Count: %d, probability: %.3f\n", wordArray[i].key, wordArray[i].count, wordArray[i].probability); //for debugging
	}
	
	/*
	construct tree
	*/
	int rootIndex = constructTree(wordArray, 0, UNIQUE_WORD_INSTANCES - 1);
	
	/*
	search tree
	*/
	char inputKey[MAX_WORD_LENGTH];
	printf("Please enter a key:\n");
	scanf("%s", inputKey);
	
	while(rootIndex > -1){
		printf("Compared with: %s (%.3f), ", wordArray[rootIndex].key, wordArray[rootIndex].probability);
		int stringCmp = strcmp(inputKey, wordArray[rootIndex].key);
		if(!stringCmp){
			printf("found.\n");
			break;
		}
		else if(stringCmp < 0){
			printf("go left subtree.\n");
			rootIndex = wordArray[rootIndex].leftNode;
		}
		else{
			printf("go right subtree.\n");
			rootIndex = wordArray[rootIndex].rightNode;
		}
	}
	if(rootIndex < 0){
		printf("Not found.\n");
	}
	
	free(wordArray);
}