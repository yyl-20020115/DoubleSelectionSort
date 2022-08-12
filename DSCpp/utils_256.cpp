#include <memory>
#include <intrin.h>
#include "utils.h"
#include "utils_256.h"
#include "horizontal_max_min_256.h"

__m256i expand_bits_to_bytes(unsigned int x)
{
	__m256i xbcast = _mm256_set1_epi32(x);    // we only use the low 32bits of each lane, but this is fine with AVX2

	// Each byte gets the source byte containing the corresponding bit
	__m256i shufmask = _mm256_set_epi64x(
		0x0303030303030303, 0x0202020202020202,
		0x0101010101010101, 0x0000000000000000);
	__m256i shuf = _mm256_shuffle_epi8(xbcast, shufmask);

	__m256i andmask = _mm256_set1_epi64x(0x8040201008040201);  // every 8 bits -> 8 bytes, pattern repeats.
	__m256i isolated_inverted = _mm256_andnot_si256(shuf, andmask);

	// this is the extra step: compare each byte == 0 to produce 0 or -1
	return _mm256_cmpeq_epi8(isolated_inverted, andmask/*_mm256_setzero_si256()*/);
	// alternative: compare against the AND mask to get 0 or -1,
	// avoiding the need for a vector zero constant.
}
__m256i _mm256i_convert_mask8_to_epi32(__mmask8 m) 
{
	const __m256i vshift_count = _mm256_set_epi32(24, 25, 26, 27, 28, 29, 30, 31);
	__m256i bcast = _mm256_set1_epi32(m);
	__m256i shifted = _mm256_sllv_epi32(bcast, vshift_count);  // high bit of each element = corresponding bit of the mask

	// use _mm256_and and _mm256_cmpeq if you need all bits set.
	return _mm256_srai_epi32(shifted, 31);
	// broadcast the sign bit to the whole element
}
void _mm256_mask_i32scatter_epi32_avx2(void* base_addr, __mmask8 k, __m256i vindex, __m256i a, const int scale)
{
	long _m = (unsigned long)k;
	unsigned long index = 0;
	int vis[8] = { 0 };
	int val[8] = { 0 };
	_mm256_store_ps((float*)vis, _mm256_castsi256_ps(vindex));
	_mm256_store_ps((float*)val, _mm256_castsi256_ps(a));

	while (_BitScanForward(&index, _m) && (index < 8)) {
		_bittestandreset(&_m, index);
		int i = vis[index] * scale;
		*(int*)(((char*)base_addr) + i) = val[index];
	}
}

void Merge256(int data[], int n)
{
#if 1
#else
	const int stride = sizeof(__m256i) / sizeof(int);
	__m256i _stride = _mm256_set1_epi32(stride);
	//merege:
	int merge_indices_buffer[stride]{};
	for (int i = 0; i < stride; i++)
		merge_indices_buffer[i] = i;
	
	__m256i merge_indices = _mm256_loadu_epi32(merge_indices_buffer);

	int* buffer = new int[(n + stride)];
	memset(buffer,~0, (n + stride) * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		__m256i current = _mm256_i32gather_epi32(data, merge_indices, sizeof(int));
		unsigned int min_value = 0;
		int min_index = HorizontalMin32(current,&min_value);
		__mmask8 mask = (1 << min_index);
		merge_indices = _mm256_mask_add_epi32(merge_indices, mask, merge_indices, _stride);
		buffer[i] = min_value;
	}
	memcpy_s(data, n * sizeof(int), buffer, n * sizeof(int));
	delete[] buffer;
#endif
}
