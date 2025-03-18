#include <stdio.h>

int nc = 10;
int nl = 10;

int img[] = {
    0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,
};

void showImage(int img[])
{
    for (int i = 0; i < nl; i++)
    {
        printf("\t");
        for (int j = 0; j < nc; j++)
        {
            printf("%3d ", img[i * nc + j]);
        }
        printf("\n");
    }
    printf("\n\n\n");

}

int lessOfThree(int a, int b, int c)
{

    if (a<b && a < c)
    {
        return a;
    } else {
        if (b<c)
        {
            return b;
        }
        return c;
    }
    
}

void distance(int img[])
{
    for (int i = 1; i < nl; i++)
    {
        for (int j = 1; j < nc; j++)
        {
            int a = (i - 1) * nl + j;
            int b = i * nl + j - 1;
            int p = i * nl + j;

            if (img[p] != 0)
            {
                if (img[a]+1 < img[b]+1)
                {
                    img[p] = img[a] + 1;
                }
                else
                {
                    img[p] = img[b] + 1;
                }
            }
        }
    }

    for (int i = nl - 2; i >= 0; i--)
    {
        for (int j = nc - 2; j >= 0; j--)
        {
            int a = i * nl + j + 1;
            int b = (i + 1) * nl + j;
            int p = i * nl + j;

            img[p] = lessOfThree(img[a] + 1, img[b] + 1, img[p]);
        }
    }
}

int main()
{
    char tmp;
    system("clear");
    printf("\n\n\n");

    showImage(img);

    distance(img);
    showImage(img);

    scanf("%c", &tmp);
    return 0;
}
