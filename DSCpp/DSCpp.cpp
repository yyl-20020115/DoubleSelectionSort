#include <iostream>
#include <intrin.h>
#include <algorithm>
#include "common_sort.h"
#include "utils.h"
#include "fast_sort_256.h"
#include "fast_sort_512.h"
#include "string_functions_256.h"

const int DATA_SIZE =  32;// *16 * 16;
const bool use_random = true;
const bool show = false;
const bool allow_common = true;
const bool allow_avx_256 = true;
const bool allow_avx_512 = false;

int data0[DATA_SIZE] = { 0 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 };
//int data0[DATA_SIZE] = { 
//	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
//	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
//	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
//	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127
//};
//int data0[DATA_SIZE] = {
//	0,0,0,0,0,0,0,0,
//	1,1,1,1,1,1,1,1,
//	3,3,3,3,3,3,3,3,
//	2,2,2,2,2,2,2,2,
//	5,5,5,5,5,5,5,5,
//	4,4,4,4,4,4,4,4,
//	7,7,7,7,7,7,7,7,
//	6,6,6,6,6,6,6,6,
//	9,9,9,9,9,9,9,9,
//	11,11,11,11,11,11,11,11,
//	8,8,8,8,8,8,8,8,
//	13,13,13,13,13,13,13,13,
//	10,10,10,10,10,10,10,10,
//	12,12,12,12,12,12,12,12,
//	14,14,14,14,14,14,14,14,
//	15,15,15,15,15,15,15,15,
//};
//int data0[DATA_SIZE] = {
// 55 ,       76 ,        36 ,        39 ,        83 ,       115 ,        21 ,        17,
// 11 ,      102 ,       126 ,       110 ,        45 ,        82 ,        75 ,        70,
//118 ,       72 ,        44 ,        60 ,        54 ,       106 ,        67 ,       126,
// 84 ,      109 ,         1 ,        82 ,       125 ,        19 ,         1 ,        12,
//  8 ,       23 ,        11 ,        17 ,        71 ,        72 ,        44 ,        42,
// 85 ,       46 ,       118 ,        74 ,        27 ,        15 ,        93 ,        15,
// 20 ,       43 ,        70 ,        29 ,        27 ,       112 ,        57 ,        27,
// 51 ,       28 ,        85 ,       112 ,        43 ,       121 ,        96 ,        18,
//  3 ,       47 ,        45 ,       113 ,       108 ,        87 ,        37 ,       102,
// 93 ,       80 ,        41 ,        37 ,         2 ,       122 ,        33 ,        19,
// 14 ,       90 ,       107 ,        15 ,        44 ,        79 ,         8 ,         6,
// 26 ,       85 ,        34 ,        95 ,        47 ,         3 ,        94 ,        27,
// 78 ,       66 ,        25 ,        55 ,        15 ,        93 ,       107 ,        38,
// 29 ,      124 ,        34 ,       123 ,        18 ,        32 ,        13 ,        13,
// 81 ,       38 ,        71 ,        54 ,        32 ,         5 ,        84 ,       116,
// 14 ,       42 ,        79 ,        11 ,       101 ,        65 ,       108 ,       118 
//};
//int data0[DATA_SIZE] = { 12,2,23,25,17,1,16,19,31,19,1,23,31,19,13,25,26,14,10,28,31,25,7,14,17,3,25,24,21,22,8,14 };
//int data0[DATA_SIZE] = { 15,2,1,4,3,9,8,6,7,10,12,11,0,5,14,13 };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };
int data4[DATA_SIZE] = { 0 };
int data5[DATA_SIZE] = { 0 };
int data6[DATA_SIZE] = { 0 };
int data7[DATA_SIZE] = { 0 };
int data8[DATA_SIZE] = { 0 };
int data9[DATA_SIZE] = { 0 };
int data10[DATA_SIZE] = { 0 };
int data11[DATA_SIZE] = { 0 };
int data12[DATA_SIZE] = { 0 };
int data13[DATA_SIZE] = { 0 };
int data14[DATA_SIZE] = { 0 };
int data15[DATA_SIZE] = { 0 };
int data16[DATA_SIZE] = { 0 };
int data17[DATA_SIZE] = { 0 };
int data18[DATA_SIZE] = { 0 };
int data19[DATA_SIZE] = { 0 };
int data20[DATA_SIZE] = { 0 };
int data21[DATA_SIZE] = { 0 };
int data22[DATA_SIZE] = { 0 };
int data23[DATA_SIZE] = { 0 };
int data24[DATA_SIZE] = { 0 };
int data25[DATA_SIZE] = { 0 };

