#include <stdio.h>
#include <string.h>
#include <xmmintrin.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*!
 * \return Returns the number of nanoseconds ellapsed between \a start and \a end.
 */
int64_t ellapsed_time(struct timespec start, struct timespec end)
{
	const int64_t delta_sec = end.tv_sec - start.tv_sec;
	const int64_t delta = delta_sec * 1000000000 + end.tv_nsec - start.tv_nsec ;
	return delta;
}

/*!
 * \brief Print help message
 */
void print_help()
{
	printf("Usage:\n\tadd [OPTIONS]\n");
	printf("OPTIONS:\n");
	printf("\t-h, --help\tDisplays this message.\n\t-n N\t\tUses arrays of size N.\n\t-f\t\tFast: does not display sum result.\n");
}

/*!
 * main function
 */
int main(int argc, char *argv[])
{
	/* Variables */	
	int fast = 0;
	unsigned long int  i = 0;
	unsigned long int  n = 16;
	int index;
	struct timespec start_time;
	struct timespec end_time;
	int64_t normal_delta_time = 0;
	int64_t sse_delta_time = 0;
	size_t array_size = 0;
	int *array1 = 0;
	int *array2 = 0;
	int *array3 = 0;
	int *array4 = 0;
	
	
	/* Parse args */
	for( index = 1 ; index < argc ; ++index )
	{
		if( strcmp(argv[index], "-h") == 0 || strcmp(argv[index], "--help") == 0 )
		{
			/* Show help and quit */
			print_help();
			return 0;
		}
		else if( strcmp(argv[index], "-n") == 0 )
		{
			if( ++index < argc)
			{
				char *endptr;
				n = strtoul(argv[index], &endptr, 10);
				if( endptr == argv[index] )
				{
					fprintf(stderr, "ERROR: %s is not an integer!\n", argv[index]);
					return -1;
				}
			}
			else
			{
				fprintf(stderr, "Missing parameter after '-n'\n");
			}
		}
		else if( strcmp(argv[index], "-f" ) == 0 )
		{
			fast = 1;
		}
	}

	/* Initialize radom number generator */
	srand(time(0));
	

	/* Initialize arrays */
	array_size = n * sizeof(int);
	array1 = (int *)malloc(array_size);
	array2 = (int *)malloc(array_size);
	array3 = (int *)malloc(array_size);
	array4 = (int *)malloc(array_size);
	
	for( i = 0 ; i < n ; ++i )
	{
		array1[i] = rand()%100;
		array2[i] = rand()%100;
	}

	memset(array3, 0, array_size);
	memset(array4, 0, array_size);

	/* Display values before */
	if ( ! fast )
	{
		printf(" i\t A\t B\tC(1)\tC(2)\n");
		printf("-----------------\n");
		for( i = 0 ; i < n ; ++i )
		{
			printf("%2d\t%2d\t%2d\t%3d\t%3d\n", i, array1[i], array2[i], array3[i], array4[i]);
		}
	}
	
	/* Sum using SSE */
	__m128i *av = (__m128i *)array1;	
	__m128i *bv = (__m128i *)array2;
	__m128i *cv = (__m128i *)array3;
	

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

	sse_delta_time = ellapsed_time( start_time, end_time);

	/* Do standard addition */

#if  _POSIX_C_SOURCE >= 199309L
	clock_gettime(CLOCK_REALTIME, &start_time);
#endif
        for( i = 0 ; i < n ; ++i )
        {
                array4[i] = array1[i] + array2[i];

        }

#if  _POSIX_C_SOURCE >= 199309L
    clock_gettime(CLOCK_REALTIME, &end_time);
#endif

	normal_delta_time = ellapsed_time( start_time, end_time);

	/* Display result */
	if( !fast )
	{	
		printf(" i\t A\t B\tC(1)\tC(2)\n");
		printf("-----------------\n");
		for( i = 0 ; i < n ; ++i )
		{
			printf("%2d\t%2d\t%2d\t%3d\t%3d\n", i, array1[i], array2[i], array3[i], array4[i]);
		}
	}

	printf("SSE Stats:\n");
	printf("\tEllapsed Time: %jd ns\n", sse_delta_time);
	printf("\tTime/int: %jd ns\n", sse_delta_time / n);

	printf("\nNormal Stats:\n");
    printf("\tEllapsed Time: %jd ns\n", normal_delta_time);
    printf("\tTime/int: %jd ns\n", normal_delta_time / n);

#if  _POSIX_C_SOURCE >= 199309L
	struct timespec  res;	
	clock_getres(CLOCK_REALTIME, &res);
	printf("Time resolution: %ds %dns\n", res.tv_sec, res.tv_nsec);
#endif

	return 0;


}
