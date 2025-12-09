#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

// getTimeOfDay()使い方
int main()
{
	long n;

	struct timeval time;
	struct timeval end_time;
	gettimeofday(&time, NULL);

	printf(" %ld %d\n", time.tv_sec, time.tv_usec);
	long start = time.tv_sec * 1000000 + time.tv_usec;

	usleep(1000);
	
	gettimeofday(&end_time, NULL);
	long end = end_time.tv_sec * 1000000 + end_time.tv_usec;

	printf(" %ld %d\n", end_time.tv_sec, end_time.tv_usec);

	printf("start: %ld  end: %ld\n", start, end);
	printf("元々： %ld %d\n", end_time.tv_sec - time.tv_sec, end_time.tv_usec - time.tv_usec);
	printf("新: %ld\n", end - start);


}