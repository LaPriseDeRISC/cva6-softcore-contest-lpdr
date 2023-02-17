#include "malloc_structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MDS_TABLE_SIZE 100000

mds_entry _mds_table[MDS_TABLE_SIZE];
mds_entry *_mds_head = NULL;
unsigned int _mds_table_index = 0;

// #define DEBUG

void mds_insert(mds_entry *entry)
{
#ifdef DEBUG
    printf("Inserting %ld of size %ld : ", (long int)entry->address, (long int)entry->size);
#endif
    entry->left = NULL;
    entry->right = NULL;

    if (_mds_head == NULL)
    {
        _mds_head = entry;
        return;
    }

    mds_entry *current = _mds_head;

    while (current != NULL)
    {
        if (entry->address <= current->address)
        {
#ifdef DEBUG
            printf("left ");
#endif
            if (current->left == NULL)
            {
#ifdef DEBUG
                printf("insertion complete\n");
#endif
                current->left = entry;
                return;
            }
            current = current->left;
        }
        else
        {
#ifdef DEBUG
            printf("right ");
#endif
            if (current->right == NULL)
            {
#ifdef DEBUG
                printf("insertion complete\n");
#endif
                current->right = entry;
                return;
            }
            current = current->right;
        }
    }

#ifdef DEBUG
    printf("insertion failure !\n");
#endif
    // todo
}

void mds_remove(mds_entry *entry)
{
    // todo
}

mds_entry *mds_register(__ptr_t address, size_t size)
{
    mds_entry *entry = &_mds_table[_mds_table_index];
#ifdef DEBUG
    printf("Registering %ld of size %ld using index %d\n", (long int)address, size, _mds_table_index);
#endif
    _mds_table_index++;
    entry->address = address;
    entry->size = size;
    mds_insert(entry);
    return entry;
}

void mds_unregister(__ptr_t address)
{
    // todo
}

void _mds_dump_rec(mds_entry *entry, char *buffer)
{
    if (entry == NULL)
        return;

    if (entry->left != NULL)
    {
        sprintf(buffer, "%s    \"%ld\" -> \"%ld\" [color=red];\n", buffer, (long int)entry->address, (long int)entry->left->address);
        _mds_dump_rec(entry->left, buffer);
    }
    if (entry->right != NULL)
    {
        sprintf(buffer, "%s    \"%ld\" -> \"%ld\" [color=blue];\n", buffer, (long int)entry->address, (long int)entry->right->address);
        _mds_dump_rec(entry->right, buffer);
    }
}

void mds_dump(char *buffer)
{
    // char *buffer[4096];
    mds_entry *current = _mds_head;
    sprintf(buffer, "digraph G {\n");
    _mds_dump_rec(current, buffer);
    sprintf(buffer, "%s}\n", buffer);
}

mds_entry *mds_get(__ptr_t address)
{
    mds_entry *current = _mds_head;
    while (current != NULL)
    {
        if (address == current->address)
            return current;
        else if (address < current->address)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

int mds_check_all()
{
    return mds_check(_mds_head);
}

int mds_check(mds_entry *entry)
{
    if (entry == NULL)
        return 0;

    if (entry->left != NULL)
    {
        if (entry->left->address > entry->address)
        {
            printf("left address %p is not less than %p", (long int)entry->left->address, (long int)entry->address);
            return 0;
        }
        if (mds_check(entry->left) == 0)
            return 0;
    }

    if (entry->right != NULL)
    {
        if (entry->right->address <= entry->address)
        {
            printf("right address %ld is not greater than %ld", (long int)entry->right->address, (long int)entry->address);
            return 0;
        }

        if (mds_check(entry->right) == 0)
            return 0;
    }

    return 1;
}