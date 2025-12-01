#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 256

// Definition of HaffmanNode structure
typedef struct HaffmanNode {
    unsigned char data;
    int weight;
    struct HaffmanNode *left, *right;
} HaffmanNode;

// Definition of MinHeap structure
typedef struct {
    HaffmanNode **nodes;
    int size;
    int capacity;
} MinHeap;

void swap(HaffmanNode **a, HaffmanNode **b) {
    HaffmanNode *temp = *a;
    *a = *b;
    *b = temp;
}

void ShiftUp(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->nodes[index]->weight < heap->nodes[parent]->weight) {
            swap(&heap->nodes[index], &heap->nodes[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void ShiftDown(MinHeap *heap, int index) {
    int left, right, smallest;
    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;

        if (left < heap->size && heap->nodes[left]->weight < heap->nodes[smallest]->weight) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right]->weight < heap->nodes[smallest]->weight) {
            smallest = right;
        }
        if (smallest != index) {
            swap(&heap->nodes[index], &heap->nodes[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void InsertMinHeap(MinHeap *heap, HaffmanNode *node) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size] = node;
    //reorganize the heap
    ShiftUp(heap, heap->size);
    heap->size++;
}

HaffmanNode* PopMinHeap(MinHeap *heap) {
    if (heap->size == 0) return NULL;
    HaffmanNode *minNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    //turn it into a min-heap again
    ShiftDown(heap, 0);
    return minNode;
}

//Create a Haffman Node
HaffmanNode* CreateHaffmanNode(unsigned char data, int weight) {
    HaffmanNode *node = (HaffmanNode *)malloc(sizeof(HaffmanNode));
    node->data = data;
    node->weight = weight;
    node->left = node->right = NULL;
    return node;
}   

// Build Haffman Tree
HaffmanNode* BuildHaffmanTree(int freq[]) {
    MinHeap heap;
    heap.size = 0;
    heap.capacity = MAX;
    heap.nodes = (HaffmanNode **)malloc(sizeof(HaffmanNode *) * heap.capacity);

    // Insert all characters with non-zero frequency into the min-heap
    for (int i = 0; i < MAX; i++) {
        if (freq[i] > 0) {
            HaffmanNode *node = CreateHaffmanNode((unsigned char)i, freq[i]);
            InsertMinHeap(&heap, node);
        }
    }

    while (heap.size > 1) {
        //find out the smallest node
        HaffmanNode *left = PopMinHeap(&heap);
        //find out the second smallest node
        HaffmanNode *right = PopMinHeap(&heap);
        //combine them into a new node
        HaffmanNode *newNode = CreateHaffmanNode(0, left->weight + right->weight);
        newNode->left = left;
        newNode->right = right;
        InsertMinHeap(&heap, newNode);
    }

    HaffmanNode *root = PopMinHeap(&heap);
    free(heap.nodes);
    return root;
}

void GenerateHaffmanCodes(HaffmanNode *root, char *code, int depth, char codes[MAX][MAX]) {
    /*
        char codes[MAX][MAX]:
        first dimension: character (0-255)
        second dimension: corresponding Haffman code as string
    */
    if (!root) return;

    if (!root->left && !root->right) {
        code[depth] = '\0';
        strcpy(codes[root->data], code);
        return;
    }

    code[depth] = '0';
    GenerateHaffmanCodes(root->left, code, depth + 1, codes);

    code[depth] = '1';
    GenerateHaffmanCodes(root->right, code, depth + 1, codes);
}

void HaffmanDecode(HaffmanNode *root, char *encoded, char *output) {
    HaffmanNode *current = root;
    int outIndex = 0;

    for (int i = 0; encoded[i]; i++) {
        if (encoded[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (!current->left && !current->right) {
            output[outIndex++] = current->data;
            current = root;
        }
    }
    output[outIndex] = '\0';
}

void FreeHaffmanTree(HaffmanNode *root) {
    if (!root) return;
    FreeHaffmanTree(root->left);
    FreeHaffmanTree(root->right);
    free(root);
}

int main() {
    char text[] = "Hello World!"; // Sample text

    int freq[MAX] = {0};
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }
    
    HaffmanNode *root = BuildHaffmanTree(freq);
    char codes[MAX][MAX] = {{0}};
    char code[MAX];
    GenerateHaffmanCodes(root, code, 0, codes);

    //Each character's Haffman code
    for (int i = 0; text[i]; i++)
        printf("Character: %c, Code: %s\n", text[i], codes[(unsigned char)text[i]]);

    // Encode
    char encoded[1000] = {0};
    for (int i = 0; text[i]; i++) {
        strcat(encoded, codes[(unsigned char)text[i]]);
    }
    printf("Encoded: %s\n", encoded);

    // Decode
    char decoded[1000] = {0};
    HaffmanDecode(root, encoded, decoded);
    printf("Decoded: %s\n", decoded);

    FreeHaffmanTree(root);

    return 0;
}