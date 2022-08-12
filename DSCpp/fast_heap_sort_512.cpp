#include <intrin.h>
#include "utils.h"
#include "utils_512.h"

void HeapAdjust512(int data[], int p, int n, bool swap)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (swap) {

		__m512i data_0 = _mm512_loadu_epi32(data + 0);
		__m512i data_i = _mm512_loadu_epi32(data + n);
		_mm512_storeu_epi32(data + n, data_0);
		_mm512_storeu_epi32(data + 0, data_i);
	}
	__m512i positions = _mm512_setr_epi32(
		(p + 0), (p + 1), (p + 2), (p + 3),
		(p + 4), (p + 5), (p + 6), (p + 7),
		(p + 0 + 8), (p + 1 + 8), (p + 2 + 8), (p + 3 + 8),
		(p + 4 + 8), (p + 5 + 8), (p + 6 + 8), (p + 7 + 8));
	__m512i temporary = _mm512_i32gather_epi32(positions, data, sizeof(int));
	__m512i child_positions =
		_mm512_add_epi32(positions, _mm512_set1_epi32(p + stride));
	do {
		//while (childpos < n)
		__mmask16 while_lt = _mm512_cmplt_epi32_mask(child_positions, _mm512_set1_epi32(n));
		//if all not, break
		if (while_lt == 0)break;

		__m512i data_at_positions = _mm512_setzero_si512();
		__m512i data_at_child_positions_plus_one = _mm512_setzero_si512();

		//if (child_positions + 1 < n && data[child_positions] < data[child_positions + 1])
		// {
		//   child_positions_plus_one = child_positions+1
		__m512i child_positions_plus_one = _mm512_add_epi32(
			child_positions, _mm512_set1_epi32(stride));

		// child_positions_plus_one < n
		__mmask16 maxpos_lt = _mm512_cmplt_epi32_mask(
			child_positions_plus_one, _mm512_set1_epi32(n));

		//data_at_child_positions = data[child_positions]
		__m512i data_at_child_positions = _mm512_i32gather_epi32(child_positions, data, sizeof(int));
		//data_at_child_positions_plus_one = data[child_positions_plus_one]
		data_at_child_positions_plus_one = _mm512_mask_blend_epi32(
			while_lt & maxpos_lt, data_at_child_positions,
			_mm512_i32gather_epi32(child_positions_plus_one, data, sizeof(int)));

		//data[child_positions]<data[child_positions + 1]
		__mmask16 maxdata_lt = _mm512_cmplt_epi32_mask(
			data_at_child_positions,
			data_at_child_positions_plus_one);

		//then: child_positions++	
		child_positions = _mm512_mask_blend_epi32(
			while_lt & maxpos_lt & maxdata_lt,
			child_positions, child_positions_plus_one);

		//}

		//if (data[position] < data[child_positions]) {
		// data[position]
		data_at_child_positions = _mm512_i32gather_epi32(child_positions, data, sizeof(int));

		//data[child_positions]
		data_at_positions = _mm512_i32gather_epi32(positions, data, sizeof(int));

		//data[positions] < data[child_positions]
		__mmask16 maxdata_lt2 = _mm512_cmplt_epi32_mask(data_at_positions, data_at_child_positions);

		if (maxdata_lt2 == 0) break;

		//data[positions] = data[childpos]
		data_at_positions = _mm512_mask_blend_epi32(while_lt & maxdata_lt2, data_at_positions, data_at_child_positions);
		//save them
		_mm512_mask_i32scatter_epi32(data, while_lt & maxdata_lt2, positions, data_at_positions, sizeof(int));

		//positions = child_positions;
		positions = _mm512_mask_blend_epi32(while_lt & maxdata_lt2, positions, child_positions);

		//NOTICE: this is very important!
		// How to calculate?
		//  child_positions = 2 * child_positions + 1;
		// Follow the following instructions:
		__m512i rem = _mm512_rem_epi32(child_positions, _mm512_set1_epi32(stride));
		__m512i body = _mm512_sub_epi32(child_positions, rem);
		child_positions = _mm512_add_epi32(
			_mm512_add_epi32(_mm512_add_epi32(body, body), rem),
			_mm512_set1_epi32((stride)));

		//save them
		_mm512_mask_i32scatter_epi32(data, while_lt, positions, temporary, sizeof(int));

	} while (true);
}
bool FastHeapSort512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = ((n / stride - 1) >> 1) * stride; i >= 0; i -= stride)
		HeapAdjust512(data, i, n, false);
	for (int i = n - stride; i > 0; i -= stride)
	{
		HeapAdjust512(data, 0, i, true);
	}

	Merge(data, n, stride);

	return true;
}
void HeapAdjust512(unsigned int data[], int p, int n, bool swap)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (swap) {

		__m512i data_0 = _mm512_loadu_epi32(data + 0);
		__m512i data_i = _mm512_loadu_epi32(data + n);
		_mm512_storeu_epi32(data + n, data_0);
		_mm512_storeu_epi32(data + 0, data_i);
	}
	__m512i positions = _mm512_setr_epi32(
		(p + 0), (p + 1), (p + 2), (p + 3),
		(p + 4), (p + 5), (p + 6), (p + 7),
		(p + 0 + 8), (p + 1 + 8), (p + 2 + 8), (p + 3 + 8),
		(p + 4 + 8), (p + 5 + 8), (p + 6 + 8), (p + 7 + 8));
	__m512i temporary = _mm512_i32gather_epi32(positions, data, sizeof(int));
	__m512i child_positions =
		_mm512_add_epi32(positions, _mm512_set1_epi32(p + stride));
	do {
		//while (childpos < n)
		__mmask16 while_lt = _mm512_cmplt_epi32_mask(child_positions, _mm512_set1_epi32(n));
		//if all not, break
		if (while_lt == 0)break;

		__m512i data_at_positions = _mm512_setzero_si512();
		__m512i data_at_child_positions_plus_one = _mm512_setzero_si512();

		//if (child_positions + 1 < n && data[child_positions] < data[child_positions + 1])
		// {
		//   child_positions_plus_one = child_positions+1
		__m512i child_positions_plus_one = _mm512_add_epi32(
			child_positions, _mm512_set1_epi32(stride));

		// child_positions_plus_one < n
		__mmask16 maxpos_lt = _mm512_cmplt_epi32_mask(
			child_positions_plus_one, _mm512_set1_epi32(n));

		//data_at_child_positions = data[child_positions]
		__m512i data_at_child_positions = _mm512_i32gather_epi32(child_positions, data, sizeof(int));
		//data_at_child_positions_plus_one = data[child_positions_plus_one]
		data_at_child_positions_plus_one = _mm512_mask_blend_epi32(
			while_lt & maxpos_lt, data_at_child_positions,
			_mm512_i32gather_epi32(child_positions_plus_one, data, sizeof(int)));

		//data[child_positions]<data[child_positions + 1]
		__mmask16 maxdata_lt = _mm512_cmplt_epi32_mask(
			data_at_child_positions,
			data_at_child_positions_plus_one);

		//then: child_positions++	
		child_positions = _mm512_mask_blend_epi32(
			while_lt & maxpos_lt & maxdata_lt,
			child_positions, child_positions_plus_one);

		//}

		//if (data[position] < data[child_positions]) {
		// data[position]
		data_at_child_positions = _mm512_i32gather_epi32(child_positions, data, sizeof(int));

		//data[child_positions]
		data_at_positions = _mm512_i32gather_epi32(positions, data, sizeof(int));

		//data[positions] < data[child_positions]
		__mmask16 maxdata_lt2 = _mm512_cmplt_epi32_mask(data_at_positions, data_at_child_positions);

		if (maxdata_lt2 == 0) break;

		//data[positions] = data[childpos]
		data_at_positions = _mm512_mask_blend_epi32(while_lt & maxdata_lt2, data_at_positions, data_at_child_positions);
		//save them
		_mm512_mask_i32scatter_epi32(data, while_lt & maxdata_lt2, positions, data_at_positions, sizeof(int));

		//positions = child_positions;
		positions = _mm512_mask_blend_epi32(while_lt & maxdata_lt2, positions, child_positions);

		//NOTICE: this is very important!
		// How to calculate?
		//  child_positions = 2 * child_positions + 1;
		// Follow the following instructions:
		__m512i rem = _mm512_rem_epi32(child_positions, _mm512_set1_epi32(stride));
		__m512i body = _mm512_sub_epi32(child_positions, rem);
		child_positions = _mm512_add_epi32(
			_mm512_add_epi32(_mm512_add_epi32(body, body), rem),
			_mm512_set1_epi32((stride)));

		//save them
		_mm512_mask_i32scatter_epi32(data, while_lt, positions, temporary, sizeof(int));

	} while (true);
}
bool FastHeapSort512(unsigned int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = ((n / stride - 1) >> 1) * stride; i >= 0; i -= stride)
		HeapAdjust512(data, i, n, false);
	for (int i = n - stride; i > 0; i -= stride)
	{
		HeapAdjust512(data, 0, i, true);
	}

	Merge(data, n, stride);

	return true;
}
