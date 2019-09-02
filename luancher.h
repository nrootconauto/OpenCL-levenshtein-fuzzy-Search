#ifndef LUANCHER_H
#define LUANCHER_H
#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>
void CL_DL_launch(cl_short* writeTo,char* t,char* p,cl_int n,cl_int m,int alphabetSize);
#endif