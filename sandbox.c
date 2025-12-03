#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main()
{
	long n;

	struct timeval time;
	struct timeval end_time;
	gettimeofday(&time, NULL);

	printf(" %ld %ld\n", time.tv_sec, time.tv_usec);

	usleep(1000000);
	
	gettimeofday(&end_time, NULL);
	printf(" %ld %ld\n", end_time.tv_sec, end_time.tv_usec);

	printf(" %ld %ld\n", end_time.tv_sec - time.tv_sec, end_time.tv_usec - time.tv_usec);
}