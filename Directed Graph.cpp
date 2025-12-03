#include<stdio.h>
#include<stdlib.h>

//Definition of AdjListNode structure
typedef struct AdjListNode {
    int vertex;                     //The vertex number
    struct AdjListNode* next;
} AdjListNode;


//Definition of Graph structure
typedef struct Graph {
    int numVertices;                //The number of vertices in the graph
    AdjListNode** adjLists;         //Array of adjacency lists
} Graph;

//Function to create a graph with a given number of vertices
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;

    graph->adjLists = (AdjListNode**)malloc(vertices * sizeof(AdjListNode*));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

//Function to create a new adjacency list node
AdjListNode* createNode(int vertex) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph* graph, int src, int dest) {
    //Add edge from src to dest
    AdjListNode* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

