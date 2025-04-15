/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Processing of the image negative
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#include "..\\utils\\imagelib.h"
#elif defined(__linux__)
#include "../utils/imagelib.h"
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef double complex cpx;

cpx W(double u, double m)
{
    cpx temp;
    double exp = 2 * M_PI / m;
    
    return;
}

int testPowerof2(int n)
{
    unsigned int m, m2;
    for (m = n, m2 = 1; m != 1; m >>= 1, m2 <<= 1)
        ;
    if (m2 != n)
    {
        printf("A dimensão %d não é potência de 2!\n", n);
        exit(10);
    }
}

unsigned int bitReverso(unsigned int x, int log2n)
{
    int n = 0;
    for (int i = 0; i < log2n; i++)
    {
        n <<= 1;
        n |= (x & 1);
        x >>= 1;
    }
    return n;
}




void msg(char *s)
{
    printf("\nFilters");
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
    // Out = mediana(In);

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
