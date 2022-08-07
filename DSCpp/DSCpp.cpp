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

void QuickSort(int* data, int low, int high)
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
        QuickSort(data, low, i - 1);
        QuickSort(data, i + 1, high);
    }
}

typedef union _m256i_m128i
{
    __m256i m256;
    struct _m128s
    {
        __m128i low;
        __m128i high;
    } m128s;

}__m256i_m128i;

const int po256[] = { 1, 3, 5, 7, 9, 11, 13, 15 };
const int pe256[] = { 0, 2, 4, 6, 8, 10, 12, 14 };
const int pt256[] = { 2, 4, 6, 8, 10, 12, 14, 16 };

int* FastOddEvenSort256(int* t, int n)
{
    __m256i_m128i mixl = { 0 };
    __m256i_m128i mixh = { 0 };

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
        bool any_even_set = false;
        bool any_odd_set = false;

        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m256i veven = _mm256_i32gather_epi32( ptr, ipe, skip);
            __m256i vodd_ = _mm256_i32gather_epi32( ptr, ipo, skip);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            __m256i upl = _mm256_unpacklo_epi32(min, max);
            __m256i uph = _mm256_unpackhi_epi32(min, max);

            _mm256_storeu_epi32(&mixl, upl);
            _mm256_storeu_epi32(&mixh, uph);

            __m128i upll = mixl.m128s.low;
            __m128i uplh = mixl.m128s.high;

            __m128i uphl = mixh.m128s.low;
            __m128i uphh = mixh.m128s.high;

            mixl.m128s.low = upll;
            mixl.m128s.high = uphl;
            mixh.m128s.low = uplh;
            mixh.m128s.high = uphh;

            {
                _mm256_storeu_si256((__m256i*)(ptr + 0), mixl.m256);
                _mm256_storeu_si256((__m256i*)(ptr + size), mixh.m256);
                any_even_set |= true;
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

            __m256i veven = _mm256_i32gather_epi32(ptr, ipt, skip);
            __m256i vodd_ = _mm256_i32gather_epi32(ptr, ipo, skip);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            __m256i upl = _mm256_unpacklo_epi32(min, max);
            __m256i uph = _mm256_unpackhi_epi32(min, max);

            _mm256_storeu_epi32(&mixl, upl);
            _mm256_storeu_epi32(&mixh, uph);

            __m128i upll = mixl.m128s.low;
            __m128i uplh = mixl.m128s.high;

            __m128i uphl = mixh.m128s.low;
            __m128i uphh = mixh.m128s.high;

            mixl.m128s.low = upll;
            mixl.m128s.high = uphl;
            mixh.m128s.low = uplh;
            mixh.m128s.high = uphh;

            int* pur = (islast ? par + 1 : ptr + 1);
            {
                _mm256_storeu_si256((__m256i*)(pur + 0), mixl.m256);
                _mm256_storeu_si256((__m256i*)(pur + size), mixh.m256);
                any_odd_set |= true;
            }
        }
        if (!(any_odd_set || any_even_set)) break;
    }

    memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

    delete[] a;

    return t;
}


typedef union _m512i_m256i 
{
    __m512i m512;
    struct _m256s
    {
        __m256i low;
        __m256i high;
    } m256s;

}__m512i_m256i;

const int po512[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31 };
const int pe512[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
const int pt512[] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };


__m256i Make256(__m128i& low, __m128i& high) {
    __m256i_m128i m = { 0 };
    m.m128s.low = low;
    m.m128s.high = high;
    return m.m256;
}
__m512i Make512(__m256i& low, __m256i& high) {
    __m512i_m256i m = { 0 };
    m.m256s.low = low;
    m.m256s.high = high;
    return m.m512;
}

int* FastOddEvenSort512(int* t, int n)
{
    __m512i_m256i mixl = { 0 };
    __m512i_m256i mixh = { 0 };

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
        bool any_even_set = false;
        bool any_odd_set = false;

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

                any_even_set |= true;
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

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            __m512i upl = _mm512_unpacklo_epi32(min, max);
            __m512i uph = _mm512_unpackhi_epi32(min, max);

            __m256i upll = _mm512_extracti32x8_epi32(upl, 0);
            __m256i uplh = _mm512_extracti32x8_epi32(upl, 1);
            __m256i uphl = _mm512_extracti32x8_epi32(uph, 0);
            __m256i uphh = _mm512_extracti32x8_epi32(uph, 1);

            ptr = pur;
            {
                _mm_storeu_epi32((ptr + 0), _mm256_extracti128_si256(upll, 0));
                _mm_storeu_epi32((ptr + 4), _mm256_extracti128_si256(uphl, 0));
                _mm_storeu_epi32((ptr + 8), _mm256_extracti128_si256(upll, 1));
                _mm_storeu_epi32((ptr + 12), _mm256_extracti128_si256(uphl, 1));
                _mm_storeu_epi32((ptr + 16), _mm256_extracti128_si256(uplh, 0));
                _mm_storeu_epi32((ptr + 20), _mm256_extracti128_si256(uphh, 0));
                _mm_storeu_epi32((ptr + 24), _mm256_extracti128_si256(uplh, 1));
                _mm_storeu_epi32((ptr + 28), _mm256_extracti128_si256(uphh, 1));
                any_odd_set |= true;
            }
        }
        if (!(any_odd_set || any_even_set)) break;
    }

    memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

    delete[] a;

    return t;

}

const int DATA_SIZE = 4096;

//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 };
int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };
int data4[DATA_SIZE] = { 0 };

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
            data4[i]
                = data3[i]
                = data2[i]
                = data1[i]
                = data0[i] //;
                = (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
            if (false) {
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
        if (false) {
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
            QuickSort(data1, 0, DATA_SIZE - 1);
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
