#ifndef LUANCHER_H
#define LUANCHER_H
#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>
typedef struct {
	int maxCores;
} CL_launcher;
void CL_DL_launch(cl_short* writeTo,char* t,char* p,cl_int n,cl_int m,char* alphabet,CL_launcher* launcher);
#endif