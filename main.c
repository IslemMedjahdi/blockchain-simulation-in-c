#include <stdlib.h>
#include <string.h>
#include "utils.c"
#include "tree.c"
#include <pthread.h>

#define MAX_TRANSACTIONS 2
#define MAX_STRING_LENGTH 256
#define MAX_NODES 10

typedef struct
{
    char data[MAX_STRING_LENGTH];
    char hash[MAX_STRING_LENGTH];
} Transaction;

typedef struct
{
    int index;
    char previous_hash[MAX_STRING_LENGTH];
    Transaction transactions[MAX_TRANSACTIONS];
    int num_transactions;
    char hash[MAX_STRING_LENGTH];
    char merkle_root[MAX_STRING_LENGTH];
    struct Block *previous;
} Block;

typedef struct
{
    Block *lastBlock;
    Block *tempBlock;
    int power;
} BlockchainNode;

Block *createBlock(int index, char *previousHash, Block *previous)
{
    Block *block = (Block *)malloc(sizeof(Block));
    block->index = index;
    block->num_transactions = 0;
    strcpy(block->previous_hash, previousHash);
    strcpy(block->merkle_root, "");
    strcpy(block->hash, "");
    block->previous = previous;
    return block;
}

Transaction *createTransaction(char *data)
{
    Transaction *transaction = (Transaction *)malloc(sizeof(Transaction));
    strcpy(transaction->data, data);
    strcpy(transaction->hash, calculateHash(data));
    return transaction;
}

void calculateMerkleRoot(Block *block)
{
    char *hashes[MAX_TRANSACTIONS];
    for (int i = 0; i < block->num_transactions; i++)
    {
        hashes[i] = block->transactions[i].hash;
    }
    Node *root = constructMerkleTree(hashes, block->num_transactions);
    strcpy(block->merkle_root, root->hash);
}

void addTransactionToBlock(Block *block, Transaction *transaction)
{
    if (block->num_transactions == MAX_TRANSACTIONS)
    {
        printf("Block is full\n");
        return;
    }
    block->transactions[block->num_transactions] = *transaction;
    block->num_transactions++;
    calculateMerkleRoot(block);
    char *hash = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    strcpy(hash, block->previous_hash);
    strcat(hash, block->merkle_root);
    strcpy(block->hash, calculateHash(hash));
}

void printTransaction(Transaction *transaction)
{
    printf("DATA: %s\n", transaction->data);
    printf("Hash: %s\n", transaction->hash);
}

void printBlock(Block *block)
{
    printf("Block %d\n", block->index);
    printf("Previous hash: %s\n", block->previous_hash);
    printf("Merkle root: %s\n", block->merkle_root);
    printf("BLOCK Hash: %s\n", block->hash);
    printf("Transactions:\n");
    for (int i = 0; i < block->num_transactions; i++)
    {
        Transaction *transaction = &block->transactions[i];
        printf("-------------------------\n");
        printTransaction(transaction);
    }
}

void printBlockChain(Block *block)
{
    if (block == NULL)
    {
        return;
    }
    printBlock(block);
    printf("-------------------------\n");
    printf("-------------------------\n");
    printBlockChain(block->previous);
}

int validateBlock(BlockchainNode *nodes)
{
    int validPower = 0;
    int invalidPower = 0;

    for (int i = 0; i < MAX_NODES; i++)
    {
        int randomValidation = rand() % 2;
        if (randomValidation == 1)
        {
            validPower += nodes[i].power;
        }
        else
        {
            invalidPower += nodes[i].power;
        }
    }

    return validPower > invalidPower;
}

void broadcastTransactionToAllNodes(BlockchainNode *nodes, Transaction *transaction)
{
    for (int i = 0; i < MAX_NODES; i++)
    {
        *transaction = *createTransaction(transaction->data);
        addTransactionToBlock(nodes[i].tempBlock, transaction);
    }

    if (nodes[0].tempBlock->num_transactions == MAX_TRANSACTIONS)
    {
        int isBlockValid = validateBlock(nodes);
        if (isBlockValid)
        {
            printf("The Block is Valid and it will be added\n");
            for (int i = 0; i < MAX_NODES; i++)
            {
                nodes[i].lastBlock = nodes[i].tempBlock;
                nodes[i].tempBlock = createBlock(nodes[i].tempBlock->index + 1, nodes[i].tempBlock->hash, nodes[i].tempBlock);
            }
        }
        else
        {
            printf("The Block is not valid and it will be ignored\n");
            for (int i = 0; i < MAX_NODES; i++)
            {
                nodes[i].tempBlock = createBlock(nodes[i].tempBlock->index, nodes[i].tempBlock->previous_hash, nodes[i].tempBlock->previous);
            }
        }
    }
}

void initNodes(BlockchainNode *nodes)
{
    for (int i = 0; i < MAX_NODES; i++)
    {
        nodes[i].lastBlock = NULL;
        nodes[i].tempBlock = createBlock(0, "GENUSIS BLOCK", NULL);
        nodes[i].power = rand() % 100;
    }
}

int main()
{
    BlockchainNode nodes[MAX_NODES];

    initNodes(nodes);

    Transaction *transaction1 = createTransaction("Alice sends 5 bitcoins to Bob");

    broadcastTransactionToAllNodes(nodes, transaction1);

    Transaction *transaction2 = createTransaction("Bob sends 2 bitcoins to Alice");

    broadcastTransactionToAllNodes(nodes, transaction2);

    Transaction *transaction3 = createTransaction("Alice sends 3 bitcoins to Bob");

    broadcastTransactionToAllNodes(nodes, transaction3);

    Transaction *transaction4 = createTransaction("Peter sends 6 bitcoins to Islem");

    broadcastTransactionToAllNodes(nodes, transaction4);

    Transaction *transaction5 = createTransaction("Moncef send 9 bitcoins to Anis");

    broadcastTransactionToAllNodes(nodes, transaction5);

    Transaction *transaction6 = createTransaction("Islem send 4 bitcoins to Anis");

    broadcastTransactionToAllNodes(nodes, transaction6);

    printf("Print Blockchain of node 0\n");
    printBlockChain(nodes[0].lastBlock);
    printf("Print Blockchain of node 1\1");
    printBlockChain(nodes[1].lastBlock);

    return 0;
}
