#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAXVEX 100
#define INF 0x3f3f3f3f

int n;
int adj[MAXVEX][MAXVEX];

void Dijkstra(int v0, int dist[], int path[]) {
    int S[MAXVEX];
    for (int v = 0; v < n; v++) {
        dist[v] = adj[v0][v];
        if (dist[v] < INF)
            path[v] = v0;
        else
            path[v] = -1;
        S[v] = 0;
    }
    dist[v0] = 0;
    S[v0] = 1;

    for (int i = 1; i < n; i++) {
        int min = INF;
        int u = -1;
        for (int w = 0; w < n; w++) {
            if (!S[w] && dist[w] < min) {
                u = w;
                min = dist[w];
            }
        }
        if (u == -1) break;
        S[u] = 1;

        for (int w = 0; w < n; w++) {
            if (!S[w] && (min + adj[u][w] < dist[w])) {
                dist[w] = min + adj[u][w];
                path[w] = u;
            }
        }
    }
}

void PrintPath(int path[], int v0, int v) {
    if (v == v0) {
        printf("%d ", v0);
        return;
    }
    if (path[v] == -1) {
        printf("No path from %d to %d\n", v0, v);
        return;
    }
    int stack[MAXVEX];
    int top = -1;
    while (v != v0) {
        stack[++top] = v;
        v = path[v];
    }
    printf("%d ", v0);
    while (top != -1) {
        printf("-> %d ", stack[top--]);
    }
    printf("\n");
}

int main() {
    for (int i = 0; i < MAXVEX; i++)
        for (int j = 0; j < MAXVEX; j++)
            adj[i][j] = INF;

    int m;
    printf("Enter number of cities n (<= %d) and number of roads m:\n", MAXVEX);
    if (scanf("%d %d", &n, &m) != 2) {
        printf("Invalid input.\n");
        return 1;
    }
    if (n > MAXVEX || n <= 0) {
        printf("Number of vertices exceeds maximum limit.\n");
        return 1;
    }

    printf("Now enter %d edges: each line -> source destination time (positive integer), vertex ids 0..%d\n", m, n-1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        if (u < 0 || u >= n || v < 0 || v >= n) {
            fprintf(stderr, "Invalid vertex indices for edge: %d %d\n", u, v);
            return 1;
        }
        // If multiple edges exist between the same vertices, keep the one with the smallest time
        if (w < adj[u][v]) adj[u][v] = w;
    }
     int s, t;
    printf("Enter start vertex s and end vertex t (ids 0..%d):\n", n-1);
    if (scanf("%d %d", &s, &t) != 2) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    if (s < 0 || s >= n || t < 0 || t >= n) {
        fprintf(stderr, "Invalid start or end vertex id\n");
        return 1;
    }

    int dist[MAXVEX], prev[MAXVEX];
    Dijkstra(s, dist, prev);

    if (dist[t] == INF) {
        printf("No reachable path from %d to %d.\n", s, t);
    } else {
        printf("Shortest time from %d to %d is: %d\n", s, t, dist[t]);
        printf("Path: ");
        PrintPath(prev, s, t);
        printf("\n");
    }

    return 0;
}