#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#define Infinite 9999

typedef struct AdjList Graph;
typedef struct AdjList Result;

struct AdjList
{
    char* City;
    int Cost;
    Graph * Next;
};

typedef struct Node heapNode;
struct Node
{
    int City ;
    int Cost;
}  ;

typedef struct MinHeap Heap;
struct MinHeap
{
    int capacity;
    int size;
    heapNode** array;
};

Graph* FindLast( Graph* );
void InsertLast (char *,int,Graph* );
int Size(Graph*);
void DeleteList(Graph* );
int IsEmpty(Graph* L);
Graph* MakeEmpty(Graph* );
int getNumOfCities(FILE*, int, Graph* );
int MaxChar(FILE*);
int Contains(char *, Graph* );
Graph** ConstuctGraph(int *);
void DisposeGraph(Graph**, int );
void DisposeList(Graph* );

void mainMenu();
void CapitalizeEachWord(char*);
int ScanInt();
int checkValidInt (char* );
int getCost (char*);

Heap* ConstructHeap (int);
heapNode* ConstructHeapNode (int, int );
void HeapifyEachNode (Heap *, int );
Heap* Heapify(Heap* heap);
heapNode* getMin (Heap * );
void changeCost(Heap*, int, int );


void dijkstra(Graph**, char*, char*, int, char* );
void printDijsktraPath(char**,int*, int, int*, char* );

void BFS(Graph**, char*, char*, int, char* );
void printBfsPath(char**,int*, int, char* );

void SaveToFile(Result*);

int main()
{
    printf("\nWelcome To My Project That Finds The Shortest Path Between Two Cities ^_^\n\n ");
    int numOfCitites=0;
    Graph ** CitiesGraph = NULL;
    char SrcCity[15] = " ",  DstCity [15];

Result* results = MakeEmpty(NULL);

    while(1)
    {
        int choice;
        // Display the main menu
        mainMenu();
        printf("What do you want to do: ");
        choice = ScanInt();

        char DijkestraResult[500];
        char BfsResult[500];

        switch (choice)
        {

        case 1:

            if(CitiesGraph != NULL)
            {
                DisposeGraph(CitiesGraph,numOfCitites );
                CitiesGraph = NULL;
                numOfCitites = 0 ;
            }

            printf("\nCONSTRUCTING THE GRAPH....\n");
            CitiesGraph = ConstuctGraph(&numOfCitites);

            if(CitiesGraph != NULL)
                printf("The Graph Has Been constructed Successfully ^_^\n\n ");

            else
            {
                printf("Cannot Construct The Graph *_*\n\n ");
                exit(0);
            }

            break;

        case 2:

            if(CitiesGraph == NULL)
            {
                printf("\nYou Should Construct The Graph...\n");
                break;
            }

            printf("Enter Source city\n");
            gets(SrcCity);
            CapitalizeEachWord(SrcCity);

            break ;

        case 3:

            if(CitiesGraph == NULL)
            {
                printf("\nYou Should Construct The Graph...\n");
                break;
            }

            if(strcmp(" ", SrcCity)==0)
            {
                printf("\nYou Should Enter A Source City First...\n");
                break;

            }

            printf("Enter destination city\n");
            gets(DstCity);
            CapitalizeEachWord(DstCity);


            DijkestraResult[0]='\0';
            printf("\nUsing Dijkstra Algorithm:\n");

            dijkstra(CitiesGraph, SrcCity,DstCity,numOfCitites,DijkestraResult  );

            BfsResult[0]='\0';
            printf("\nUsing BFS Algorithm:\n");

            BFS(CitiesGraph, SrcCity,DstCity,numOfCitites, BfsResult );

            InsertLast(DijkestraResult , 0 , results);
             InsertLast(BfsResult , 0 , results);

            break;

        case 4:

            if(CitiesGraph != NULL)
            {
                printf("Disposing The Graph.....\n\n ");
                DisposeGraph(CitiesGraph, numOfCitites);
            }

            printf("Saving Results To The Output File.....\n");
            SaveToFile(results);
            DisposeList(results);

            printf("\nThanks For Using My Project That Finds The Shortest Path Between Two Cities ^_^\n\n ");
            exit(0);


        default:
            printf("Enter a choice between 1 and 8\n");

        }
    }

    return 0;
}

