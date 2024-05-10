#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *hash;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(char *hash)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->hash = hash;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node *constructMerkleTree(char **hashes, int numHashes)
{
    Node **nodes = malloc(numHashes * sizeof(Node *));
    for (int i = 0; i < numHashes; i++)
    {
        nodes[i] = createNode(hashes[i]);
    }
    int numNodes = numHashes;
    while (numNodes > 1)
    {
        int j = 0;
        for (int i = 0; i < numNodes; i += 2)
        {
            Node *left = nodes[i];
            Node *right = (i + 1 < numNodes) ? nodes[i + 1] : nodes[i];
            char *combinedHash = malloc(strlen(left->hash) + strlen(right->hash) + 1);
            strcpy(combinedHash, left->hash);
            strcat(combinedHash, right->hash);
            combinedHash = calculateHash(combinedHash);
            Node *parent = createNode(combinedHash);
            parent->left = left;
            parent->right = right;
            nodes[j++] = parent;
        }
        numNodes = j;
    }
    Node *root = nodes[0];
    free(nodes);
    return root;
}
