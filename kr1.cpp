#include <pthread.h>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <stdlib.h>

struct arg {
	int *left;
	int *right;
	long long sum = 0;
};

void* v_routine(void* argv) {
    	arg* parg = (arg*)(argv);
	int *x = parg->left;
	long long sum = 0;
    	while (x < parg->right) {
		sum += *x;
        x++;
    	}
    	parg->sum = sum;
    	return NULL;
}


int vector_sum(int *v, int size, int numt) {
	int step = size / numt;
	int last = size % numt;
    	arg *itrvs = (arg *)calloc(numt + (last != 0), sizeof(arg));
    	for (int i = 0; i < numt; ++i) {
            itrvs[i].left = v + i*step;
        	itrvs[i].right = v + (i+1)*step;
    	}
	if (last){
	        itrvs[numt].left = v + numt*step; 
            itrvs[numt].right = v + numt*step + last;
	}
   	pthread_t *threads = (pthread_t *)calloc(numt +  (last != 0), sizeof(pthread_t));

   	for (int i = 0; i < numt + (last != 0); ++i) {
            int retVal = pthread_create(&threads[i], NULL, v_routine, itrvs + i);
            assert(retVal == 0);
     	}

    	for (int i = 0; i < numt + (last != 0); ++i) {
        	void* status = NULL;
            	pthread_join(threads[i], &status);
        }

    	long long sum = 0;

    	for (int i = 0; i < numt + (last != 0); ++i)
        	sum += itrvs[i].sum;
    	free(itrvs);
    	free(threads);
	return sum;
}

int main(int argc, char* argv[]) {
    	int v[6] = {1,2,3,4,5,6};
	char* end;
    	printf("%d\n",vector_sum(v, 6, strtol(argv[1], &end, 10)));
    	return 0;
}
// restrict указатель - гарантия(от программиста) того, что на данную область памяти больше никто не ссылается
