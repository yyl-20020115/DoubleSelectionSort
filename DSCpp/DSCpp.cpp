#include <iostream>
#include <intrin.h>
#include <algorithm>

void do_print(__m512i& data, bool nl = false) {
    bool first = true;
    printf("[");
    for (int i = 0; i < sizeof(data.m512i_u32)/sizeof(int); i++) {
        if (!first) {
            printf(", ");
        }
        printf("%08X",data.m512i_i32[i]);
        first = false;
    }
    printf("]");
    if (nl)printf("\n");
}
void do_print(__m256i& data, bool nl = false) {
    bool first = true;
    printf("[");
    for (int i = 0; i < sizeof(data.m256i_u32) / sizeof(int); i++) {
        if (!first) {
            printf(", ");
        }
        printf("%08X", data.m256i_u32[i]);
        first = false;
    }
    printf("]");
    if (nl)printf("\n");
}
void do_print(__m128i& data, bool nl = false) {
    bool first = true;
    printf("[");
    for (int i = 0; i < sizeof(data.m128i_u32) / sizeof(int); i++) {
        if (!first) {
            printf(", ");
        }
        printf("%08X", data.m128i_u32[i]);
        first = false;
    }
    printf("]");
    if (nl)printf("\n");
}
void do_print(int data[], int N, bool nl = false) {
    bool first = true;
    printf("[");
    for (int i = 0; i < N; i++) {
        if (!first) {
            printf(", ");
        }
        printf("%08X", data[i]);
        first = false;
    }
    printf("]");
    if (nl)printf("\n");
}