/*
*inserts a new node at the end of the list
*/
void InsertLast (char * City,int Cost,Graph*L )
{
    Graph* temp;
    temp = (Graph*)malloc(sizeof(Graph));

    if(temp == NULL)
    {
        printf("\nCannot allocate memory*_*\n\n");
        exit(0);
    }

    temp->City = strdup(City);
    CapitalizeEachWord(temp->City);
    temp->Cost = Cost;

    Graph*P = FindLast(L);

    temp->Next=P->Next;
    P->Next = temp;
}
/*
*find the last node  of the list
*/
Graph* FindLast( Graph* L)
{
    Graph* P = L;

    while(P->Next!=NULL)
        P=P->Next;

    return P;
}

/*
*returns the number of nodes in a list
*/
int Size(Graph* L)
{
    int count =0;
    Graph* P = L;

    while(P->Next != NULL)
    {
        count++;
        P = P->Next;
    }
    return count;
}

/*
*frees all nodes in the list
*/
void DeleteList(Graph* L)
{
    Graph *P, *temp;

    P = L->Next;
    L->Next = NULL;

    while (P != NULL)
    {
        temp = P->Next;
        free(P->City);
        free(P);
        P = temp;
    }
}

/*
*returns 0 if the Linked List is not Empty
*/
int IsEmpty(Graph* L)
{
    return L->Next == NULL; // returns 1 if the list is empty
}

/*
*creates an empty list by allocating memory for it
*/
Graph* MakeEmpty(Graph* L)
{
    if (L != NULL)
        DeleteList(L);

    L = (Graph*) malloc(sizeof(Graph));

    if (L == NULL)
    {
        printf("Out Of Memory *_*");
        exit(0);
    }
    L->Next = NULL;
    return L;
}

/*
* searches for a specific city in a list
*if it found return 1 o.w. return 0
*/
int Contains(char * City, Graph* L)
{
    Graph* P;
    P = L->Next;

    while(P != NULL )
    {
        if(strcmp(City, P->City)==0)
            return 1;

        P = P->Next;
    }
    return 0;
}

