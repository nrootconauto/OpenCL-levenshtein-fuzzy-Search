//https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0186251#pone.0186251.e003
#include "luancher.h"
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
char* loadFile(const char* name) {
	FILE* file=fopen(name,"r");
	fseek(file,0,SEEK_SET);
	size_t start,end;
	start=ftell(file);
	fseek(file,0,SEEK_END);
	end=ftell(file);
	char* retVal=malloc(1+end-start);
	fseek(file,0,SEEK_SET);
	fread(retVal,end-start,1,file);
	retVal[end-start]=0;
	return retVal;
}
//todo make me saucy
int secondIntFactorization(int number,int maxCores) {
	int min=number;
	for(int i=number-1;i!=0;i--) {
		if(number%i==0)
			if((number/i<min)) {
				if(number/i>maxCores)
					break;
				min=number/i;
			}
	}
	return min;
}
void CL_DL_launch(cl_short* writeTo,char* t,char* p,cl_int n,cl_int m,char* alphabet,CL_launcher* launcher) {
	int alphabetSize=strlen(alphabet);
	cl_uint numPlatforms;
	clGetPlatformIDs(-1,NULL,&numPlatforms);
	printf("How Many Platforms:%i\n",(int)numPlatforms);
	cl_platform_id* platforms=alloca(numPlatforms*sizeof(cl_platform_id));
	clGetPlatformIDs(1,platforms,&numPlatforms);
	//for(cl_uint i=0;i!=numPlatforms;i++) {
	cl_int i=0;
	cl_uint devicesCount;
	clGetDeviceIDs(platforms[i],CL_DEVICE_TYPE_ALL,-1,NULL,&devicesCount);
	printf("Devices Count:%i\n",(int)devicesCount);
	cl_device_id device;
	clGetDeviceIDs(platforms[i],CL_DEVICE_TYPE_ALL,1,&device,NULL);
	//create a context
	cl_context context=clCreateContext(NULL,1,&device,NULL,NULL,NULL);
	char* source=loadFile("test.cl");
	size_t lengths=strlen(source);
	cl_program program=clCreateProgramWithSource(context,1,&source,&lengths,NULL);
	clBuildProgram(program,1,&device,NULL,NULL,NULL);
	cl_build_status errorLog;
	clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_STATUS,sizeof(errorLog),&errorLog,NULL);
	size_t logSize;
	clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,0,NULL,&logSize);
	char log[logSize+1];
	clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,logSize,log,NULL);
	log[logSize]=0;
	printf("===[BUILD LOG]===\n%s\n",log);
	cl_kernel hello=clCreateKernel(program,"levensthein",NULL);
	cl_kernel world=clCreateKernel(program,"asm",NULL);
	cl_command_queue line=clCreateCommandQueue(context,device,CL_QUEUE_PROFILING_ENABLE,NULL);
	//make matrices
	size_t size=alphabetSize*(n+1)*sizeof(cl_int);
	cl_int* X=malloc(size);
	memset(X,0,size);
	size_t sizeD=(n+1)*(m+1)*sizeof(cl_int);
	cl_int* D=malloc(sizeD);
	memset(D,0,sizeD);
	cl_char* alpha=malloc(alphabetSize);
	memcpy(alpha,alphabet,alphabetSize);
	cl_mem alphaBuffer=clCreateBuffer(context,CL_MEM_READ_WRITE,alphabetSize,NULL,NULL);
	
	cl_mem xBuffer=clCreateBuffer(context,CL_MEM_READ_WRITE,size,NULL,NULL);
	cl_mem dBuffer=clCreateBuffer(context,CL_MEM_READ_WRITE,sizeD,NULL,NULL);
	clEnqueueWriteBuffer(line,xBuffer,CL_TRUE,0,size,X,0,NULL,NULL);
	clEnqueueWriteBuffer(line,dBuffer,CL_TRUE,0,sizeD,D,0,NULL,NULL);
	clEnqueueWriteBuffer(line,alphaBuffer,CL_TRUE,0,alphabetSize,alpha,0,NULL,NULL);
	cl_mem tBuffer=clCreateBuffer(context,CL_MEM_READ_ONLY,n,NULL,NULL);
	cl_mem pBuffer=clCreateBuffer(context,CL_MEM_READ_ONLY,m,NULL,NULL);
	clEnqueueWriteBuffer(line,tBuffer,CL_TRUE,0,n,t,0,NULL,NULL);
	clEnqueueWriteBuffer(line,pBuffer,CL_TRUE,0,m,p,0,NULL,NULL);
	clSetKernelArg(hello,0,sizeof(cl_mem),&xBuffer);
	clSetKernelArg(hello,1,sizeof(cl_mem),&dBuffer);
	clSetKernelArg(hello,2,sizeof(cl_mem),&alphaBuffer);
	clSetKernelArg(hello,3,sizeof(cl_mem),&pBuffer);
	clSetKernelArg(hello,4,sizeof(cl_mem),&tBuffer);
	clSetKernelArg(hello,5,sizeof(cl_int),&n);
	clSetKernelArg(hello,6,sizeof(cl_int),&m);
	clSetKernelArg(world,0,sizeof(cl_mem),&xBuffer);
	clSetKernelArg(world,1,sizeof(cl_mem),&dBuffer);
	clSetKernelArg(world,2,sizeof(cl_mem),&alphaBuffer);
	clSetKernelArg(world,3,sizeof(cl_mem),&tBuffer);
	clSetKernelArg(world,4,sizeof(cl_mem),&pBuffer);
	clSetKernelArg(world,5,sizeof(cl_int),&n);
	clSetKernelArg(world,6,sizeof(cl_int),&m);
	size_t rowSize=(n+1);
	size_t offset=0;
	size_t rowsSize=rowSize*(alphabetSize); //of ALL rows
	clEnqueueNDRangeKernel(line,hello,1,&offset,&rowsSize,&rowSize,0,NULL,NULL);
	//clEnqueueTask(line,hello,0,NULL,NULL);
	clEnqueueReadBuffer(line,xBuffer,CL_TRUE,0,size,X,0,NULL,NULL);
	//clFinish(line);
	//get biggest divior of the maximum amount of cores that can act in p length parrawels
	//rowSize=m+1;//secondIntFactorization(m+1,launcher->maxCores);
	rowSize=(m+1);
	rowsSize=rowSize*(m+1);
	offset=0;
	while(offset!=(m+1)*(n+1)) {
		clEnqueueNDRangeKernel(line,world,1,&offset,&rowsSize,&rowSize,0,NULL,NULL);
		clFinish(line);
		offset+=m+1;
		rowsSize=(m+1);
	}
	//clEnqueueTask(line,hello,0,NULL,NULL);
	clEnqueueReadBuffer(line,dBuffer,CL_TRUE,0,sizeD,D,0,NULL,NULL);
	printf(" %s\n",t);
	for(int x=0;x!=(m+1);x++) {
		if(x>=1)
			printf("%c:",p[x-1]);
		for(int y=0;y!=n+1;y++)
			printf("%i,",D[y*(m+1)+x]);
		printf("\n");
	}
	clFinish(line);
	
	clReleaseContext(context);
	//}
	clReleaseDevice(device);
}