#include <stdlib.h> // malloc(), free()

int main()
{
    const int rows = 3, cols = 2;

    // Init the 2d array
    int **twod_array = malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
    {
        twod_array[i] = malloc(sizeof(int) * cols);
    }

    // Free memory
    for (int i = 0; i < rows; i++)
    {
        free(twod_array[i]);
    }
    free(twod_array);

    return 0;
}
