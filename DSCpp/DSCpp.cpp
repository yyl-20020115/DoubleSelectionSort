#include <iostream>
#include <intrin.h>
#include <algorithm>

void do_print(__m512i data, bool nl = false) {
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
void do_print(__m256i data, bool nl = false) {
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
void do_print(__m128i data, bool nl = false) {
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
//Fast Quick Sort:
//  using AVX512 long stride to achive a faster speed than common quick sort
//
bool FastQuickSortImpl512(int data[], int low, int high) {
    const int stride = sizeof(__m512i) / sizeof(int);
    if (low < high)
    {
        unsigned long index = 0;
        unsigned long i = low, j = high;
        int delta = 0;
        int k = data[i];
        __m512i t = _mm512_set1_epi32(data[i]);
        while (i < j)
        {
            while (i < j)
            {
                __m512i ds = _mm512_loadu_epi32(data + j - stride + 1);
                __mmask16 rt = _mm512_cmple_epi32_mask(ds, t);
                
                if (rt == 0) {
                    j = j - stride < i ? i : j - stride;
                }
                else {
                    if (_BitScanReverse(&index, rt)) {
                        delta = stride - index - 1;
                        j = j - delta < i ? i : j - delta;
                    }
                    break;
                }
            }
            
            if (i < j)
                data[i++] = data[j];

            while (i < j)
            {
                __m512i ds = _mm512_loadu_epi32(data + i);
                __mmask16 rt = _mm512_cmpgt_epi32_mask(ds, t);
                if (rt == 0) {
                    i = i + stride > j ? j : i + stride;
                }
                else {
                    if (_BitScanForward(&index, rt)) {
                        i = i + index > j ? j : i + index;
                    }
                    break;
                }
            }
            if (i < j)
                data[j--] = data[i];
        }
        data[i] = k;
        FastQuickSortImpl512(data, low, i - 1);
        FastQuickSortImpl512(data, i + 1, high);
    }
    return true;
}

bool FastQuickSort512(int data[], int n) 
{
    const int stride = sizeof(__m512i) / sizeof(int);
    if (n % stride > 0) return false;

    return FastQuickSortImpl512(data, 0, n - 1);
}
bool FastQuickSortImpl256(int data[], int low, int high) {
    const int stride = sizeof(__m256i) / sizeof(int);
    if (low < high)
    {
        unsigned long index = 0;
        unsigned long i = low, j = high;
        int delta = 0;
        int k = data[i];
        __m256i t = _mm256_set1_epi32(data[i]);
        while (i < j)
        {
            while (i < j)
            {
                __m256i ds = _mm256_loadu_epi32(data + j - stride + 1);
                __mmask16 rt = _mm256_cmple_epi32_mask(ds, t);

                if (rt == 0) {
                    j = j - stride < i ? i : j - stride;
                }
                else {
                    if (_BitScanReverse(&index, rt)) {
                        delta = stride - index - 1;
                        j = j - delta < i ? i : j - delta;
                    }
                    break;
                }
            }

            if (i < j)
                data[i++] = data[j];

            while (i < j)
            {
                __m256i ds = _mm256_loadu_epi32(data + i);
                __mmask16 rt = _mm256_cmpgt_epi32_mask(ds, t);
                if (rt == 0) {
                    i = i + stride > j ? j : i + stride;
                }
                else {
                    if (_BitScanForward(&index, rt)) {
                        i = i + index > j ? j : i + index;
                    }
                    break;
                }
            }
            if (i < j)
                data[j--] = data[i];
        }
        data[i] = k;
        FastQuickSortImpl256(data, low, i - 1);
        FastQuickSortImpl256(data, i + 1, high);
    }
    return true;
}

bool FastQuickSort256(int data[], int n)
{
    const int stride = sizeof(__m256i) / sizeof(int);
    if (n % stride > 0) return false;

    return FastQuickSortImpl512(data, 0, n - 1);
}
inline int HorizentalMin16(__m128i data, unsigned short* p) {
    __m128i result = _mm_minpos_epu16(data);
    if (p != 0) {
        *p = result.m128i_i16[0];
    }
    return result.m128i_i16[1] & 0x7;
}
inline int HorizentalMax16(__m128i data, unsigned short* p) {
    __m128i zero = _mm_setzero_si128();
    __m128i ones = _mm_cmpeq_epi32(zero, zero);
    __m128i subs = _mm_subs_epu16(ones, data);

    __m128i result = _mm_minpos_epu16(subs);
    if (p != 0) {
        *p = (0xffff - result.m128i_i16[0]);
    }
    return result.m128i_i16[1] & 0x7;
}
inline int HorizentalMin32(__m256i data, unsigned int* p) {
    __m128i zero = _mm_setzero_si128();
    __m128i ones = _mm_cmpeq_epi32(zero, zero);
    __m256i idx = _mm256_setr_epi16(0, 2, 4, 6, 8, 10, 12, 14,1,3,5,7,9,11,13,15);
    __m256i r = _mm256_permutexvar_epi16(idx, data);
    __m128i low = _mm256_extracti128_si256(r, 0);
    __m128i high = _mm256_extracti128_si256(r, 1);
    __m128i higher_value = _mm_minpos_epu16(high);
    __m128i t = _mm_set1_epi16(higher_value.m128i_i16[0]);
    __m128i mask = _mm_cmpeq_epi16(high, t);
    __m128i _low = _mm_and_si128(low, mask);
    __m128i _low_sub = _mm_sub_epi16(zero, _low);
    __m128i _low_sub_inv = _mm_andnot_si128(_low_sub, ones);
    __m128i lower_value = _mm_minpos_epu16(_low_sub_inv);
    unsigned short lower_index = lower_value.m128i_i16[1]; 
    if (p != 0) {
        *p = (r.m256i_i16[lower_index]) | (higher_value.m128i_i16[0] << 16);
    }
    return lower_index+8;
}
inline int HorizentalMax32(__m256i data, unsigned int* p) {
    __m128i zero = _mm_setzero_si128();
    __m128i ones = _mm_cmpeq_epi32(zero, zero);
    __m256i zero_ = _mm256_setzero_si256();
    __m256i ones_ = _mm256_cmpeq_epi32(zero_, zero_);
    __m256i _data_ = _mm256_sub_epi32(ones_, data); 
    __m256i idx = _mm256_setr_epi16(0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15);
    __m256i r = _mm256_permutexvar_epi16(idx, _data_);
    __m128i low = _mm256_extracti128_si256(r, 0);
    __m128i high = _mm256_extracti128_si256(r, 1);
    __m128i higher_value = _mm_minpos_epu16(high);
    __m128i t = _mm_set1_epi16(higher_value.m128i_i16[0]);
    __m128i mask = _mm_cmpeq_epi16(high, t);
    __m128i _low = _mm_and_si128(low, mask);
    __m128i _low_sub_inv = _mm_andnot_si128(_low, ones);
    __m128i lower_value = _mm_minpos_epu16(_low_sub_inv);
    unsigned short lower_index = lower_value.m128i_i16[1];

    if (p != 0) {
        *p = (~r.m256i_i16[lower_index]) | (~higher_value.m128i_i16[0] << 16);
    }
    return 8 - 1 - lower_index;
}
inline int HorizentalMin32(__m512i data, unsigned int* p)
{
    __m256i low = _mm512_extracti32x8_epi32(data, 0);
    __m256i high = _mm512_extracti32x8_epi32(data, 1);

    unsigned int lv = 0;
    unsigned int hv = 0;

    int li = HorizentalMin32(low, &lv);
    int hi = HorizentalMin32(low, &hv);
    if (lv <= hv) {
        if (p != 0)*p = lv;
        return li;
    }
    else //hv<lv
    {
        if (p != 0)*p = hv;
        return 8 + hi;
    }
}
inline int HorizentalMax32(__m512i data, unsigned int* p)
{
    __m256i low = _mm512_extracti32x8_epi32(data, 0);
    __m256i high = _mm512_extracti32x8_epi32(data, 1);

    unsigned int lv = 0;
    unsigned int hv = 0;

    int li = HorizentalMax32(low, &lv);
    int hi = HorizentalMax32(low, &hv);
    if (lv < hv) {
        if (p != 0)*p = hv;
        return hi;
    }
    else //hv<=lv
    {
        if (p != 0)*p = lv;
        return 8 + li;
    }
}

inline int HorizentalMin64(__m128i data, unsigned long long* p)
{
    unsigned long long low = _mm_extract_epi64(data, 0);
    unsigned long long high = _mm_extract_epi64(data, 1);
    if (low <= high) {
        if (p != 0) *p = low;
        return 0;
    }
    else {
        if (p != 0) *p = high;
        return 1;
    }
}
inline int HorizentalMax64(__m128i data, unsigned long long* p)
{
    unsigned long long low = _mm_extract_epi64(data, 0);
    unsigned long long high = _mm_extract_epi64(data, 1);
    if (low >= high) {
        if (p != 0) *p = low;
        return 0;
    }
    else {
        if (p != 0) *p = high;
        return 1;
    }
}


inline int HorizentalMin64(__m256i data, unsigned long long* p)
{
    __m128i low = _mm256_extracti64x2_epi64(data, 0);
    __m128i high = _mm256_extracti64x2_epi64(data, 0);
    
    unsigned long long lv = 0;
    unsigned long long hv = 0;

    int li = HorizentalMin64(low, &lv);
    int hi = HorizentalMin64(high, &hv);
    if (lv <= hv) {
        if (p != 0) *p = lv;
        return li;
    }
    else {
        if (p != 0)*p = hv;
        return hi + 2;
    }
}

inline int HorizentalMax64(__m256i data, unsigned long long* p)
{
    __m128i low = _mm256_extracti64x2_epi64(data, 0);
    __m128i high = _mm256_extracti64x2_epi64(data, 0);

    unsigned long long lv = 0;
    unsigned long long hv = 0;

    int li = HorizentalMax64(low, &lv);
    int hi = HorizentalMax64(high, &hv);
    if (lv >= hv) {
        if (p != 0) *p = lv;
        return li;
    }
    else {
        if (p != 0)*p = hv;
        return hi + 2;
    }

}
inline int HorizentalMin64(__m512i data, unsigned long long* p)
{
    __m256i low = _mm512_extracti64x4_epi64(data, 0);
    __m256i high = _mm512_extracti64x4_epi64(data, 1);

    unsigned long long lv = 0;
    unsigned long long hv = 0;

    int li = HorizentalMin64(low, &lv);
    int hi = HorizentalMin64(high, &hv);
    if (lv <= hv) {
        if (p != 0) *p = lv;
        return li;
    }
    else {
        if (p != 0)*p = hv;
        return hi + 4;
    }
}

inline int HorizentalMax64(__m512i data, unsigned long long* p)
{
    __m256i low = _mm512_extracti64x4_epi64(data, 0);
    __m256i high = _mm512_extracti64x4_epi64(data, 1);

    unsigned long long lv = 0;
    unsigned long long hv = 0;

    int li = HorizentalMin64(low, &lv);
    int hi = HorizentalMin64(high, &hv);
    if (lv >= hv) {
        if (p != 0) *p = lv;
        return li;
    }
    else {
        if (p != 0)*p = hv;
        return hi + 4;
    }
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
    
    for (int repeat = 0; repeat < n / 2; repeat++)
    {
        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m256i veven = _mm256_i32gather_epi32( ptr, ipe, skip);
            __m256i vodd_ = _mm256_i32gather_epi32( ptr, ipo, skip);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            _mm256_i32scatter_epi32(ptr, ipe, min, skip);
            _mm256_i32scatter_epi32(ptr, ipo, max, skip);
        }
        for (int part = 0; part < n; part += dbls)
        {
            if (dbls >= n - part - 1) a[n] = a[n - 1];

            int* par = a + part;

            __m256i veven = _mm256_i32gather_epi32(par, ipt, skip);
            __m256i vodd_ = _mm256_i32gather_epi32(par, ipo, skip);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            _mm256_i32scatter_epi32(par, ipo, min, skip);
            _mm256_i32scatter_epi32(par, ipt, max, skip);
 
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

    for (int repeat = 0; repeat < n / 2; repeat++)
    {
        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m512i veven = _mm512_i32gather_epi32(ipe, ptr, skip);
            __m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, skip);

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            _mm512_i32scatter_epi32(ptr, ipe, min, skip);
            _mm512_i32scatter_epi32(ptr, ipo, max, skip);
        }
        for (int part = 0; part < n; part += dbls)
        {
            if (dbls >= n - part - 1) a[n] = a[n - 1];

            int* par = a + part;

            __m512i veven = _mm512_i32gather_epi32(ipt, par, skip);
            __m512i vodd_ = _mm512_i32gather_epi32(ipo, par, skip);

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            _mm512_i32scatter_epi32(par, ipo, min, skip);
            _mm512_i32scatter_epi32(par, ipt, max, skip);
        }
    }

    memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

    delete[] a;

    return t;

}

const int DATA_SIZE = 4096;

int data0[DATA_SIZE] = { 0 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 };
//int data0[DATA_SIZE] = { 12,2,23,25,17,1,16,19,31,19,1,23,31,19,13,25,26,14,10,28,31,25,7,14,17,3,25,24,21,22,8,14 };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };
int data4[DATA_SIZE] = { 0 };
int data5[DATA_SIZE] = { 0 };
int data6[DATA_SIZE] = { 0 };

bool CheckSequence(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}
int main()
{
    __m128i data16 = _mm_set_epi16(5, 3, 1, 4, 2, 9, 7, 6); //0 is last
    __m256i data32 = _mm256_set_epi32(
        0x00050055, 
        0x00240044, 
        0x00060066, 
        0x00090099,
        0x00010033,
        0x00020011,
        0x00170077,
        0x00030033
        );
    unsigned short r16 = 0;
    unsigned int r32 = 0;
    unsigned long long r64 = 0;
    int i = 0;
    i = HorizentalMin16(data16, &r16);

    i = HorizentalMax16(data16, &r16);

    i = HorizentalMin32(data32, &r32);

    i = HorizentalMax32(data32, &r32);

    i = HorizentalMin64(data32, &r64);

    i = HorizentalMax64(data32, &r64);

    bool show = false;
    long long t0;
    srand((unsigned)time(0));
    {
        printf("original data(count = %d):\n",DATA_SIZE);
        for (int i = 0; i < DATA_SIZE; i++) {
            data6[i]
                = data5[i]
                = data4[i]
                = data3[i]
                = data2[i]
                = data1[i]
                = data0[i] //
                = (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
            if (show) {
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
        if (show) {
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

    if(true)
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

    if (true)
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

    if(true)
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
