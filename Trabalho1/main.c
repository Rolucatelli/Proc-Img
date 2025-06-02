/*=============================================================
 *             UNIFAL - Universidade Federal de Alfenas.
 *               BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: Decodificador do formato PGH
 * Professor.: Luiz Eduardo da Silva
 * Aluno.....: Rodrigo Luís Gasparino Lucatelli
 * Data......: 09/06/2025
 *=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "./utils/imagelib.h"

typedef struct
{
    int nr, nc, ml; // nr = n.rows, nc = n.columns, ml = max level
    int *hg;        // histogram vector
    int *imgE;      // image encoded
} *pgh;

typedef struct huf
{
    int freq, father, left, right;
} nodetype;

int pqmindelete(nodetype *node, int *rootnodes)
{
    int aux;
    aux = *rootnodes;
    *rootnodes = node[*rootnodes].father;
    return aux;
}

int pqinsert(nodetype *node, int *rootnodes, int i)
{
    int k, ant, aux;
    aux = *rootnodes;
    ant = -1;
    while (aux != -1 && node[aux].freq <= node[i].freq)
    {
        ant = aux;
        aux = node[aux].father;
    }
    if (ant == -1)
    {
        node[i].father = *rootnodes;
        *rootnodes = i;
    }
    else
    {
        node[i].father = node[ant].father;
        node[ant].father = i;
    }
}

/*--------------------------------------------------------------
 * Mostra a estrutura de nos (Arvore + lista ordenada)
 *--------------------------------------------------------------*/
void shownodes(nodetype *node, int root, int n)
{
    int i;
    printf("\n\nRoot=%2d", root);
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", i);
    printf("\n       ");
    for (i = 0; i < 2 * n - 1; i++)
        printf("-----");
    printf("\n%7s", "freq");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].freq);
    printf("\n%7s", "father");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].father);
    printf("\n%7s", "left");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].left);
    printf("\n%7s", "right");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].right);
    printf("\n\nLista = ");
    while (root != -1)
    {
        printf("%d:[%d|%d]=> ", root, node[root].freq, node[root].father);
        root = node[root].father;
    }
    printf("\n");
}

/*--------------------------------------------------------------
 * buildTree
 * Funcao que constroi a arvore de huffman
 * Parametros:
 *    h  - vetor de histograma (frequencia de cada pixel [0..mn])
 *    mn - maximo nivel de cinza
 * Retorna a raiz da árvore construida (2 * mn)
 *--------------------------------------------------------------*/
int buildTree(nodetype *node, int *h, int mn)
{
    int i, k, p, p1, p2, rootnodes = -1, root, father;
    for (i = 0; i < mn + 1; i++)
    {
        node[i].freq = h[i];
        node[i].left = -1;
        node[i].right = -1;
        pqinsert(node, &rootnodes, i);
    }
    // shownodes(node, rootnodes, mn + 1);
    for (p = mn + 1; p < 2 * (mn + 1) - 1; p++)
    {
        p1 = pqmindelete(node, &rootnodes);
        p2 = pqmindelete(node, &rootnodes);
        node[p1].father = p;
        node[p2].father = p;
        node[p].freq = node[p1].freq + node[p2].freq;
        node[p].left = p1;
        node[p].right = p2;
        pqinsert(node, &rootnodes, p);
        // shownodes(node, rootnodes, mn + 1);
    }
    puts("\n");
    return pqmindelete(node, &rootnodes);
}

/*-------------------------------------------------------------------------
 * Read pgh binary image
 * Params (in):
 *   name = image file name
 * Returns:
 *   image structure
 *-------------------------------------------------------------------------+*/
image read_pgh(char *name)
{

    char lines[100];
    int nr, nc, ml;
    pgh imgIn = malloc(sizeof(pgh));
    FILE *fimg;
    ERROR((fimg = fopen(name, "rb")) == NULL, errormsg("File open error: <%s>", name));
    /*--- PNM = "P1" or "P2" or "P3" ---*/
    fgets(lines, 80, fimg);
    ERROR((lines[0] != 'P' || (lines[1] != 'H')), errormsg("File type image error: <%s>", name));
    /*--- Comment lines ---*/
    fgets(lines, 80, fimg);
    while (strchr(lines, '#'))
        fgets(lines, 80, fimg);
    sscanf(lines, "%d %d", &nc, &nr);

    fgets(lines, 80, fimg);
    sscanf(lines, "%d", &ml);

    ERROR(nc == 0 || nr == 0 || ml == 0, errormsg("Image dimensions error: <%s>", name));
    imgIn->nr = nr;
    imgIn->nc = nc;
    imgIn->ml = ml;

    int *histograma = malloc((1 + ml) * sizeof(int));

    fread(histograma, sizeof(int), ml + 1, fimg);

    // printf("Histograma:\n");
    // for (int i = 0; i <= ml; i++)
    // {
    //     printf("  Tom %d: %d\n", i, histograma[i]);
    // }
    imgIn->hg = histograma;

    int newsize = (2 * (ml + 1)) - 1;
    nodetype *node = calloc(newsize, sizeof(nodetype));

    int root = buildTree(node, histograma, ml);

    image img = img_create(nr, nc, ml, 2);

    int byte;
    int aux = root, j = 0;
    while ((byte = fgetc(fimg)) != EOF)
    {
        for (int i = 7; i >= 0; i--)
        {
            int bit = (byte >> i) & 1;
            if (bit)
            {
                aux = node[aux].right;
            }
            else
            {
                aux = node[aux].left;
            }
            if (node[aux].left == -1 && node[aux].right == -1)
            {
                img->px[j++] = aux;
                aux = root;
            }
        }
    }

    fclose(fimg);
    free(imgIn);
    return img;
}

image img_build(int nr, int nc, int ml, int tp)
{
    image img = malloc(sizeof(image *));
    img->px = malloc(nr * nc * sizeof(int));
    img->nr = nr;
    img->nc = nc;
    img->ml = ml;
    img->tp = tp;
    return img;
}

void msg(char *s)
{
    printf("\nDecoding files in PGH format ");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgh]\n\n", s);
    printf("    image-name[.pgh] is image file in pgh format \n\n");
    exit(1);
}

void name_manager(char *name, char *in, char *out)
{
    char *p = strstr(name, ".pgh");
    if (p)
        *p = 0;
    sprintf(in, "%s%s", name, ".pgh");
    sprintf(out, "%s%s", name, "-result.pgm");
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In;
    if (argc < 2)
        msg(argv[0]);

    name_manager(argv[1], nameIn, nameOut);

    //-- read image PGH
    In = read_pgh(nameIn);

    //-- save image PGM
    img_put(In, nameOut, GRAY);

    //-- show image
    sprintf(cmd, "eog %s &", nameOut);
    puts(cmd);
    system(cmd);
    img_free(In);

    return 0;
}