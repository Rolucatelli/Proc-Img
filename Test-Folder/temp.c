#include <stdio.h>
#include <stdlib.h>

void main()
{
    FILE *arq = fopen("ts.txt", "w");
    for (int j = 0; j < 256; j++)
    {
        fprintf(arq, "\n");
        for (int i = 0; i < 256; i++)
        {
            fprintf(arq, "%d ", i);
        }
    }
    fclose(arq);
}