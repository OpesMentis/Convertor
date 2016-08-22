#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int size = 1000;
    int i;

    float* a = (float*)malloc(size*sizeof(float));
    float* b = (float*)malloc(size*sizeof(float));


    for (i=0; i<size; i++)
        a[i] = i+1.7;

    for (i=0; i<size; i++)
        b[i] = i+1.7;

    free(a);
    free(b);

    return 0;
}
