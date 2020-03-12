#include <stdlib.h>

#include "raymath.h"

#include "helpers.h"
#include "path.h"

path *path_new()
{
        path *head = malloc(sizeof(path));
        head->position = VECTOR2_NULL;
        head->next = NULL;
        return head;
}

void path_destroy(path *p)
{
        // No-op if null
        if (!p) {
                return;
        }
        while (p->next) {
                path *tmp = p->next;
                free(p);
                p = tmp;
        }
}

int path_empty(path *head)
{
        return Vector2Eq(head->position, VECTOR2_NULL);
}

void path_push(path *head, Vector2 position)
{
        if (Vector2Eq(head->position, VECTOR2_NULL)) {
                head->position = position;
                return;
        }
        path *cur = head;
        while (cur->next) cur = cur->next;

        cur->next = malloc(sizeof(path));
        cur->next->position = position;
        cur->next->next = NULL;
}

void path_push_front(path **head, Vector2 position)
{
        path *new_head = path_new();
        new_head->position = position;
        new_head->next = *head;
        *head = new_head;
}

Vector2 path_pop(path **head)
{
        if (!(*head))
                return VECTOR2_NULL;
        if (!(*head)->next) {
                Vector2 tmp = (*head)->position;
                (*head)->position = VECTOR2_NULL;
                return tmp;
        }
        path *next = (*head)->next;
        Vector2 ret = (*head)->position;
        free(*head);
        *head = next;

        return ret;
}
