#include <stdio.h>

#include "gen.h"
#include "level.h"

static int level_counter;

/* Generate a level with a given name. Outputs to a file with .level appended
 * to name
 * TODO: Prim's algorithm?
 */
void gen_level()
{
        int x, y;
        char path[LEVEL_NAME_LIMIT];

        level_get_path(path, level_counter++);
        FILE *f = fopen(path, "w+");

        for (y = 0; y < LEVEL_SIZE; ++y) {
                for (x = 0; x < LEVEL_SIZE; ++x) {
                        if (x < LEVEL_SIZE - 1 && y < LEVEL_SIZE - 1 && x > 0 &&
                            y > 0)
                                fprintf(f, " ");
                        else
                                fprintf(f, "#");
                }
                fprintf(f, "\n");
        }

        fclose(f);
}
