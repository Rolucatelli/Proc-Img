#include <stdio.h>
#include <stdlib.h>

int main(){

    unsigned int px = 0x0058bc30;
    int R = 0, G = 0, B = 0;
    R = ((px >> 16) & 0Xff);
    G = (px >> 8) & 0Xff;
    B = (px) & 0Xff;
    
    system("clear");
    printf("Color:%X %d\n", px, px);
    printf("R:%X %d\nG:%X %d\nB:%X %d\n", R, R, G, G, B, B);
    return 0;
}