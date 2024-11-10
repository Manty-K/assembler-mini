#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "symb.h"

typedef struct node
{

    ENTRY *entry;
    struct node *left;
    struct node *right;

} NODE;

NODE *root = NULL;

ENTRY *getEntry(char *label)
{
    NODE *current = root;
    while (current != NULL)
    {
        int cmp = strcmp(current->entry->label, label);
        if (cmp == 0)
        {
            return current->entry;
        }
        else if (cmp < 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return NULL;
}

void addLabel(char *label, long location, char section)
{

    if (root == NULL)
    {

        ENTRY *e = malloc(sizeof(ENTRY));
        e->label = malloc(sizeof(label));
        strcpy(e->label, label);
        e->location = location;
        e->section = section;
        NODE *n = malloc(sizeof(NODE));
        n->entry = e;
        n->left = n->right = NULL;
        root = n;
    }
    else
    {

        if (getEntry(label))
        {
            puts("Label Exists");
            return;
        }

        NODE *current = root;
        NODE *parent = NULL;
        while (current != NULL)
        {
            parent = current;
            int cmp = strcmp(current->entry->label, label);
            if (cmp < 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        ENTRY *e = malloc(sizeof(ENTRY));
        e->label = malloc(sizeof(label));
        strcpy(e->label, label);
        e->location = location;
        e->section = section;
        NODE *n = malloc(sizeof(NODE));
        n->entry = e;
        n->left = n->right = NULL;
        if (strcmp(parent->entry->label, label) < 0)
        {
            parent->left = n;
        }
        else
        {
            parent->right = n;
        }
    }
}

void inorder(NODE *node)
{

    if (node == NULL)
    {
        return;
    }
    inorder(node->left);
    printf("%08lx\t%s\t\t%c\n", node->entry->location, node->entry->label, node->entry->section);
    inorder(node->right);
}
void displaySymbolTable()
{
    puts("Symbol Table");
    puts("Loc\t\tLabel\t\tSection");
    puts("--------------------------------------");
    inorder(root);
}
