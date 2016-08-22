#include <stdlib.h>
#include <stdio.h>

void init(float* out, int size)
{
    int i;
    for (i=0; i<size; i++)
        out[i] = i+1.7;
}

int main(int argc, char **argv)
{
    int size = 1000;

    float* a = (float*)malloc(size*sizeof(float));
    float* b = (float*)malloc(size*sizeof(float));

    init(a,size);
    init(b,size);

    free(a);
    free(b);

    return 0;
}
