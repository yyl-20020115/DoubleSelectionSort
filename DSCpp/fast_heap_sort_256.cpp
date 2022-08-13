#include <intrin.h>
#include "utils.h"
#include "utils_256.h"
void HeapAdjust256(int data[], int p, int n, bool swap)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (swap) {

		__m256i data_0 = _mm256_loadu_epi32(data + 0);
		__m256i data_i = _mm256_loadu_epi32(data + n);
		_mm256_storeu_epi32(data + n, data_0);
		_mm256_storeu_epi32(data + 0, data_i);
	}
	__m256i positions = _mm256_setr_epi32(
		(p + 0), (p + 1), (p + 2), (p + 3),
		(p + 4), (p + 5), (p + 6), (p + 7));
	__m256i temporary = _mm256_i32gather_epi32(data, positions, sizeof(data[0]));
	__m256i child_positions =
		_mm256_add_epi32(positions, _mm256_set1_epi32(p + stride));
	do {
		//while (childpos < n)
		__mmask8 while_lt = _mm256_cmplt_epi32_mask(child_positions, _mm256_set1_epi32(n));
		//if all not, break
		if (while_lt == 0)break;

		__m256i data_at_positions = _mm256_setzero_si256();
		__m256i data_at_child_positions_plus_one = _mm256_setzero_si256();

		//if (child_positions + 1 < n && data[child_positions] < data[child_positions + 1])
		// {
		//   child_positions_plus_one = child_positions+1
		__m256i child_positions_plus_one = _mm256_add_epi32(
			child_positions, _mm256_set1_epi32(stride));

		// child_positions_plus_one < n
		__mmask8 maxpos_lt = _mm256_cmplt_epi32_mask(
			child_positions_plus_one, _mm256_set1_epi32(n));

		//data_at_child_positions = data[child_positions]
		__m256i data_at_child_positions = _mm256_i32gather_epi32(data, child_positions, sizeof(data[0]));
		//data_at_child_positions_plus_one = data[child_positions_plus_one]
		data_at_child_positions_plus_one = _mm256_mask_blend_epi32(
			while_lt & maxpos_lt, data_at_child_positions,
			_mm256_i32gather_epi32(data, child_positions_plus_one, sizeof(data[0])));

		//data[child_positions]<data[child_positions + 1]
		__mmask8 maxdata_lt = _mm256_cmplt_epi32_mask(
			data_at_child_positions,
			data_at_child_positions_plus_one);

		//then: child_positions++	
		child_positions = _mm256_mask_blend_epi32(
			while_lt & maxpos_lt & maxdata_lt,
			child_positions, child_positions_plus_one);

		//}

		//if (data[position] < data[child_positions]) {
		// data[position]
		data_at_child_positions = _mm256_i32gather_epi32(data, child_positions, sizeof(data[0]));

		//data[child_positions]
		data_at_positions = _mm256_i32gather_epi32(data, positions, sizeof(data[0]));

		//data[positions] < data[child_positions]
		__mmask8 maxdata_lt2 = _mm256_cmplt_epi32_mask(data_at_positions, data_at_child_positions);

		if (maxdata_lt2 == 0) break;

		//data[positions] = data[childpos]
		data_at_positions = _mm256_mask_blend_epi32(while_lt & maxdata_lt2, data_at_positions, data_at_child_positions);
		//save them
		_mm256_mask_i32scatter_epi32(data, while_lt & maxdata_lt2, positions, data_at_positions, sizeof(int));

		//positions = child_positions;
		positions = _mm256_mask_blend_epi32(while_lt & maxdata_lt2, positions, child_positions);

		//NOTICE: this is very important!
		// How to calculate?
		//  child_positions = 2 * child_positions + 1;
		// Follow the following instructions:
		__m256i rem = _mm256_rem_epi32(child_positions, _mm256_set1_epi32(stride));
		__m256i body = _mm256_sub_epi32(child_positions, rem);
		child_positions = _mm256_add_epi32(
			_mm256_add_epi32(_mm256_add_epi32(body, body), rem),
			_mm256_set1_epi32((stride)));

		//save them
		_mm256_mask_i32scatter_epi32(data, while_lt, positions, temporary, sizeof(data[0]));

	} while (true);
}
bool FastHeapSort256(int data[], int n)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = ((n / stride - 1) >> 1) * stride; i >= 0; i -= stride)
		HeapAdjust256(data, i, n, false);
	for (int i = n - stride; i > 0; i -= stride)
	{
		HeapAdjust256(data, 0, i, true);
	}

	Merge(data, n, stride);

	return true;
}
void HeapAdjust256(unsigned int data[], int p, int n, bool swap)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (swap) {

		__m256i data_0 = _mm256_loadu_epi32(data + 0);
		__m256i data_i = _mm256_loadu_epi32(data + n);
		_mm256_storeu_epi32(data + n, data_0);
		_mm256_storeu_epi32(data + 0, data_i);
	}
	__m256i positions = _mm256_setr_epi32(
		(p + 0), (p + 1), (p + 2), (p + 3),
		(p + 4), (p + 5), (p + 6), (p + 7));
	__m256i temporary = _mm256_i32gather_epi32((int*)data, positions, sizeof(data[0]));
	__m256i child_positions =
		_mm256_add_epi32(positions, _mm256_set1_epi32(p + stride));
	do {
		//while (childpos < n)
		__mmask8 while_lt = _mm256_cmplt_epi32_mask(child_positions, _mm256_set1_epi32(n));
		//if all not, break
		if (while_lt == 0)break;

		__m256i data_at_positions = _mm256_setzero_si256();
		__m256i data_at_child_positions_plus_one = _mm256_setzero_si256();

		//if (child_positions + 1 < n && data[child_positions] < data[child_positions + 1])
		// {
		//   child_positions_plus_one = child_positions+1
		__m256i child_positions_plus_one = _mm256_add_epi32(
			child_positions, _mm256_set1_epi32(stride));

		// child_positions_plus_one < n
		__mmask8 maxpos_lt = _mm256_cmplt_epi32_mask(
			child_positions_plus_one, _mm256_set1_epi32(n));

		//data_at_child_positions = data[child_positions]
		__m256i data_at_child_positions = _mm256_i32gather_epi32((int*)data, child_positions, sizeof(data[0]));
		//data_at_child_positions_plus_one = data[child_positions_plus_one]
		data_at_child_positions_plus_one = _mm256_mask_blend_epi32(
			while_lt & maxpos_lt, data_at_child_positions,
			_mm256_i32gather_epi32((int*)data, child_positions_plus_one, sizeof(data[0])));

		//data[child_positions]<data[child_positions + 1]
		__mmask8 maxdata_lt = _mm256_cmplt_epi32_mask(
			data_at_child_positions,
			data_at_child_positions_plus_one);

		//then: child_positions++	
		child_positions = _mm256_mask_blend_epi32(
			while_lt & maxpos_lt & maxdata_lt,
			child_positions, child_positions_plus_one);

		//}

		//if (data[position] < data[child_positions]) {
		// data[position]
		data_at_child_positions = _mm256_i32gather_epi32((int*)data, child_positions, sizeof(data[0]));

		//data[child_positions]
		data_at_positions = _mm256_i32gather_epi32((int*)data, positions, sizeof(data[0]));

		//data[positions] < data[child_positions]
		__mmask8 maxdata_lt2 = _mm256_cmplt_epi32_mask(data_at_positions, data_at_child_positions);

		if (maxdata_lt2 == 0) break;

		//data[positions] = data[childpos]
		data_at_positions = _mm256_mask_blend_epi32(while_lt & maxdata_lt2, data_at_positions, data_at_child_positions);
		//save them
		_mm256_mask_i32scatter_epi32(data, while_lt & maxdata_lt2, positions, data_at_positions, sizeof(data[0]));

		//positions = child_positions;
		positions = _mm256_mask_blend_epi32(while_lt & maxdata_lt2, positions, child_positions);

		//NOTICE: this is very important!
		// How to calculate?
		//  child_positions = 2 * child_positions + 1;
		// Follow the following instructions:
		__m256i rem = _mm256_rem_epi32(child_positions, _mm256_set1_epi32(stride));
		__m256i body = _mm256_sub_epi32(child_positions, rem);
		child_positions = _mm256_add_epi32(
			_mm256_add_epi32(_mm256_add_epi32(body, body), rem),
			_mm256_set1_epi32((stride)));

		//save them
		_mm256_mask_i32scatter_epi32(data, while_lt, positions, temporary, sizeof(data[0]));

	} while (true);
}
bool FastHeapSort256(unsigned int data[], int n)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = ((n / stride - 1) >> 1) * stride; i >= 0; i -= stride)
		HeapAdjust256(data, i, n, false);
	for (int i = n - stride; i > 0; i -= stride)
	{
		HeapAdjust256(data, 0, i, true);
	}

	Merge(data, n, stride);

	return true;
}
