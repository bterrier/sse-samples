#include <stdio.h>
#include <string.h>
#include <xmmintrin.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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
	
	
	for( i = 0 ; i < n/4 ; ++i )
	{
		cv[i] = _mm_add_epi32(av[i], bv[i]);
			
	}
	
	
		
	printf("\n\n i\t A\t B\tC=A+B\n");
	printf("-----------------\n");
	for( i = 0 ; i < n ; ++i )
	{
		printf("%2d\t%2d\t%2d\t%3d\n", i, array1[i], array2[i], array3[i]);
	}	return 0;
}