/*
*read the file and construct the adjacency list of the graph
*/
Graph** ConstuctGraph(int*numOfCities)
{
    char filename[] = "Cities.txt";

    FILE* fin = fopen(filename, "r");

    if (fin == NULL) //make sure that the file is opened correctly
    {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    int maxChar = MaxChar(fin);// Determine the maximum number of characters in a line
    fseek(fin, 0, SEEK_SET); // Reset the file pointer to the beginning of the file


    Graph* Cities = MakeEmpty(NULL);// Create an empty graph structure to store city data

    *numOfCities = getNumOfCities(fin, maxChar, Cities);
    fseek(fin, 0, SEEK_SET); // Reset the file pointer to the beginning of the file


    Graph** CityGraph = (Graph**)malloc(sizeof(Graph*)*(*numOfCities));// Allocate memory for an array of graph pointers to represent the city graph

    Graph * temp = Cities ;

    for(int i = 0 ; i < *numOfCities ; i++)
    {
        CityGraph[i] = MakeEmpty(NULL);// Create an empty list for the current city
        temp = temp->Next;
        InsertLast(temp->City, 0, CityGraph[i] ); // Insert the city into the corresponding graph
    }

    char* line = malloc(maxChar + 2); // Allocate memory for the line ,size of char is 1bit
    if (line == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(0);
    }
    int index =0;
    int state = 0;// State to track which part of the line is being processed

    while (fgets(line, maxChar + 2, fin) != NULL)   // Read line by line
    {
        char *des = NULL;
        int cost;

        char* token = strtok(line, "\t");

        while (token != NULL) //tokenise the line
        {

            switch(state)
            {
            case 0:// Find the source city index in CityGraph based on the token

                for(int i = 0 ; i<*numOfCities ; i++)//to find the source city
                {
                    if(strcmp(token,CityGraph[i]->Next->City )==0)
                        index = i;
                }

                state =1;
                break;

            case 1:

                des = strdup(token);
                state =2;
                break;

            case 2:

                cost = getCost(token);

                break;
                state =0;

            }
            token = strtok(NULL, "\t");

        }
        state = 0;//after finishing the line reset the state

        InsertLast(des, cost, CityGraph[index]);// After processing the line, insert the destination city and cost into the corresponding City list

        free(des);
        des = NULL;
    }
    return CityGraph;
}
/*
*reads a file line by line and counts the number of unique cities in the file.
*save names of the cities in cities list
*/
int getNumOfCities(FILE*fin, int maxChar, Graph* Cities)
{
    char* line = malloc(maxChar + 2); // Allocate memory for the line ,size of char is 1bit
    if (line == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(0);
    }

    while (fgets(line, maxChar + 2, fin) != NULL)   // Read line by line
    {
        char* token = strtok(line, "\t");

        for(int i = 0 ; i< 2 ; i++)
        {
            if (token != NULL) //tokenise the line
            {

                if(Contains(token,Cities )==0)// Check if the city is not already listed in the graph
                {
                    CapitalizeEachWord(token);
                    InsertLast(token, 0, Cities);

                }
            }

            token = strtok(NULL, "\t");
        }
    }
    return Size(Cities);// Return the number of cities in the graph
}

/*
*extracts the cost value from a string and returns it as an integer
*/
int getCost (char* num)
{
    int cost = 0  ;
    int i = 0;
    while(num[i]!='k'&& num[i]!='K') // Iterate through each character in the string until 'k' is reched
    {

        if(!((num[i]-48)>=0 && (num[i]-48)<=9))// Check if the character is not a digit
            return -1;

        // Calculate  the cost
        cost= (cost* 10)+(num[i]-48);

        i++;
    }
    return cost;
}
/*
*return the maximum number of characters in a line
*/
int MaxChar(FILE*fin)
{
    char c;
    int chars=0, maxChars=0;

    while ((c = fgetc(fin)) != EOF)//iterates over all the chars in the file
    {
        if(c!='\n')
            chars ++;

        else
        {
            if(chars>maxChars)
                maxChars=chars;

            chars=0;
        }
    }
    if(chars>maxChars)
        maxChars=chars;

    return maxChars;
}
/*
*prints the main menu
*/
void mainMenu()
{
    printf("\nChoose from the below operations:\n\n");
    printf("1. Load	Cities.\n");
    printf("2. Enter Source.\n");
    printf("3. Enter Destination.\n");
    printf("4. Exit\n");
}
/*
*reads from the user the string
*and iterates until it is valid value
*/
int ScanInt()
{
    char string [3];
    int Integer;

    strcpy(string, "");
    fgets(string, sizeof(string), stdin);// Read the student's name using fgets to allow reading a line with spaces
    string[strlen(string)-1] = '\0';

    Integer = checkValidInt(string);//get the value

    while(Integer<0) //ask the  user to enter the num again if it is not a num
    {
        printf("THIS FIELD must be integers !! Enter it again: ");
        strcpy(string,"");
        fgets(string, sizeof(string), stdin);// Read the student's name using fgets to allow reading a line with spaces
        string[strlen(string)-1] = '\0';//the last char is '\n' so make it '\0'
        Integer =checkValidInt(string);
    }
    return Integer;
}
/*
*convert a string to its integer value
*is the string contains any char
*it returns -1 o.w. it returns the value
*/
int checkValidInt (char* num)
{
    int number = 0  ;
    for(int i = 0; i <strlen(num); i++) // Iterate through each character in the string
    {

        if(!((num[i]-48)>=0 && (num[i]-48)<=9))// Check if the character is not a digit
            return -1;

        // Calculate  the number
        number= (number* 10)+(num[i]-48);
    }
    return number;
}

/*
*Dispose the adjacency list that represents the graph
*/
void DisposeGraph(Graph** CitiesGraph, int numOfCities)
{
    for(int i = 0 ; i<numOfCities ; i++)
        DisposeList(CitiesGraph[i]);

    free(CitiesGraph);
}

void DisposeList(Graph* City)
{
    DeleteList(City);
    free(City);
}
/*
*make sure that the string starts with a letter
*capitalize the first letter of each word
*make the other letter in the lowercase
*/
void CapitalizeEachWord(char*String)
{
    while(!isalpha(String[0]))
    {
        printf("Make sure that it is starts with a letter , this starts with %c \n Enter it again: ",String[0]);
        scanf("%s",String);
    }

    char capital [strlen(String)];
    strcpy(capital,"");

    char* token = strtok(String, " ");
    while (token != NULL)
    {
        if (islower(token[0]))
            token[0] -=32 ;

        for(int i = 1 ; i < strlen(token) ; i++)
        {
            if (isupper(token[i]))
                token[i] +=32 ;
        }

        strcat(capital,token);
        strcat(capital," ");

        token = strtok(NULL, " "); // Get the next token
    }
    capital[strlen(capital)-1] = '\0'; //for the last space
    strcpy(String,"");
    strcat(String, capital);
}

Heap* ConstructHeap (int capacity)
{
    Heap* heap = (Heap*)malloc(sizeof(Heap)); // Allocate memory for the heap structure

    heap->capacity = capacity;
    heap->size = 0;

    heap->array = (heapNode**)malloc(capacity * sizeof(heapNode*));// Allocate memory for the heap array (array of heapNode pointers

    return heap;
}

heapNode* ConstructHeapNode (int city, int cost)
{
    heapNode* Node = (heapNode*)malloc(sizeof(heapNode));

    // Set the city and cost values of the heapNode
    Node->City = city;
    Node->Cost = cost;

    return Node;

}

void HeapifyEachNode(Heap* heap, int index)
{
    int IndexOfSmallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

       // Check if the left child exists and has a smaller cost than the current node
    if (left < heap->size && heap->array[left]->Cost < heap->array[IndexOfSmallest]->Cost)
    {
        IndexOfSmallest = left;
    }

    // Check if the right child exists and has a smaller cost than the current node
    if (right < heap->size && heap->array[right]->Cost < heap->array[IndexOfSmallest]->Cost)
    {
        IndexOfSmallest = right;
    }

    if (IndexOfSmallest != index)// Check if the right child exists and has a smaller cost than the current node or left child
    {
        heapNode* temp = heap->array[IndexOfSmallest];
        heap->array[IndexOfSmallest] = heap->array[index];
        heap->array[index] = temp;

        HeapifyEachNode(heap, IndexOfSmallest); // Recursively call HeapifyEachNode on the swapped child to ensure the heap property is maintained
    }
}

Heap* Heapify(Heap* heap)
{
    int startIndex = (heap->size / 2) - 1;

    for (int i = startIndex; i >= 0; i--) // Iterate from the last non-leaf node up to the root of the heap
    {
        HeapifyEachNode(heap, i);
    }

    return heap;
}

heapNode* getMin (Heap * heap)
{
    if (heap->size==0)
    {
        return NULL;
    }

    heapNode* minNode = heap->array[0];// Get the minimum node from the root of the heap

    // Move the last node to the root position
    heapNode* lastNode = heap->array[heap->size - 1];
    heap->array[0] = lastNode;

    heap->size--;// Decrease the size of the heap

    Heapify(heap); // Restore the heap property by heapifying from the root

    return minNode;
}

void changeCost(Heap* heap, int City, int Cost)
{
    int i;
    for (i = 0; i < heap->size; ++i)
    {
        if (heap->array[i]->City == City)// Find the node with the specified city
        {
            heap->array[i]->Cost = Cost;// Update the cost of the node
            break;
        }
    }

    Heapify(heap); // Restore the heap property by heapifying the heap

}

void dijkstra(Graph** graph, char* srcCity, char* destCity, int numVertices, char* DijkestraResult)
{
    int* distance = (int*)malloc(numVertices * sizeof(int));
    int* visited = (int*)malloc(numVertices * sizeof(int));
    int* previous = (int*)malloc(numVertices * sizeof(int));
    char** cities = (char**)malloc(numVertices * sizeof(char*));

    // Initialize distance array, visited array, and previous array
    int i;
    for (i = 0; i < numVertices; i++)
    {
        distance[i] = Infinite;

        visited[i] = 0;
        previous[i] = -1;
        cities[i] = strdup(graph[i]->Next->City);  // Store city names for reference
    }

    // Find the index of the source city in the cities array
    int srcIndex = -1;
    for (i = 0; i < numVertices; i++)
    {
        if (strcmp(cities[i], srcCity) == 0)
        {
            srcIndex = i;
            break;
        }
    }

    // Check if the source city is found
    if (srcIndex != -1)
    {
        // Set distance of source city to 0
        distance[srcIndex] = 0;
    }
    else
    {
        printf("Source city not found.\n");
        strcat(DijkestraResult,"Source city not found." );
        return;
    }

    // Create a min heap and initialize it with vertices

    Heap* heap = ConstructHeap(numVertices);

    for (i = 0; i < numVertices; i++)
    {
        heapNode* Node = ConstructHeapNode(i, distance[i]);
        heap->array[i] = Node;
    }

    heap->size = numVertices;

    heap = Heapify(heap);

    // Dijkstra's algorithm
    while (heap->size!=0)
    {
        heapNode* minNode = getMin(heap); // Extract vertex with minimum distance from the heap
        int current = minNode->City; // Get the vertex index from the minNode

        visited[current] = 1;
        visited[current] = 1; // Mark the current vertex as visited

        Graph* temp = graph[current]->Next->Next; // Get the first neighbor of the current vertex

        while (temp != NULL)
        {
            int neighbor = -1;
            for (i = 0; i < numVertices; i++) // Find the index of the neighbor city
            {
                if (strcmp(cities[i], temp->City) == 0)
                {
                    neighbor = i;
                    break;
                }
            }
            if (neighbor != -1 && !visited[neighbor] && distance[current] != Infinite && distance[current] + temp->Cost < distance[neighbor])
            {
                distance[neighbor] = distance[current] + temp->Cost; // Update the distance to the neighbor vertex
                changeCost(heap, neighbor, distance[neighbor]); // Update the heap with the new distance value
                previous[neighbor] = current; // Set the previous vertex in the shortest path
            }
        temp = temp->Next;
        }
    }

    // Print the shortest path from the source city to the destination city
    int destIndex = -1;
    for (i = 0; i < numVertices; ++i)
    {
        if (strcmp(cities[i], destCity) == 0)
        {
            destIndex = i;// Store the index of the destination city
            break;
        }
    }

    if(destIndex == -1)
    {
        printf("Destination city not found.\n");
        strcat(DijkestraResult,"Destination city not found." );
        return;
    }


    if (distance[destIndex] != Infinite)
    {
        printf("Shortest path from %s to %s: ", srcCity, destCity);

        strcat(DijkestraResult,"Shortest path from " );
        strcat(DijkestraResult,cities[srcIndex] );
        strcat(DijkestraResult," to " );
        strcat(DijkestraResult,cities[destIndex] );
        strcat(DijkestraResult,": " );

        printDijsktraPath(cities,previous, destIndex, distance, DijkestraResult);

        printf("\nShortest distance: %d\n", distance[destIndex]);

        strcat(DijkestraResult,"\nShortest distance: " );

        char str[10];
        sprintf(str, "%d",distance[destIndex] ); // integer to string
        strcat(DijkestraResult,str );

    }
    else
        printf("No path found from %s to %s.\n", srcCity, destCity);

    // Free memory
    for (i = 0; i < numVertices; ++i)
        free(cities[i]);

    free(distance);
    free(visited);
    free(previous);
    free(cities);
    free(heap->array);
    free(heap);
}

void printDijsktraPath(char** cities, int* previous, int destIndex, int* distance, char* DijkestraResult)
{
    char str[5];
    sprintf(str, "%d", distance[destIndex]); // integer to string

    if (previous[destIndex] != -1)// Recursively print the Dijkstra path from the source to the current destination
        printDijsktraPath(cities, previous, previous[destIndex], distance, DijkestraResult);


    printf("%s (%d km) ", cities[destIndex], distance[destIndex]);

    // Append the city name and its distance to the Dijkstra result string
    strcat(DijkestraResult, " -> ");
    strcat(DijkestraResult, cities[destIndex]);
    strcat(DijkestraResult, " (");
    strcat(DijkestraResult, str);
    strcat(DijkestraResult, " km) ");
}

void BFS(Graph** graph, char* srcCity, char* destCity, int numVertices, char*BfsResult)
{

    int* visited = (int*)malloc(numVertices * sizeof(int));
    int* previous = (int*)malloc(numVertices * sizeof(int));
    char** cities = (char**)malloc(numVertices * sizeof(char*));

    int*queue = (int*)malloc(sizeof(int)*numVertices);
    int front = -1, rear = -1;

    int pathFound = 0;

// Initialize , visited array, and previous array
    int i;
    for (i = 0; i < numVertices; i++)
    {
        visited[i] = 0;
        previous[i] = -1;
        cities[i] = strdup(graph[i]->Next->City);  // Store city names for reference
    }

    int srcIndex = -1;
    for (i = 0; i < numVertices; i++)// Find the index of the source city in the cities array
    {
        if (strcmp(cities[i], srcCity) == 0)
        {
            srcIndex = i;
            break;
        }
    }

    if(srcIndex == -1)// Check if the source city is found
    {
        printf("Source city not found.\n");
        strcat(BfsResult,"Source city not found." );
        return;
    }

    int destIndex = -1;
    for (i = 0; i < numVertices; ++i)
    {
        if (strcmp(cities[i], destCity) == 0)
        {
            destIndex = i;// Store the index of the destination city
            break;
        }
    }

    if(destIndex == -1)
    {
        printf("Destination city not found.\n");
        strcat(BfsResult,"Destination city not found." );
        return;
    }

// Mark the start vertex as visited and enqueue it
    visited[srcIndex] = 1;
    queue[++rear] = srcIndex;

    while (front != rear)
    {
        // Dequeue a vertex from the queue
        int currentVertex = queue[++front];

        // If the current vertex is the end vertex, a path is found
        if (currentVertex == destIndex)
        {
            pathFound = 1;
            break;
        }

        Graph* temp = graph[currentVertex]->Next->Next;

        while (temp != NULL)
        {
            int neighbor = -1;
            for (i = 0; i < numVertices; i++) // Find the index of the neighbor city
            {
                if (strcmp(cities[i], temp->City) == 0)
                {
                    neighbor = i;
                    break;
                }
            }

            if (!visited[neighbor])
            {
                // Mark the adjacent vertex as visited, enqueue it, and store its parent
                visited[neighbor] = 1;
                queue[++rear] = neighbor;
                previous[neighbor] = currentVertex;
            }
            temp = temp->Next;
        }
    }

    if (pathFound)
    {
        // Print the path from srcIndex to destIndex
        printf("Path from %s to %s: ", cities[srcIndex], cities[destIndex]);

        strcat(BfsResult,"Path from " );
        strcat(BfsResult,cities[srcIndex] );
        strcat(BfsResult," to " );
        strcat(BfsResult,cities[destIndex] );
        strcat(BfsResult,": " );

        printBfsPath(cities, previous, destIndex, BfsResult);
        printf("\n");
    }
    else
    {
        printf("No path found from %s to %s.\n", cities[srcIndex], cities[destIndex]);
        strcat(BfsResult,"No path found from " );
        strcat(BfsResult,cities[srcIndex] );
        strcat(BfsResult," to " );
        strcat(BfsResult,cities[destIndex] );
    }

// Free memory
    for (i = 0; i < numVertices; ++i)
    {
        free(cities[i]);
    }

    free(visited);
    free(previous);
    free(cities);
    free(queue);
}

void printBfsPath(char** cities,int* previous, int destIndex, char * BfsResult)
{
    if (previous[destIndex] != -1)// Check if there is a previous vertex for the destination city
    {
        // Recursively call printPath to print the path from the source to the previous vertex
        printBfsPath(cities,previous, previous[destIndex], BfsResult);
        printf("-> %s ", cities[destIndex]);
        strcat(BfsResult," -> " );
        strcat(BfsResult,cities[destIndex] );
    }

    else // If there is no previous vertex, it means the current city is the source city and print it  and its distance
    {
        printf("%s ", cities[destIndex]);
        strcat(BfsResult,cities[destIndex] );
    }
}

void SaveToFile(Result*results){

 char filename[] = "shortest_distance.txt";

    FILE* fout = fopen(filename, "w");// Open the file for writing

    if (fout == NULL) //make sure that the file is opened correctly
    {
        printf("Failed to open file: %s\n", filename);
        return ;
    }

Result* temp = results->Next;
int counter = 1;

while(temp!= NULL && temp->Next!= NULL){    // Iterate through the results and write them to the file
    fprintf(fout,"\n\n==>For Search #%d: \n" , counter++);

     fprintf(fout , "\nUsing Dijkstra Algorithm:\n");
    fprintf(fout , "%s\n" , temp->City);

     fprintf(fout , "\nUsing BFS Algorithm:\n");
    fprintf(fout , "\n%s\n" , temp->Next->City);
    fprintf(fout,"______________________________________________________________________________________________________________________________________________");
temp = temp->Next->Next;
}
fclose(fout);

printf("The Results Have Been Saved Successfully ^_^\n");
}
