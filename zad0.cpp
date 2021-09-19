#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

struct itrv {
	double h;
	double left;
	double right;
	double sum = 0;
};

void* pi_routine(void* arg) {
	itrv* parg = (itrv*)(arg);
	double x = parg->left + parg->h/2, sum = 0;
	while (x < parg->right) {
		x += parg->h;
		sum += 4.0/(1 + x*x);
	}
	parg->sum = sum * parg->h;
	return NULL;
}


void join_run(int nump, int numt) {
	timeval t1, t2;
	gettimeofday(&t1, NULL);
	double h = 1./static_cast<double>(nump);
	double step = 1./static_cast<double>(numt); 
	itrv *itrvs = (itrv *)calloc(numt, sizeof(itrv));
    	for (int i = 0; i < numt; ++i) {
        	itrvs[i].left = i*step;
		itrvs[i].right = (i+1)*step;
		itrvs[i].h = h;
    	}
    	pthread_t *threads = (pthread_t *)calloc(numt, sizeof(pthread_t));
    	
	for (int i = 0; i < numt; ++i) {
        	int retVal = pthread_create(&threads[i], NULL, pi_routine, itrvs + i);
        	assert(retVal == 0);
   	 }
	
	for (int i = 0; i < numt; ++i) {
		void* status = NULL;
        	pthread_join(threads[i], &status);
    	}
	
	double pi = 0;
	
	for (int i = 0; i < numt; ++i) 
		pi += itrvs[i].sum;		
	gettimeofday(&t2, NULL);
	printf("%lf\nElapsed time: %lf\n", pi,  t2.tv_sec - t1.tv_sec + (static_cast<double>(t2.tv_usec - t1.tv_usec))/1000000);
	free(itrvs);
	free(threads);
}

int main(int argc, char* argv[]) {
	char *end;	
	join_run(strtol(argv[1], &end, 10),strtol(argv[2], &end, 10));
	return 0;
}
