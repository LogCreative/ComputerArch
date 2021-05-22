#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif


#define CLOCK_RATE_GHZ 2.26e9

static __inline__ uint64_t RDTSC()
{
    uint32_t hi, lo;
    __asm__ volatile
	(
		"rdtsc"
		: "=a"(lo), "=d"(hi)
	);
	return (((uint64_t)hi) << 32) | ((uint64_t)lo);
}

static int sum_naive(int n, int *a)
{
    int sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += a[i];
	}
	return sum;
}

static int sum_unrolled(int n, int *a)
{
    int sum = 0;

    // unrolled loop
	for (int i = 0; i < n / 4 * 4; i += 4)
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];

    }

    // tail case
	for (int i = n / 4 * 4; i < n; i++)
	{
		sum += a[i];
	}

    return sum;
}

static int sum_vectorized(int n, int *a)
{
    // WRITE YOUR VECTORIZED CODE HERE

	// the parallelization could only
	// apply on aligned groups
	int groups =  n / 4;

	// On linux, int is 32-bit wide.
	// 128-bit = 4 int;
	// Initialize sum vector of 128-bit as zero.
	__m128i sum = _mm_setzero_si128();

	// For loop will add numbers on mod 4 basis.
	for(int i = 0; i < groups; ++i){
		// load a vector of adders.
		__m128i adder = _mm_loadu_si128(a + i * 4);

		// add the vector to a temporary variable.
		__m128i added = _mm_add_epi32(sum, adder);

		// store the value to the sum vector.
		_mm_storeu_si128(&sum, added);
	}

	int result = 0;

	// However there is some remaining numbers
	// that didn't count, whose quantity is < 4.
	int remain = n - groups * 4;
	int tip = groups * 4;

	// For each offset, calulate manually.
	for(int j = 0; j < remain; ++j)
		result += a[tip++];

	// and to get the final result, the member
	// of the vector in sum has to be added as well,
	// in other word, 4 numbers.
	
	// For each right shift, get the lower 32 bit.
	// which is one unit and add to the result.
	for(int i = 0; i < 4; ++i)
		result += _mm_cvtsi128_si32(_mm_srli_si128(sum,i * 4));

    return result;
}

static int sum_vectorized_unrolled(int n, int *a)
{
    // UNROLL YOUR VECTORIZED CODE HERE

	__m128i sum = _mm_setzero_si128();

	// For unrolled scenario,
	// every loop will contribute 128 * 4 = 512 bit operation.
	for(int i = 0; i < n / 16 * 16; i += 16){
		_mm_storeu_si128(&sum, _mm_add_epi32(sum, _mm_loadu_si128(a + i )));
		_mm_storeu_si128(&sum, _mm_add_epi32(sum, _mm_loadu_si128(a + i + 4)));
		_mm_storeu_si128(&sum, _mm_add_epi32(sum, _mm_loadu_si128(a + i + 8)));
		_mm_storeu_si128(&sum, _mm_add_epi32(sum, _mm_loadu_si128(a + i + 12)));
	}
	
	int result = 0;
	for(int i = n / 16 * 16; i < n; i++)
		result += a[i];
	
	for(int k = 0; k < 4; ++k)
		result += _mm_cvtsi128_si32(_mm_srli_si128(sum,k * 4));

    return result;
}

void benchmark(int n, int *a, int(*computeSum)(int, int*), char *name)
{
    // warm up cache
    int sum = computeSum(n, a);

    // measure
    uint64_t beginCycle = RDTSC();
    sum += computeSum(n, a);
	uint64_t cycles = RDTSC() - beginCycle;

    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;

    // print results
	printf("%20s: ", name);
	if (sum == 2 * sum_naive(n, a))
	{
		printf("%.2f microseconds\n", microseconds);
	}
	else
	{
		printf("ERROR!\n");
	}
}

int main(int argc, char **argv)
{
    const int n = 9999;

    // initialize the array with random values
	srand48(time(NULL));
	int a[n] __attribute__((aligned(16)));
	for (int i = 0; i < n; i++)
	{
		a[i] = lrand48();
	}

    // benchmark series of codes
	benchmark(n, a, sum_naive, "naive");
	benchmark(n, a, sum_unrolled, "unrolled");
	benchmark(n, a, sum_vectorized, "vectorized");
	benchmark(n, a, sum_vectorized_unrolled, "vectorized unrolled");

    return 0;
}
