#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    int wordEnd;
    struct Trie *children[26];
};

struct Trie *createTrie();

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    int length = strlen(word);
    int i;
    struct Trie *tmp = pTrie; 
    for(i = 0; i < length; i++) 
    { 
        if(tmp->children[word[i]-'a'] == NULL)
        { 
            tmp->children[word[i]-'a'] = createTrie(); 
        } 
        tmp = tmp->children[word[i]-'a'];
    }
    tmp->wordEnd++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int length = strlen(word);
    int i;
    struct Trie *tmp = pTrie;
    for(i = 0; i < length; i++) 
    {
        if(tmp->children[word[i]-'a'] == NULL) 
        {
           return 0;
        } 
        tmp = tmp->children[word[i]-'a'];
    }
    return tmp->wordEnd;
        
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    int i;
    struct Trie* tmp;
    for(i = 0; i < 26; i++) 
    {
        if(pTrie->children[i] != NULL) 
        {
            tmp = pTrie->children[i];
            free(pTrie);
            pTrie = tmp;
        }
    }
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *newTrie = malloc(sizeof(struct Trie));
    if(newTrie == NULL) 
    {
        printf("Memory not allocated\n");
        exit(-1);
    }
    newTrie->wordEnd = 0;
    int i;
    for(i = 0; i < 26; i++) 
    {
        newTrie->children[i] = NULL;
    }
    return newTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* fp = fopen(filename,"r");
	int count = 0;
    char buffer[26];
	int i;
	*pInWords = NULL;
	
	if (fp)
	{
		fscanf(fp,"%d\n",&count);
		*pInWords = (char *)malloc(sizeof(char) * count);
		
		if (pInWords== NULL)
		{
			printf("Memory not allocated\n");
			exit(-1);
		}
		for (i = 0; i < count; i++)
		{
            pInWords[i] = malloc(sizeof(char) * (strlen(buffer) + 1));
            if (pInWords[i]== NULL)
		    {
			    printf("Memory not allocated\n");
			    exit(-1);
		    }
			fscanf(fp, "%s", buffer);
			strcpy(pInWords[i], buffer);
		}
		fclose(fp);
	}
	return count;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}