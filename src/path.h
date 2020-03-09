#pragma once

#include "common.h"

typedef struct path_node {
        Vector2 position;
        unsigned distance; /* Distance to dst */
        struct path_node *next;
} path; /* Defines a path as a linked list */


/* Function interface to path */
path *path_new();
void path_destroy(path *s);
void path_push(path *head, Vector2 position);
Vector2 path_pop(path **head);
int path_count(path *head);
