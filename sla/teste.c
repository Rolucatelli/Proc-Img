#include <stdio.h>
#include <stdarg.h>

void pf(char *str, ...){

    va_list args;
    va_start(args, str);
    vprintf(str, args);

}

int main()
{
    pf("Hello World! %s %s %s\n", "Rodrigo", "Luis", "Gasparino Lucatelli");
    pf("Testando %s %s\n", "Um", "Dois");


    return 0;
}







