/*
CIS 3490
Assignment 4
Question 1.1
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
#define n 600 //keep this value identical to the one above
#define TOTAL_WORDS 2045
#define MAX_WORD_LENGTH 17 //the longest word responsibilities is 16 characters long
#define INT_MAX 2147483647 //just like in <limits.h>

/*
Structs
*/

/*
A structure that saves the nodes of the Optimal Binary Search Tree.
Each node holds the key (search word), the count (number of times it appears in data_A4_Q1,
The probability (count / total words), the minimum average number of comparisons and the index
of the left and right child nodes.
*/
typedef struct word
{
	char key[MAX_WORD_LENGTH];
	int count;
	double probability;
	double minCost;
	int leftNode, rightNode;
} Word;

/*
Function Definitions
*/

void saveWord( Word *wordArray, int *wordArrayCount, char newWord[MAX_WORD_LENGTH] );
void swapNodes( Word *wordArray, int lower, int upper );
void optimalBST( Word *P, double *C, int *R );
int constructTree( Word *P, double *C, int *R, int i, int j );

/*
Functions
*/
/*
A function that prints the root and average number tables.
Used in debugging
*/
void printA( double *averageNumberTable, int *rootTable ){
	printf("R:\n");
	for(int i = 1; i < n + 1; i++){
		for(int j = i; j < n + 1; j++){
			printf("%d %.3f, ", rootTable[i * (n + 1) + j], averageNumberTable[i * (n + 1) + j]);
		}
		printf("\n");
	}
	return;
}

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
A function that creates the averageNumberTable (c[][]) and rootTable (R[][]).
Input: 	A pointer to the wordArray (P[]), averageNumberTable, and rootTable
Output: directly modifies the averageNumberTable and rootTable. Does not return anything.
*/
void optimalBST( Word *P, double *C, int *R ){
	//All p[] values must have -1, because it goes from [0..n - 1], not [1..n]
	//All C and R have n + 2 rows and n + 1 columns
	for(int i = 1; i < n + 1; i++){ //for i <- 1 to n do
		C[i * (n + 1) + i - 1] = 0; //C[i][i-1]
		C[i * (n + 1) + i] = P[i - 1].probability; //C[i][i]
		R[i * (n + 1) + i] = i; //R[i][i]
	}
	C[(n + 1) * (n + 1) + n] = 0;
	
	for(int d = 1; d < n; d++){ //for d <- 1 to n-1 do //diagonal count
		for(int i = 1; i < n - d + 1; i++){ //for i <- 1 to n - d do
			int j = i + d;
			double minval = INT_MAX; //should be a high enough number not to require DOUBLE_MIN
			int kmin;
			
			for(int k = i; k < j + 1; k++){ //for k <- i to j do
				if( C[i * (n + 1) + k - 1] + C[(k + 1) * (n + 1) + j] < minval){ //C[i][k-1] + C[K+1][j]
					minval = C[i * (n + 1) + k - 1] + C[(k + 1) * (n + 1) + j]; //C[i][k-1] + C[k+1][j]
					kmin = k;
				}
			}
			R[i * (n + 1) + j] = kmin; //R[i][j]
			double sum = P[i - 1].probability;
			
			for(int s = i + 1; s < j + 1; s++){ //for s <- i +1 to j do
				sum += P[s - 1].probability;
			}
			C[i * (n + 1) + j] = minval + sum; //C[i][j]
		}
	}
	
	return;
}

/*
A function that creates the tree by updating the wordArray (p[]) nodes to hold the index of the left and right child nodes.
Input: 	A pointer to the wordArray, averageNumberTable (C[][]), and rootTable (R[][]), and an integer holding the i and j values which
		represent the current table index being pointed at.
Output: directly modifies the wordArray. Does not return anything.
*/
int constructTree( Word *P, double *C, int *R , int i, int j ){
	//All p[] values must have -1, because it goes from [0..n - 1], not [1..n]
	//printf("For R[%d][%d]\n", i, j); //for debugging
	if(i > n || j < 0 || j < i){
		//printf("Back\n"); //for debugging
		return -1;
	}
	int k = R[i * (n + 1) + j]; //root index in P[] <- R[i][j] - 1
	P[k - 1].minCost = C[i * (n + 1) + j];
	//printf("Left node\n"); //for debugging
	P[k - 1].leftNode = constructTree( P, C, R , i, k - 1 ); //
	//printf("Right node\n"); //for debugging
	P[k - 1].rightNode = constructTree( P, C, R , k + 1, j ); //
	
	return k; //returns the index of the current node
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
	Word *wordArray = malloc(UNIQUE_WORD_INSTANCES * sizeof(Word)); //acts as P[1..n] in OptimalBST and also as the tree
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
		//printf("Word: %s, Count: %d, probability: %.3f\n", wordArray[i].key, wordArray[i].count, wordArray[i].probability); //for debugging
	}
	
	/*
	construct tree
	*/
	double* averageNumberTable = malloc((n + 2) * (n + 1) * sizeof(double));
	int* rootTable = malloc((n + 2) * (n + 1) * sizeof(int));
	
	optimalBST(wordArray, averageNumberTable, rootTable);
	//printA(averageNumberTable, rootTable); //for debugging
	int rootIndex = constructTree(wordArray, averageNumberTable, rootTable, 1, n);
	
	free(averageNumberTable);
	free(rootTable);
	
	/*
	search tree
	*/
	char inputKey[MAX_WORD_LENGTH];
	printf("Please enter a key:\n");
	scanf("%s", inputKey);
	
	while(rootIndex > 0){
		printf("Compared with: %s (%.3f), ", wordArray[rootIndex - 1].key, wordArray[rootIndex - 1].minCost);
		int stringCmp = strcmp(inputKey, wordArray[rootIndex - 1].key);
		if(!stringCmp){
			printf("found.\n");
			break;
		}
		else if(stringCmp < 0){
			printf("go left subtree.\n");
			rootIndex = wordArray[rootIndex - 1].leftNode;
		}
		else{
			printf("go right subtree.\n");
			rootIndex = wordArray[rootIndex - 1].rightNode;
		}
	}
	if(rootIndex < 0){
		printf("Not found.\n");
	}
	
	free(wordArray);
}