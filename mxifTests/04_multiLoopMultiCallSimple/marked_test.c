/**
 * @file		basic.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		26-05-15
 * @copyright  none
 * @brief		basic example
 */
#include <stdlib.h>
#include <stdio.h>
/**
 * init vector
 */


void init(float *out,int size)
{
  int i;
  for (i = 0; i < size; i++) 
    out[i] = ((float )i);
}
/**
 * add kernel
 */

void add(float *out,float *in1,float *in2,int size)
{
  int i;
  for (i = 0; i < size; i++) 
    out[i] = in1[i] + in2[i];
}
/**
 * substract kernel
 */

void sub(float *out,float *in1,float *in2,int size)
{
  int i;
  for (i = 0; i < size; i++) 
    out[i] = in1[i] - in2[i];
}
/**
 * multiplication kernel
 */

void mul(float *out,float *in1,float *in2,int size)
{
  int i;
  for (i = 0; i < size; i++) 
    out[i] = in1[i] * in2[i];
}
/**
 * main
 */

int main(int argc,char **argv)
{
  
  int size = 10000;
  float *a = (float *)(malloc(size * sizeof(float )));
  float *b = (float *)(malloc(size * sizeof(float )));
  float *c = (float *)(malloc(size * sizeof(float )));
  float *d = (float *)(malloc(size * sizeof(float )));
  float *e = (float *)(malloc(size * sizeof(float )));
  init(b,size);
  init(c,size);
  init(e,size);
// a = b + c
  add(a,b,c,size);
// d = c + a
  mul(d,c,a,size);
// e = b + a
  sub(e,b,a,size);
// a = e + d
  add(a,e,d,size);
  free(a);
  free(b);
  free(c);
  free(d);
  free(e);
  
  return 0;
}
