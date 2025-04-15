#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#include "..\\utils\\imagelib.h"
#elif defined(__linux__)
#include "../utils/imagelib.h"
#endif

#define MAXNODES 511

/*
 * Root = 3
 * ----------------------------------------------------------------
 *              0       1       2       3       4       5       6
 * ----------------------------------------------------------------
 * freq     |   3       8       11      1       4       0       0
 * father   |   1       2       -1      0       0       0       0
 * left     |   0       0       0       0       0       0       0
 * right    |   0       0       0       0       0       0       0
 *
 * Lista => 3:[1|0] => 0:[3|1] => 1:[8|2] => 2:[11|-1]
 *
 * ----------------------------------------------------------------
 *              0       1       2       3       4       5       6
 * ----------------------------------------------------------------
 * freq     |   3       8       11      1       4       12       23
 * father   |   4       5       6       4       5       6       -1
 * left     |   -1      -1      -1      -1      3       4        2
 * right    |   -1      -1      -1      -1      0       1        5
 *
 * Lista => 6:[23|-1]
 */

typedef struct huf
{
    int freq, father, left, right;
} nodetype;

void huffman(nodetype *lista, int tam)
{
}

nodetype *histograma(image in)
{

    int w = in->nc;
    int h = in->nr;
    int max = in->ml;

    int hist[max + 1];

    // Cauculo do histograma
    for (int i = 0; i < max + 1; i++)
        hist[i] = 0;
    for (int i = 0; i < h * w; i++)
        hist[in->px[i]]++;

    int tamTabela = max + max + 1;

    nodetype *tabela = calloc(tamTabela, sizeof(nodetype));

    for (int i = 0; i < max; i++)
    {
        tabela[i].freq = hist[i];
    }

    int k = 0, menor = tabela[0].freq;
    for (int i = 0; i < max; i++)
    {
        for (int j = 0; j < max; j++)
        {
            if (tabela[j].freq < tabela[menor].freq)
            {
                menor = j;
            }
        }
        // Não ta completo
        tabela[menor].father=i;
    }

    return tabela;
}

int pqMinDel(int *sla)
{
    int aux;
    // aux = *sla[0].father
}

int main()
{

    return 0;
}