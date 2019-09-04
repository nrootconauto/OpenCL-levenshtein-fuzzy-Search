 # OpenCL Levenshtein fuzzy searcher

 
 Does a fuzzy search for your text based on https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0186251#pone.0186251.e003
## How to use
**YOU MUST FIRST COMPILE THE test.cl([http://clcc.sourceforge.net/](http://clcc.sourceforge.net/)) KERNEL FOR YOUR DEVICE/PLATFORM**
 1. Compile the Cl kernel using clcc or somthing
 2. Select your platform and device that you will use(select it with `CL_fuzzy_load(platform,device,binaryLocation)`
 3. Launch it!!! :) (Take a peak at the code below for an example)

## Easy ways you(anyone) can contribute

 - Fix the spelling in the source code
 -  Actually include an example program
 -  Give thanks to https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0186251#pone.0186251.e003

 ## Example Code:
  
    #include <stdlib.b>
    #include <stdio.h>
    #include <luancher.h>
    int main() {	    	
    	const char* alpabet="ACGT";
    		const char* t="CATGACTG";
    		const char* p="TACTG";
    		CL_launcher launcher=CL_fuzzy_load(0,0,"cl.out");
    		int m=strlen(p);
    		int n=strlen(t);
    		int* D=alloca(sizeof(int)*(n+1)*(m+1));
    		CL_fuzzy_launch(D,t,p,n,m,alpabet,&launcher);
    		CL_fuzzy_free(&launcher);
    		return EXIT_SUCCESS;
    }	

