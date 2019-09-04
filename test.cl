typedef unsigned char charType;
#define access(X,Y) (Y)*(m+1)+(X)
int r2(global int* matrix,global charType* t,global charType* p,global charType* alpha,int i,int j,int n,int m) {
	if(i==0) {
		return 0;
	}
	//make sure in bounds
	if((i-1>=0)) {
		if(p[i-1]==alpha[j]) {
			return i;
		}
	}
	return *(matrix+(j)*(n+1)+(i-1));
}
int r3(global int* D,global int* X,global charType* alpha,global charType*  t,global charType* p,int i,int j,int n,int m) {
	if(j==0) {
		return i;
	}
	if(i==0) {
		return 0;
	}
        if(t[j-1]==p[i-1]) {
		return *(D+access(i-1,j-1));
	}
	if(true) {
		int l=0;
		for(;p[i-1]!=alpha[l];l++);
		int xlj=*(X+l*(n+1)+j);
                if(0==xlj) {
			int min1=*(D+access(i-1,j));
			int min2=*(D+access(i-1,j-1));
			int min3=i+j-1;
			min2=(min1<min2)?min1:min2;
			int min4=(min2<min3)?min2:min3;
			return 1+min4;
		}
		//else
		int min1=*(D+access(i-1,j));
                int min2=*(D+access(i-1,j-1));
		int min3=*(D+access(i-1,xlj-1))+(j-1-xlj);
		min2=(min1<min2)?min1:min2;
		int min4=(min2<min3)?min2:min3;
                return 1+min4;
	}
	return -1;
}
__kernel void levensthein(global int* X,global int* D,global charType* alpha,global charType* T,global charType* P,int n,int m) {
	int i=get_local_id(0);
        int j=get_group_id(0);// 1,2,3,4 -->
        X[get_global_id(0)]=r2(X,T,P,alpha,i,j,n,m);
	//m*get_group_id(0)+get_local_id(0);
}

__kernel void asm(global int* X,global int* D,global charType* alpha,global charType* T,global charType* P,int n,int m) {
        int globalId=get_global_id(0);
        int y=globalId/(m+1);
        int x=globalId-y*(m+1);
        int size=get_local_size(0);
        int locWhere=get_local_id(0);
        D[globalId]=r3(D,X,alpha,T,P,x,y,n,m);
        //if(size==1+locWhere)
        //        barrier(CLK_GLOBAL_MEM_FENCE);
        //else 
        //        barrier(CLK_GLOBAL_MEM_FENCE);
}