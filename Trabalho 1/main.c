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
#include "./utils/imagelib.h"

typedef struct
{
    int nr, nc, ml; // nr = n.rows, nc = n.columns, ml = max level
    int *hg;        // histogram vector
    int *imgE;      // image encoded
} *pgh;

/*-------------------------------------------------------------------------
 * Read pgh ascii image
 * Params (in):
 *   name = image file name
 * Returns:
 *   image structure
 *-------------------------------------------------------------------------+*/
image read_pgh(char *name)
{
    char lines[100];
    int nr, nc, ml;
    pgh img;
    FILE *fimg;
    ERROR((fimg = fopen(name, "r")) == NULL, errormsg("File open error: <%s>", name));
    /*--- PNM = "P1" or "P2" or "P3" ---*/
    fgets(lines, 80, fimg);
    ERROR((lines[0] != 'P' || (lines[1] != 'H')), errormsg("File type image error: <%s>", name));
    /*--- Comment lines ---*/
    fgets(lines, 80, fimg);
    while (strchr(lines, '#'))
        fgets(lines, 80, fimg);
    sscanf(lines, "%d %d", &nc, &nr);

    fscanf(fimg, "%d", &ml);

    ERROR(nc == 0 || nr == 0 || ml == 0, errormsg("Image dimensions error: <%s>", name));
    // img = img_create(nr, nc, ml, 4);
    img->nr = nr;
    img->nc = nc;
    img->ml = ml;
    img->hg = malloc((1 + ml) * sizeof(int));

    // ERROR(!img, errormsg("Image allocation error: %s\n\n img_get routine", name));
    for (int i = 0; i <= ml; i++)
    {
        int k;
        fscanf(fimg, "%x", &k);
        img->hg[i] = k;
    }

    fclose(fimg);
    img_info(name, img);
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

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In;
    if (argc < 2)
        msg(argv[0]);

    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, PGH, GRAY);

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