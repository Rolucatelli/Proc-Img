/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Processing of the image negative
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#include "..\\utils\\imagelib.h"
#elif defined(__linux__)
#include "../utils/imagelib.h"
#endif

image eq(image img)
{

    int w = img->nc;
    int h = img->nr;
    int max = img->ml;
    int *pixels = img->px;

    float n[max + 1];
    float pr[max + 1];
    int T[max + 1];

    // Cauculo do histograma
    for (int i = 0; i < max + 1; i++)
        n[i] = 0;
    for (int i = 0; i < h * w; i++)
        n[pixels[i]]++;

    // Probabilidade das intensidades acumulada
    pr[0] = n[0] / (w * h);
    T[0] = pr[0] * max;
    for (int i = 1; i < max + 1; i++)
    {
        pr[i] = pr[i - 1] + n[i] / (w * h);
        T[i] = pr[i] * max;
    }

    // Aplicação da função na imagem
    for (int i = 0; i < w * h; i++)
        pixels[i] = T[pixels[i]];

    return img;
}

void msg(char *s)
{
    printf("\nEqualizacao de histograma");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm]\n\n", s);
    printf("    image-name[.pgm] is image file in pgm format \n\n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);

    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = eq(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    puts(cmd);
    system(cmd);
    img_free(In);
    // img_free(Out);
    return 0;
}
