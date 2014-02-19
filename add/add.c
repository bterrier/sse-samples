#include <stdio.h>
#include <string.h>
#include <xmmintrin.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int64_t ellapsed_time(struct timespec start, struct timespec end)
{
	int64_t delta_sec = end.tv_sec - start.tv_sec;
	int64_t delta = delta_sec * 1000000000 + end.tv_nsec - start.tv_nsec ;
	return delta;
}

int main(int argc, char *argv[])
{
	if(argc > 1 && strcmp(argv[1], "-h") == 0)
	{
		printf("Usage:\n\tadd [-h] [-n N]\n");
		printf("\t-h\tDisplays this message.\n\t-n N\tUses arrays of size N.\n");
		return 0;
	}
	unsigned long int  i = 0;
	unsigned long int  n = 16;
	if(argc == 3)
	{
		
		if(strcmp(argv[1], "-n") == 0 )
		{
			n = strtoul (argv[2], 0, 10);
		}
		
	}
	
	int *array1 = (int *)malloc(n * sizeof(int));
	int *array2 = (int *)malloc(n * sizeof(int));
	int *array3 = (int *)malloc(n * sizeof(int));
	
	srand(time(0));
	
	printf(" i\t A\t B\tC=A+B\n");
	printf("-----------------\n");
	for( i = 0 ; i < n ; ++i )
	{
		array1[i] = rand()%100;
		array2[i] = rand()%100;
		printf("%2d\t%2d\t%2d\t%3d\n", i, array1[i], array2[i], array3[i]);
	}
	
	__m128i *av = (__m128i *)array1;	
	__m128i *bv = (__m128i *)array2;
	__m128i *cv = (__m128i *)array3;
	
	struct timespec start_time;
	struct timespec end_time;

#if  _POSIX_C_SOURCE >= 199309L
	clock_gettime(CLOCK_REALTIME, &start_time);
#endif

	for( i = 0 ; i < n/4 ; ++i )
	{
		cv[i] = _mm_add_epi32(av[i], bv[i]);

	}
#if  _POSIX_C_SOURCE >= 199309L
	clock_gettime(CLOCK_REALTIME, &end_time);
#endif
	int64_t delta_time = ellapsed_time( start_time, end_time);
	printf("\n\n i\t A\t B\tC=A+B\n");
	printf("-----------------\n");
	for( i = 0 ; i < n ; ++i )
	{
		printf("%2d\t%2d\t%2d\t%3d\n", i, array1[i], array2[i], array3[i]);
	}

	printf("Ellapsed Time: %jd ns\n", delta_time);
	printf("Time/int: %jd ns\n", delta_time / n);

	return 0;


}