int main()
{
#if 0
	__m128i __data8 = _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m128i _data16 = _mm_setr_epi16(5, 3, 1, 4, 2, 9, 7, 6); //0 is last
	__m256i _data32 = _mm256_setr_epi32(
		0x00050055,
		0x00240044,
		0x0002ffff,
		0x0002ffff,
		0x00050033,
		0x0002ffff,
		0x00170077,
		0x00030033
	);

	unsigned char r8 = 0;
	unsigned short r16 = 0;
	unsigned int r32 = 0;
	unsigned long long r64 = 0;
	int i = 0;

	i = HorizontalMin8(__data8, &r8);
	i = HorizontalMax8(__data8, &r8);
	i = HorizontalMin16(_data16, &r16);
	i = HorizontalMax16(_data16, &r16);
	i = HorizontalMin32(_data32, &r32);
	i = HorizontalMax32(_data32, &r32);
	i = HorizontalMin64(_data32, &r64);
	i = HorizontalMax64(_data32, &r64);
#endif

	long long t0;
	//init
	if (allow_common)
	{
		srand((unsigned)time(0));
		printf("original data(count = %d):\n", DATA_SIZE);
		for (int i = 0; i < DATA_SIZE; i++) {
			if (use_random) {
				data0[i] = (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
			}
			data25[i]
				= data24[i]
				= data23[i]
				= data22[i]
				= data21[i]
				= data20[i]
				= data19[i]
				= data18[i]
				= data17[i]
				= data16[i]
				= data15[i]
				= data14[i]
				= data13[i]
				= data12[i]
				= data11[i]
				= data10[i]
				= data9[i]
				= data8[i]
				= data7[i]
				= data6[i]
				= data5[i]
				= data4[i]
				= data3[i]
				= data2[i]
				= data1[i]
				= data0[i];
			if (show) {
				printf("%d ", data0[i]);
			}
		}
		printf("\n\n");
	}
	//system quick sort
	if (allow_common)
	{
		printf("for system quick sort:\n");
		t0 = _Query_perf_counter();
		{
			std::sort(data0, data0 + DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		if (show) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data0[i]);
			}
		}
		printf("\n\n");
	}
	//quick sort
	if (allow_common)
	{
		printf("for quick sort:\n");
		t0 = _Query_perf_counter();
		{
			QuickSort(data1, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data1, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data1[i]);
			}
		}
		printf("\n\n");
	}
	//fast quick sort 256
	if (allow_avx_256)
	{
		printf("for fast quick sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastQuickSort256(data5, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data5, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data5[i]);
			}
		}
		printf("\n\n");
	}
	//fast quick sort 512
	if (allow_avx_512)
	{
		printf("for fast quick sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastQuickSort512(data6, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data6, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data6[i]);
			}
		}
		printf("\n\n");
	}
	//odd even sort
	if (false & allow_common)
	{
		t0 = _Query_perf_counter();
		printf("for odd even sort:\n");
		{
			OddEvenSort(data2, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data2, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data2[i]);
			}
		}
		printf("\n\n");
	}
	//odd even sort 256
	if (false & allow_avx_256)
	{
		printf("for fast odd even sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastOddEvenSort256(data3, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data3, DATA_SIZE);

		printf("correct:%s\n", b ? "true" : "false");
		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data3[i]);
			}
		}
		printf("\n\n");
	}
	//odd even sort 512
	if (false & allow_avx_512)
	{
		printf("for fast odd even sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastOddEvenSort512(data4, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data4, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");
		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data4[i]);
			}
		}
		printf("\n\n");
	}
	//merge sort recursive
	if (allow_common)
	{
		printf("for merge sort recursive:\n");
		t0 = _Query_perf_counter();
		{
			RecursiveMergeSort(data9, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data9, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data9[i]);
			}
		}
		printf("\n\n");
	}
	//merge sort non recursive
	if (allow_common)
	{
		printf("for merge sort non-recursive:\n");
		t0 = _Query_perf_counter();
		{
			RecursiveMergeSort(data10, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data10, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data10[i]);
			}
		}
		printf("\n\n");
	}
	//fast merge sort 256
	if (allow_avx_256)
	{
		printf("for fast merge sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastMergeSort256(data7, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data7, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data7[i]);
			}
		}
		printf("\n\n");
	}
	//fast merge sort 512
	if (allow_avx_512)
	{
		printf("for fast merge sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastMergeSort512(data8, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data8, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data8[i]);
			}
		}
		printf("\n\n");
	}
	//single selection sort
	if (allow_common)
	{
		printf("for single selection sort:\n");
		t0 = _Query_perf_counter();
		{
			SingleSelectionSort(data11, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data11, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data11[i]);
			}
		}
		printf("\n\n");
	}
	//single selection sort 256
	if (allow_avx_256)
	{
		printf("for fast single selection sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastSingleSelectionSort256(data12, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data12, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d", data12[i]);
				if (data12[i] != data0[i]) {
					printf("[%d], ", data0[i]);
				}
				else
					printf(", ");
			}
		}
		printf("\n\n");
	}
	//single selection sort 512
	if (allow_avx_512)
	{
		printf("for fast single selection sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastSingleSelectionSort512(data13, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data13, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d", data13[i]);
				if (data13[i] != data0[i]) {
					printf("[%d], ", data0[i]);
				}
				else
					printf(", ");
			}
		}
		printf("\n\n");
	}
	//double selection sort
	if (allow_common)
	{
		printf("for double selection sort:\n");
		t0 = _Query_perf_counter();
		{
			DoubleSelectionSort(data14, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data14, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data14[i]);
			}
		}
		printf("\n\n");
	}
	//double selection sort 256
	if (allow_avx_256)
	{
		printf("for fast double selection sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastDoubleSelectionSort256(data15, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data15, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data15[i]);
			}
		}
		printf("\n\n");
	}
	//double selection sort 512
	if (allow_avx_512)
	{
		printf("for fast double selection sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastDoubleSelectionSort512(data16, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data16, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data16[i]);
			}
		}
		printf("\n\n");
	}
	//bubble sort
	if (allow_common)
	{
		printf("for bubble sort:\n");
		t0 = _Query_perf_counter();
		{
			BubbleSort(data17, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data17, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data17[i]);
			}
		}
		printf("\n\n");
	}
	//bubble sort 256
	if (allow_avx_256)
	{
		printf("for fast bubble sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastBubbleSort256(data18, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data18, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data18[i]);
			}
		}
		printf("\n\n");
	}
	//bubble sort 512
	if (allow_avx_512)
	{
		printf("for fast bubble sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastBubbleSort512(data19, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data19, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data19[i]);
			}
		}
		printf("\n\n");
	}
	//insertion sort
	if (allow_common)
	{
		printf("for insertion sort:\n");
		t0 = _Query_perf_counter();
		{
			InsertionSort(data20, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data20, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data20[i]);
			}
		}
		printf("\n\n");
	}
	//fast insertion sort 256
	if (allow_avx_256)
	{
		printf("for fast insertion sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastInsertionSort256(data21, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data21, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data21[i]);
			}
		}
		printf("\n\n");
	}
	//fast insertion sort 512
	if (allow_avx_512)
	{
		printf("for fast insertion sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastInsertionSort512(data22, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data22, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data22[i]);
			}
		}
		printf("\n\n");
	}
	//heap sort
	if (allow_common)
	{
		printf("for heap sort:\n");
		t0 = _Query_perf_counter();
		{
			HeapSort(data23, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data23, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data23[i]);
			}
		}
		printf("\n\n");
	}
	//fast heap sort 256
	if (allow_avx_256)
	{
		printf("for fast heap sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastHeapSort256(data24, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data24, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data24[i]);
			}
		}
		printf("\n\n");
	}
	//fast heap sort 512
	if (allow_avx_512)
	{
		printf("for fast heap sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastHeapSort512(data25, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data25, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data25[i]);
			}
		}
		printf("\n\n");
	}
	//string index-of 256
	if (allow_common)
	{
		printf("for string index-of:\n");
		int index = 0;
		t0 = _Query_perf_counter();
		{
			index = StringIndexOf256("bacbababadababacambabacaddababacasdsd", "ababaca");
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool b = true;
		printf("correct:%s\n", b ? "true" : "false");


		printf("\n\n");
	}

	return 0;
}
