#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "symb.h"

extern int pass;

typedef struct node
{

    ENTRY *entry;
    struct node *left;
    struct node *right;

} NODE;

NODE *root = NULL;

ENTRY *getEntry(char *label)
{
    if (pass != 2)
    {
        return NULL;
    }

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

long getLblLoc(char *label)
{
    ENTRY *en = getEntry(label);
    if (en != NULL)
    {
        return en->location;
    }
    return -1;
}

void addLabel(char *label, long location, char section, long value)
{
    // if (pass != 1)
    // {
    //     return;
    // }

    if (root == NULL)
    {

        ENTRY *e = malloc(sizeof(ENTRY));
        e->label = malloc(sizeof(label));
        strcpy(e->label, label);
        e->location = location;
        e->section = section;
        e->value = value;
        NODE *n = malloc(sizeof(NODE));
        n->entry = e;
        n->left = n->right = NULL;
        root = n;
    }
    else
    {

        if (getEntry(label))
        {
            // puts("Label Exists");
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
        e->value = value;
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
    printf("%08lx\t%s\t\t%c\t%ld\n", node->entry->location, node->entry->label, node->entry->section, node->entry->value);
    inorder(node->right);
}
void displaySymbolTable()
{
    puts("Symbol Table");
    puts("Loc\t\tLabel\t\tSection\t\tValue");
    puts("--------------------------------------");
    inorder(root);
}

void inorderS(NODE *node, FILE *fp, char *buffer, size_t sizeOfBuffer)
{

    if (node == NULL)
    {
        return;
    }

    inorderS(node->left, fp, buffer, sizeOfBuffer);

    memset(buffer, 0, sizeof(sizeOfBuffer));
    sprintf(buffer, "%08lx\t%s\t%c\t%08X\n", node->entry->location, node->entry->label, node->entry->section, node->entry->value != NULL ? node->entry->value : 0);
    fputs(buffer, fp);

    inorderS(node->right, fp, buffer, sizeOfBuffer);
}

void saveSymbolTable()
{
    FILE *fp = fopen("p.sym", "w");
    char *buffer = malloc(100);
    inorderS(root, fp, buffer, 100);
    fclose(fp);
    free(buffer);
}

void importSymbolTable()
{
    FILE *fp = fopen("p.sym", "r");
    if (fp == NULL)
    {
        perror("import symbol");
        return;
    }

    char *buffer = malloc(100);
    char *lbl = NULL;
    long hex;
    char sec;
    long value = 0;

    while (fgets(buffer, 100, fp) != NULL)
    {
        char *token = strtok(buffer, "\t");
        hex = strtol(token, NULL, 16);
        token = strtok(NULL, "\t");
        if (lbl)
            free(lbl);
        lbl = strdup(token);

        token = strtok(NULL, "\t");
        sec = token[0];

        addLabel(lbl, hex, sec, value); // TODO: Fix Value Import
    }
    // displaySymbolTable();

    free(buffer);
    if (lbl)
        free(lbl);
    fclose(fp);
    // puts("Imported");
}
