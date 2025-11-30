#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256   // ASCII 字符范围

// ==========================
// 哈夫曼树节点定义
// ==========================
typedef struct HuffmanNode {
    unsigned char ch;          // 字符
    int weight;                // 权值（出现次数）
    struct HuffmanNode *left;  // 左子树
    struct HuffmanNode *right; // 右子树
} HuffmanNode;


// ==========================
// 最小堆（优先队列）
// ==========================
typedef struct {
    HuffmanNode* data[MAX_CHAR];
    int size;
} MinHeap;

void swap(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* t = *a;
    *a = *b;
    *b = t;
}

void heapifyUp(MinHeap* heap, int idx) {
    int parent = (idx - 1) / 2;
    while (idx > 0 && heap->data[idx]->weight < heap->data[parent]->weight) {
        swap(&heap->data[idx], &heap->data[parent]);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    int left, right, smallest;
    while (1) {
        left = 2 * idx + 1;
        right = 2 * idx + 2;
        smallest = idx;

        if (left < heap->size && heap->data[left]->weight < heap->data[smallest]->weight)
            smallest = left;
        if (right < heap->size && heap->data[right]->weight < heap->data[smallest]->weight)
            smallest = right;

        if (smallest == idx) break;

        swap(&heap->data[idx], &heap->data[smallest]);
        idx = smallest;
    }
}

void heapPush(MinHeap* heap, HuffmanNode* node) {
    heap->data[heap->size++] = node;
    heapifyUp(heap, heap->size - 1);
}

HuffmanNode* heapPop(MinHeap* heap) {
    if (heap->size == 0) return NULL;
    HuffmanNode* root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);
    return root;
}


// ==========================
// 构建哈夫曼树
// ==========================
HuffmanNode* buildHuffmanTree(int freq[]) {
    MinHeap heap = { .size = 0 };

    // 把出现过的字符生成节点并放入最小堆
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i] > 0) {
            HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
            node->ch = (unsigned char)i;
            node->weight = freq[i];
            node->left = node->right = NULL;
            heapPush(&heap, node);
        }
    }

    // 合并节点构建哈夫曼树
    while (heap.size > 1) {
        HuffmanNode* n1 = heapPop(&heap);
        HuffmanNode* n2 = heapPop(&heap);

        HuffmanNode* newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode));
        newNode->ch = 0;
        newNode->weight = n1->weight + n2->weight;
        newNode->left = n1;
        newNode->right = n2;

        heapPush(&heap, newNode);
    }

    return heapPop(&heap);
}


// ==========================
// 生成哈夫曼编码（递归）
// ==========================
void buildCodes(HuffmanNode* root, char* code, int depth, char codes[MAX_CHAR][MAX_CHAR]) {
    if (!root) return;

    // 叶子节点（实际字符）
    if (!root->left && !root->right) {
        code[depth] = '\0';
        strcpy(codes[root->ch], code);
        return;
    }

    code[depth] = '0';
    buildCodes(root->left, code, depth + 1, codes);

    code[depth] = '1';
    buildCodes(root->right, code, depth + 1, codes);
}


// ==========================
// 解码
// ==========================
void decode(HuffmanNode* root, char* encoded, char* output) {
    HuffmanNode* cur = root;
    int outIdx = 0;

    for (int i = 0; encoded[i]; i++) {
        cur = (encoded[i] == '0') ? cur->left : cur->right;

        // 到达叶子
        if (!cur->left && !cur->right) {
            output[outIdx++] = cur->ch;
            cur = root;
        }
    }
    output[outIdx] = '\0';
}


// ==========================
// 主函数
// ==========================
int main() {
    char text[] = "hello world";  // 你可以改成任意英文文章

    int freq[MAX_CHAR] = {0};

    // 统计频率
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }

    // 构建哈夫曼树
    HuffmanNode* root = buildHuffmanTree(freq);

    // 生成编码表
    char codes[MAX_CHAR][MAX_CHAR] = {0};
    char code[256];
    buildCodes(root, code, 0, codes);

    // 显示编码表
    printf("=== Huffman Codes ===\n");
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i] > 0) {
            if (i == ' ') printf("[space]");
            else printf("'%c'", i);
            printf(" : %s\n", codes[i]);
        }
    }

    // 编码整个文本
    char encoded[10240] = "";
    for (int i = 0; text[i]; i++) {
        strcat(encoded, codes[(unsigned char)text[i]]);
    }

    printf("\nEncoded: %s\n", encoded);

    // 解码
    char decoded[10240];
    decode(root, encoded, decoded);

    printf("Decoded: %s\n", decoded);

    return 0;
}
