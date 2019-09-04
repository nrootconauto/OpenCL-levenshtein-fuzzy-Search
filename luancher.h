#ifndef LUANCHER_H
#define LUANCHER_H
#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>
typedef struct {
	cl_context context;
	cl_kernel r2;
	cl_program program;
	cl_kernel r3;
	cl_device_id deviceId;
	cl_platform_id platformId;
} CL_launcher;
CL_launcher CL_fuzzy_load(int p,int d,const char* binaryLocation);
void CL_fuzzy_free(CL_launcher* launcher);
void CL_fuzzy_launch(cl_short* writeTo,char* t,char* p,cl_int n,cl_int m,char* alphabet,CL_launcher* launcher);
#endif