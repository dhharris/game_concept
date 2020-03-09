#include <stdlib.h>

#include "raymath.h"

#include "helpers.h"
#include "path.h"

path *path_new()
{
        path *head = malloc(sizeof(path));
        head->position = Vector2Zero();
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
        // FIXME: Doesn't work if the head node has position {0,0}
        if (Vector2Eq(head->position, Vector2Zero())) {
                head->position = position;
                return;
        }
        path *cur = head;
        while (cur->next) cur = cur->next;

        cur->next = malloc(sizeof(path));
        cur->next->position = position;
        cur->next->next = NULL;
}

Vector2 path_pop(path **head)
{
        if (!(*head))
                return Vector2Zero(); // TODO: different falsy value
        path *next = (*head)->next;
        Vector2 ret = (*head)->position;
        free(*head);
        *head = next;

        return ret;
}
