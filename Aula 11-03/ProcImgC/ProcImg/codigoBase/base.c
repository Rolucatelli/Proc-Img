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

int abs(int n)
{
    return n < 0 ? -n : n;
}

image sobel(image img)
{
    image out = img_clone(img);
    int w = img->nc;
    int h = img->nr;
    int max = img->ml;
    int *pixels = img->px;

    // Inicializando a primeira e a ultima coluna da imagem de saida
    for (int i = 0; i < h; i++)
    {
        out->px[i * w] = 0;
        out->px[i * w + w - 1] = 0;
    }
    // Inicializando a primeira e a ultima linha da imagem de saida
    for (int j = 0; j < w; j++)
    {
        out->px[j] = 0;
        out->px[(h - 1) * w + j] = 0;
    }

    for (int i = 1; i < h - 1; i++)
        for (int j = 0; j < w - 1; j++)
        {
            int soma, Z[9];
            int k = 0;
            // Calculando a mascara que vai ser aplicada ao pixel
            for (int y = -1; y < 2; y++)
                for (int x = -1; x < 2; x++)
                {
                    Z[k++] = pixels[(i + y) * w + (j + x)];
                }
            soma = abs((Z[6] + 2 * Z[7] + Z[8]) - (Z[0] + 2 * Z[1] + Z[2])) +
                   abs((Z[2] + 2 * Z[5] + Z[8]) - (Z[0] + 2 * Z[3] + Z[6]));
            // Aplicando o resultado a imagem de saida
            out->px[i * w + j] = soma > 168 ? 0 : max;
        }
    return out;
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void sort(int v[], int n){
    for (int i = 0; i < n-1; i++)
    {
        int m = i;
        for (int j = i + 1; j < n; j++)
        {
            if (v[j] < v[m])
            {
                m = j;
            }
            
        }
        int tmp = v[m];
        v[m] = v[i];
        v[i] = tmp;
    }    
}

image mediana(image img)
{
    image out = img_clone(img);
    int w = img->nc;
    int h = img->nr;
    int max = img->ml;
    int *pixels = img->px;

    // Inicializando a primeira e a ultima coluna da imagem de saida
    for (int i = 0; i < h; i++)
    {
        out->px[i * w] = 0;
        out->px[i * w + w - 1] = 0;
    }
    // Inicializando a primeira e a ultima linha da imagem de saida
    for (int j = 0; j < w; j++)
    {
        out->px[j] = 0;
        out->px[(h - 1) * w + j] = 0;
    }

    for (int i = 1; i < h - 1; i++)
        for (int j = 0; j < w - 1; j++)
        {
            int Z[9];
            int k = 0;
            // Calculando a mascara que vai ser aplicada ao pixel
            for (int y = -1; y < 2; y++)
                for (int x = -1; x < 2; x++)
                {
                    Z[k++] = pixels[(i + y) * w + (j + x)];
                }
            // Aplicando o resultado a imagem de saida
            qsort(Z,9,sizeof(int),compare);
            // sort(Z,9);
            out->px[i * w + j] = Z[4];
        }
    return out;
}

void msg(char *s)
{
    printf("\n Filters");
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

    // Out = eq(In);
    // Out = sobel(In);
    Out = mediana(In);


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