void Swap(int* a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void OddEvenSort(int* a,int n)
{
    for (int c = 0; c < n; c++)
    {
        int p = c % 2;
        for (int d = 0; d < n - p - p; d += 2)
        {
            //this is for odd length
            if (d + p + 1 >= n) break;
            //this step should be in parallel
            if (a[d + p + 0] > a[d + p + 1])
            {
                Swap(a, d + p + 0, d + p + 1);
            }
        }
    }
}

void QuickSortImpl(int* data, int low, int high)
{
    if (low < high)
    {
        int i = low, j = high;
        int k = data[i];
        while (i < j)
        {
            while (i < j && data[j] > k)
                --j;
            if (i < j)
                data[i++] = data[j];
            while (i < j && data[i] <= k)
                ++i;
            if (i < j)
                data[j--] = data[i];
        }
        data[i] = k;
        QuickSortImpl(data, low, i - 1);
        QuickSortImpl(data, i + 1, high);
    }
}
void QuickSort(int data[], int n)
{
    QuickSortImpl(data, 0, n - 1);
}

bool FastQuickSortImpl(int data[], int low, int high) {
    const int stride = sizeof(__m512i) / sizeof(int);
    int length = high - low + 1;
    if (length % stride > 0) return false;
 
    //do_print(data, length,true);
    
    if (low < high)
    {
        unsigned long index = 0;
        int i = low, j = high;
        int k = data[i];
        __m512i t = _mm512_set1_epi32(data[i]);
        while (i < j)
        {
            while (i < j)
            {
                //&& data[j] > k
                __m512i ds = _mm512_loadu_epi32(data + j - stride + 1);
                __mmask16 rt = _mm512_cmple_epi32_mask(ds, t);
                //for (int p = 0; p < 16; p++) {
                //    int* u = data + j - stride + p + 1;
                //    if (rt & (1 << p)) {
                //        printf("%d %d %08X<k(%d)\n",p, *u, *u, k);
                //    }
                //    else {
                //        printf("%d %d %08X>=k(%d)\n", p, *u, *u, k);
                //    }
                //}

                if (rt == 0) {
                    j -= stride;
                }
                else {
                    if (_BitScanReverse(&index, rt)) {
                        j -= (stride - index - 1);
                        break;
                    }
                }

            }
            if (i < j)
                data[i++] = data[j];

            while (i < j)
            {
                //&& data[i] <= k
                __m512i ds = _mm512_loadu_epi32(data + i);
                __mmask16 rt = _mm512_cmpgt_epi32_mask(ds, t);
                if (rt == 0) {
                    i += stride;
                }
                else {
                    if (_BitScanForward(&index, rt)) {
                        i += index;
                        break;
                    }
                }
            }
            if (i < j)
                data[j--] = data[i];
        }
        data[i] = k;
        FastQuickSortImpl(data, low, i - 1);
        FastQuickSortImpl(data, i + 1, high);
    }
    return true;
}

bool FastQuickSort(int data[], int n) 
{
    return FastQuickSortImpl(data, 0, n - 1);
}

const int po256[] = { 1, 3, 5, 7, 9, 11, 13, 15 };
const int pe256[] = { 0, 2, 4, 6, 8, 10, 12, 14 };
const int pt256[] = { 2, 4, 6, 8, 10, 12, 14, 16 };

int* FastOddEvenSort256(int* t, int n)
{
    const int size = sizeof(__m256i) / sizeof(int);
    const int last = size - 1;
    const int half = size >> 1;
    const int skip = sizeof(int);
    const int dbls = (size << 1);
    const int quad = dbls << 1;

    if (n < dbls || n % dbls>0) {
        std::sort(t, t + n);
        return t;
    }
    int* a = new int[n + 1];
    memcpy_s(a, (n) * sizeof(int), t, (n) * sizeof(int));
    a[n] = 0;
    
    __m256i zeros = _mm256_setzero_si256();
    __m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
    __m256i ipo = _mm256_loadu_epi32(po256);
    __m256i ipe = _mm256_loadu_epi32(pe256);
    __m256i ipt = _mm256_loadu_epi32(pt256);

    int buff[2 * dbls + 2] = { 0 };
    
    for (int repeat = 0; repeat < n / 2; repeat++)
    {
        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m256i veven = _mm256_i32gather_epi32( ptr, ipe, skip);
            __m256i vodd_ = _mm256_i32gather_epi32( ptr, ipo, skip);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            __m256i upl = _mm256_unpacklo_epi32(min, max);
            __m256i uph = _mm256_unpackhi_epi32(min, max);

            {
                _mm_storeu_epi32((ptr + 0), _mm256_extracti128_si256(upl, 0));
                _mm_storeu_epi32((ptr + 4), _mm256_extracti128_si256(uph, 0));
                _mm_storeu_epi32((ptr + 8), _mm256_extracti128_si256(upl, 1));
                _mm_storeu_epi32((ptr + 12), _mm256_extracti128_si256(uph, 1));
            }
        }
        for (int part = 0; part < n; part += dbls)
        {
            bool islast = dbls >= n - part - 1;

            if (islast)
            {
                memcpy_s(buff, sizeof(buff), a + part, dbls * sizeof(int));
                buff[dbls] = buff[dbls - 1];
            }
            int* par = a + part;

            int* ptr = islast ? buff : par;
            int* pur = (islast ? par + 1 : ptr + 1);

            __m256i veven = _mm256_i32gather_epi32(ptr, ipt, skip);
            __m256i vodd_ = _mm256_i32gather_epi32(ptr, ipo, skip);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            __m256i upl = _mm256_unpacklo_epi32(min, max);
            __m256i uph = _mm256_unpackhi_epi32(min, max);

            ptr = pur;
            {
                _mm_storeu_epi32((ptr + 0), _mm256_extracti128_si256(upl, 0));
                _mm_storeu_epi32((ptr + 4), _mm256_extracti128_si256(uph, 0));
                _mm_storeu_epi32((ptr + 8), _mm256_extracti128_si256(upl, 1));
                _mm_storeu_epi32((ptr + 12), _mm256_extracti128_si256(uph, 1));
            }
        }
    }

    memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

    delete[] a;

    return t;
}

const int po512[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31 };
const int pe512[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
const int pt512[] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };

int* FastOddEvenSort512(int* t, int n)
{
    const int size = sizeof(__m512i) / sizeof(int);
    const int last = size - 1;
    const int half = size >> 1;
    const int skip = sizeof(int);
    const int dbls = (size << 1);
    const int quad = dbls << 1;

    if (n < dbls || n % dbls>0) {
        std::sort(t, t + n);
        return t;
    }
    int* a = new int[n + 1];
    memcpy_s(a, (n) * sizeof(int), t, (n) * sizeof(int));
    a[n] = 0;

    __m256i zeros = _mm256_setzero_si256();
    __m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
    __m512i ipo = _mm512_loadu_epi32(po512);
    __m512i ipe = _mm512_loadu_epi32(pe512);
    __m512i ipt = _mm512_loadu_epi32(pt512);

    int buff[2 * dbls + 2] = { 0 };

    for (int repeat = 0; repeat < n / 2; repeat++)
    {
        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m512i veven = _mm512_i32gather_epi32(ipe, ptr, skip);
            __m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, skip);

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            __m512i upl = _mm512_unpacklo_epi32(min, max);
            __m512i uph = _mm512_unpackhi_epi32(min, max);

            __m256i upll = _mm512_extracti32x8_epi32(upl, 0);
            __m256i uplh = _mm512_extracti32x8_epi32(upl, 1);
            __m256i uphl = _mm512_extracti32x8_epi32(uph, 0);
            __m256i uphh = _mm512_extracti32x8_epi32(uph, 1);

            
            {
                _mm_storeu_epi32((ptr + 0), _mm256_extracti128_si256(upll, 0));
                _mm_storeu_epi32((ptr + 4), _mm256_extracti128_si256(uphl, 0));
                _mm_storeu_epi32((ptr + 8), _mm256_extracti128_si256(upll, 1));
                _mm_storeu_epi32((ptr + 12), _mm256_extracti128_si256(uphl, 1));
                _mm_storeu_epi32((ptr + 16), _mm256_extracti128_si256(uplh, 0));
                _mm_storeu_epi32((ptr + 20), _mm256_extracti128_si256(uphh, 0));
                _mm_storeu_epi32((ptr + 24), _mm256_extracti128_si256(uplh, 1));
                _mm_storeu_epi32((ptr + 28), _mm256_extracti128_si256(uphh, 1));
            }
        }
        for (int part = 0; part < n; part += dbls)
        {
            bool islast = dbls >= n - part - 1;

            if (islast)
            {
                memcpy_s(buff, sizeof(buff), a + part, dbls * sizeof(int));
                buff[dbls] = buff[dbls - 1];
            }

            int* par = a + part;
            int* ptr = islast ? buff : par;
            int* pur = (islast ? par + 1 : ptr + 1);

            __m512i veven = _mm512_i32gather_epi32(ipt, ptr, skip);
            __m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, skip);

            ptr = pur;

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            __m512i upl = _mm512_unpacklo_epi32(min, max);
            __m512i uph = _mm512_unpackhi_epi32(min, max);

            __m256i upll = _mm512_extracti32x8_epi32(upl, 0);
            __m256i uplh = _mm512_extracti32x8_epi32(upl, 1);
            __m256i uphl = _mm512_extracti32x8_epi32(uph, 0);
            __m256i uphh = _mm512_extracti32x8_epi32(uph, 1);

            {
                _mm_storeu_epi32((ptr + 0), _mm256_extracti128_si256(upll, 0));
                _mm_storeu_epi32((ptr + 4), _mm256_extracti128_si256(uphl, 0));
                _mm_storeu_epi32((ptr + 8), _mm256_extracti128_si256(upll, 1));
                _mm_storeu_epi32((ptr + 12), _mm256_extracti128_si256(uphl, 1));
                _mm_storeu_epi32((ptr + 16), _mm256_extracti128_si256(uplh, 0));
                _mm_storeu_epi32((ptr + 20), _mm256_extracti128_si256(uphh, 0));
                _mm_storeu_epi32((ptr + 24), _mm256_extracti128_si256(uplh, 1));
                _mm_storeu_epi32((ptr + 28), _mm256_extracti128_si256(uphh, 1));
            }
        }
    }

    memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

    delete[] a;

    return t;

}

const int DATA_SIZE = 32;

//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, };
int data0[DATA_SIZE] = { 12,2,23,25,17,1,16,19,31,19,1,23,31,19,13,25,26,14,10,28,31,25,7,14,17,3,25,24,21,22,8,14 };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };
int data4[DATA_SIZE] = { 0 };
int data5[DATA_SIZE] = { 0 };

bool CheckSequence(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}
int main()
{
    long long t0;
    srand((unsigned)time(0));
    {
        printf("original data(count = %d):\n",DATA_SIZE);
        for (int i = 0; i < DATA_SIZE; i++) {
            data5[i]
                = data4[i]
                = data3[i]
                = data2[i]
                = data1[i]
                = data0[i] //
                ;
                //= (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
            if (true) {
                printf("%d ", data0[i]);
            }
        }
        printf("\n\n");
    }

    {
        printf("for system quick sort:\n");
        t0 = _Query_perf_counter();
        {
            std::sort(data0, data0 + DATA_SIZE);
        }
        printf("time:%lf(ms)\n",
            ((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
        if (true) {
            for (int i = 0; i < DATA_SIZE; i++) {
                printf("%d ", data0[i]);
            }
        }
        printf("\n\n");
    }

    {
        printf("for quick sort:\n");
        t0 = _Query_perf_counter();
        {
            QuickSort(data1,DATA_SIZE);
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

    {
        printf("for fast quick sort:\n");
        t0 = _Query_perf_counter();
        {
            FastQuickSort(data5, DATA_SIZE);
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
    return 0;
}
