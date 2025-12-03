#include<stdio.h>
#include<stdlib.h>

typedef struct AdjListNode {
    int adjvex;                     // 邻接点域
    struct AdjListNode* next;       // 指向下一个邻接点的指针
} AdjListNode;

typedef struct Graph {
    int numVertices;                // 图的顶点数
    AdjListNode** adjLists;         // 邻接表数组
} Graph;

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;

    graph->adjLists = (AdjListNode**)malloc(vertices * sizeof(AdjListNode*));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    // 添加从src到dest的边
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->adjvex = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

void DFS(Graph* graph, int v, int* visited) {
    visited[v] = 1;
    printf("%d ", v);

    AdjListNode* temp = graph->adjLists[v];
    while (temp) {
        int adjVertex = temp->adjvex;
        if (!visited[adjVertex]) {
            DFS(graph, adjVertex, visited);
        }
        temp = temp->next;
    }
}

void BFS(Graph* graph, int startVertex) {
    int* visited = (int*)malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; i++) {
        visited[i] = 0;
    }

    int* queue = (int*)malloc(graph->numVertices * sizeof(int));
    int front = 0, rear = 0;

    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    while (front < rear) {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex);

        AdjListNode* temp = graph->adjLists[currentVertex];
        while (temp) {
            int adjVertex = temp->adjvex;
            if (!visited[adjVertex]) {
                visited[adjVertex] = 1;
                queue[rear++] = adjVertex;
            }
            temp = temp->next;
        }
    }

    free(visited);
    free(queue);
}

int main() {
    Graph* graph = createGraph(5);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    printf("Depth First Search (starting from vertex 0):\n");
    int* visitedDFS = (int*)malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; i++) {
        visitedDFS[i] = 0;
    }
    DFS(graph, 0, visitedDFS);
    printf("\n");

    printf("Breadth First Search (starting from vertex 0):\n");
    BFS(graph, 0);
    printf("\n");

    free(visitedDFS);
    // Free graph memory (not shown for brevity)

    return 0;
}