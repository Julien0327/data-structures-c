#include<stdio.h>
#include<stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode, *BinaryTree;

typedef struct QueueNode {
    BinaryTree Data;
    struct QueueNode* next;
} QueueNode, *QueuePtr;

typedef struct {
    QueuePtr front;
    QueuePtr rear;
} LinkQueue;

typedef struct StackNode {
    BinaryTree data;
    struct StackNode *next;
} StackNode, *LinkStack;

// Create a new node with user input
BinaryTree CreateBinaryTree() {
    int data;
    printf("Please enter the value of the node (-1 for NULL):\n");
    scanf("%d", &data);
    if (data == -1) { // Using -1 as a sentinel value for NULL
        return NULL;
    }

    TreeNode* T = (TreeNode*)malloc(sizeof(TreeNode));
    if (!T) 
        return NULL;
    T->data = data;
    printf("Creating left child of %d:\n", data);
    T->left = CreateBinaryTree();
    printf("Creating right child of %d:\n", data);
    T->right = CreateBinaryTree();
    return T;
}

void PreOrderTraversal(BinaryTree T) {
    if (T != NULL) {
        printf("%d ", T->data);
        PreOrderTraversal(T->left);
        PreOrderTraversal(T->right);
    }
}

void InOrderTraversal(BinaryTree T) {
    if (T != NULL) {
        InOrderTraversal(T->left);
        printf("%d ", T->data);
        InOrderTraversal(T->right);
    }
}

void PostOrderTraversal(BinaryTree T) {
    if (T != NULL) {
        PostOrderTraversal(T->left);
        PostOrderTraversal(T->right);
        printf("%d ", T->data);
    }
}

void InitStack(LinkStack *S){
    *S = NULL;
}

int IsEmpty(LinkStack S) {
    return S == NULL;
}

void PushStack(LinkStack *S, BinaryTree T){
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    if (!node)
        return;
    
    node->data = T;
    node->next = *S;
    *S = node;
}

BinaryTree PopStack(LinkStack *S){
    if (IsEmpty(*S))
        return NULL;
    
    StackNode *p = *S;
    BinaryTree T = p->data;
    *S = p->next;
    free(p);
    return T;
}

void PreOrderTraversalNonRecursive(BinaryTree T) {
    if (!T) {
        printf("The tree is empty!\n");
        return;
    }
    
    LinkStack S;
    InitStack(&S);
    PushStack(&S, T);

    while (!IsEmpty(S))
    {
        //pop a node from stack and process it
        BinaryTree node = PopStack(&S);
        printf("%d ", node->data);
        if (node->right)
            PushStack(&S, node->right);
        if (node->left)
            PushStack(&S, node->left);
    }
    
}

void InOrderTraversalNonRecursive(BinaryTree T) {
    LinkStack S;
    InitStack(&S);
    BinaryTree current = T;

    while (current != NULL || !IsEmpty(S)) {
        while (current != NULL) {
            PushStack(&S, current);
            current = current->left;
        }
        current = PopStack(&S);
        printf("%d ", current->data);
        current = current->right;
    }
}

void PostOrderTraversalNonRecursive(BinaryTree T) {
    if (!T) {
        printf("The tree is empty!\n");
        return;
    }

    LinkStack S1, S2;
    InitStack(&S1);
    InitStack(&S2);
    PushStack(&S1, T);

    while (!IsEmpty(S1)) {
        BinaryTree node = PopStack(&S1);
        PushStack(&S2, node);
        if (node->left)
            PushStack(&S1, node->left);
        if (node->right)
            PushStack(&S1, node->right);
    }

    while (!IsEmpty(S2)) {
        BinaryTree node = PopStack(&S2);
        printf("%d ", node->data);
    }
}

//This is an alternative implementation of level-order traversal using a static array as a queue
// void LevelOrderTraversal(BinaryTree T) {
//     if (T == NULL) {
//         printf("The tree is empty!\n");
//         return;
//     }
//     BinaryTree queue[100];
//     int front = 0, rear = 0;
//     queue[rear++] = T;
//     while (front < rear) {
//         BinaryTree node = queue[front++];
//         printf("%d ", node->data);
//         if (node->left)
//             queue[rear++] = node->left;
//         if (node->right)
//             queue[rear++] = node->right;
//     }
// }

void InitQueue(LinkQueue* Q) {
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QueueNode));
    if (!Q->front)
        return;
    Q->front->next = NULL;
}

int IsEmpty(LinkQueue Q) {return Q.front == Q.rear;}

void EnQueue(LinkQueue* Q, BinaryTree T) {
    QueuePtr p = (QueuePtr)malloc(sizeof(QueueNode));
    if (!p)
        return;
    p->Data = T;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
}

BinaryTree DeQueue(LinkQueue* Q) {
    if (IsEmpty(*Q)) {
        printf("The queue is empty!\n");
        return NULL;
    }
    QueuePtr p = Q->front->next;
    BinaryTree x = p->Data;
    Q->front->next = p->next;
    
    if (Q->rear == p)
        Q->rear = Q->front;

    free(p);
    printf("%d ", x->data);
    return x;
}

void LevelOrderTraversal(BinaryTree T) {
    if (T == NULL) {
        printf("The tree is empty!\n");
        return;
    }

    LinkQueue Q;
    InitQueue(&Q);
    EnQueue(&Q, T);

    while (!IsEmpty(Q)) {
        BinaryTree node = DeQueue(&Q);
        //process the children of the dequeued node
        if (node->left)
            EnQueue(&Q, node->left);
        if (node->right)
            EnQueue(&Q, node->right);
    }
    printf("\n");
}

int CountHeight(BinaryTree T) {
    if (T == NULL) {
        return 0;
    } else {
        int leftHeight = CountHeight(T->left);
        int rightHeight = CountHeight(T->right);
        return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    }
}

int CountLeaves(BinaryTree T) {
    if (T == NULL) 
        return 0;
    if (T->left == NULL && T->right == NULL)
        return 1;
    return CountLeaves(T->left) + CountLeaves(T->right);
}

int CountNodes(BinaryTree T) {
    if (T == NULL) 
        return 0;
    return CountNodes(T->left) + CountNodes(T->right) + 1;
}

void FreeTree(BinaryTree T) {
    if (T != NULL) {
        FreeTree(T->left);
        FreeTree(T->right);
        free(T);
    }
}

int main() {
    BinaryTree root = CreateBinaryTree();

    printf("Pre-order Traversal: ");
    PreOrderTraversal(root);
    printf("\n");

    printf("Pre-order Traversal (Non-Recursive): ");
    PreOrderTraversalNonRecursive(root);
    printf("\n");

    printf("In-order Traversal: ");
    InOrderTraversal(root);
    printf("\n");

    printf("In-order Traversal (Non-Recursive): ");
    InOrderTraversalNonRecursive(root);
    printf("\n");

    printf("Post-order Traversal: ");
    PostOrderTraversal(root);
    printf("\n");

    printf("Post-order Traversal (Non-Recursive): ");
    PostOrderTraversalNonRecursive(root);
    printf("\n\n");

    printf("Level-order Traversal: ");
    LevelOrderTraversal(root);
    printf("\n");

    int height = CountHeight(root);
    printf("Height of the tree: %d\n", height);

    int leaves = CountLeaves(root);
    printf("Number of leaves: %d\n", leaves);

    int nodes = CountNodes(root);
    printf("Number of nodes: %d\n", nodes);

    FreeTree(root);
    return 0;
}