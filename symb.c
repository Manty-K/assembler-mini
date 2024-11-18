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

void addLabel(char *label, long location, char section)
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

void inorderS(NODE *node, FILE *fp, char *buffer)
{

    if (node == NULL)
    {
        return;
    }

    inorderS(node->left, fp, buffer);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%08lx\t%s\t%c\n", node->entry->location, node->entry->label, node->entry->section);
    fputs(buffer, fp);

    inorderS(node->right, fp, buffer);
}

void saveSymbolTable()
{
    FILE *fp = fopen("p.sym", "w");
    char *buffer = malloc(100);
    inorderS(root, fp, buffer);
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

        addLabel(lbl, hex, sec);
    }
    // displaySymbolTable();

    free(buffer);
    if (lbl)
        free(lbl);
    fclose(fp);
    // puts("Imported");
}
