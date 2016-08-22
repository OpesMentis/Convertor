#include <stdlib.h>
#include <stdio.h>

void init(float* out, int size)
{
    out[0] = size;
}

int main(int argc, char **argv)
{
    const int size = 1000;
    float a[size], b[size];

    init(a,size);
    init(b,size);

    return 0;
}
