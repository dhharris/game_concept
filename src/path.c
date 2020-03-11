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

void path_destroy(path *s)
{
        while (s->next) {
                path *tmp = s->next;
                free(s);
                s = tmp;
        }
}

int path_count(path *head)
{
        int count = 0;
        while (head) {
                head = head->next;
                count++;
        }

        return count;
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
        path *next = (*head)->next;
        Vector2 ret = (*head)->position;
        free(*head);
        *head = next;

        return ret;
}
