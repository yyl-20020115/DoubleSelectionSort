#include <iostream>
#include <intrin.h>
#include <algorithm>

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

void FastOddEvenSort256(int* a, int n)
{
    __m256i zeros = _mm256_setzero_si256();
    __m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
    __m256i ipo = _mm256_loadu_epi32(po256);
    __m256i ipe = _mm256_loadu_epi32(pe256);
    __m256i ipt = _mm256_loadu_epi32(pt256);
    __m256i_m128i mix = { 0 };

    const int size = sizeof(mix) / sizeof(int);
    const int last = size - 1;
    const int half = size >> 1;
    const int dbls = (size << 1);
    const int quad = dbls << 1;

    int buff[2 * dbls] = { 0 };

    for (int repeat = 0; repeat < n / 2; repeat++)
    {
        bool any_even_set = false;
        bool any_odd_set = false;

        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m256i veven = _mm256_i32gather_epi32( ptr, ipe, half);
            __m256i vodd_ = _mm256_i32gather_epi32( ptr, ipo, half);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            __m256i upl = _mm256_unpacklo_epi32(min, max);
            _mm256_storeu_epi32(&mix, upl);
            __m128i upll = mix.m128s.low;
            __m128i uplh = mix.m128s.high;

            __m256i uph = _mm256_unpackhi_epi32(min, max);
            _mm256_storeu_epi32(&mix, uph);
            __m128i uphl = mix.m128s.low;
            __m128i uphh = mix.m128s.high;

            mix.m128s.low = upll;
            mix.m128s.high = uphl;

            __m256i uplx = mix.m256;

            mix.m128s.low = uplh;
            mix.m128s.high = uphh;

            __m256i uphx = mix.m256;

            __m256i lx = _mm256_lddqu_si256((__m256i*)(ptr + 0));
            __m256i hx = _mm256_lddqu_si256((__m256i*)(ptr + size));
            
            if (!_mm256_testz_si256(_mm256_sub_epi32(uplx, lx), ones))
            {
                _mm256_storeu_si256((__m256i*)ptr, uplx);
                any_even_set |= true;
            }

            if (!_mm256_testz_si256(_mm256_sub_epi32(uphx, hx), ones))
            {
                _mm256_storeu_si256((__m256i*)(ptr + size), uphx);
                any_even_set |= true;
            }
        }
        for (int part = 0; part < n; part += dbls)
        {
            bool islast = dbls >= n - part - 1;

            //this is the last
            if (islast)
            {
                memcpy_s(buff, sizeof(buff), a + part, dbls * sizeof(int));
                buff[dbls] = buff[dbls - 1];
            }

            int* par = a + part;
            int* ptr = islast ? buff : par;

            __m256i veven = _mm256_i32gather_epi32(ptr, ipt, half);
            __m256i vodd_ = _mm256_i32gather_epi32(ptr, ipo, half);

            __m256i min = _mm256_min_epi32(vodd_, veven);
            __m256i max = _mm256_max_epi32(vodd_, veven);

            __m256i upl = _mm256_unpacklo_epi32(min, max);
            _mm256_storeu_epi32(&mix, upl);
            __m128i upll = mix.m128s.low;
            __m128i uplh = mix.m128s.high;

            __m256i uph = _mm256_unpackhi_epi32(min, max);
            _mm256_storeu_epi32(&mix, uph);
            __m128i uphl = mix.m128s.low;
            __m128i uphh = mix.m128s.high;

            mix.m128s.low = upll;
            mix.m128s.high = uphl;

            __m256i uplx = mix.m256;

            mix.m128s.low = uplh;
            mix.m128s.high = uphh;

            __m256i uphx = mix.m256;
            __m256i uphc = uphx;

            __m256i lx = _mm256_lddqu_si256((__m256i*)(ptr + 0));
            __m256i hx = _mm256_lddqu_si256((__m256i*)(ptr + size));

            if (islast) {
                uphc.m256i_u32[size - 1] = 0;
                hx.m256i_u32[size - 1] = 0;
            }

            if (!_mm256_testz_si256(_mm256_sub_epi32(uplx, lx), ones))
            {
                _mm256_storeu_si256((__m256i*)(a + part + 1), uplx);
                any_odd_set |= true;
            }
            if (!_mm256_testz_si256(_mm256_sub_epi32(uphc, hx), ones))
            {
                any_odd_set |= true;

                _mm256_storeu_si256((__m256i*)(buff), uphx);
                if (islast) {
                    memcpy_s(a + part + 1,
                        (dbls - 2) * sizeof(int),
                        buff, 
                        (dbls - 2) * sizeof(int)
                    );
                }
                else {
                    memcpy_s(a + part + 1,
                        dbls * sizeof(int),
                        buff,
                        dbls * sizeof(int)
                    );
                }            
            }
        }
        if (!(any_odd_set || any_even_set)) break;
    }
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

//TODO: working on it
void FastOddEvenSort512(int* a, int n)
{
    __m256i zeros = _mm256_setzero_si256();
    __m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
    __m512i ipo = _mm512_loadu_epi32(po512);
    __m512i ipe = _mm512_loadu_epi32(pe512);
    __m512i ipt = _mm512_loadu_epi32(pt512);
    __m512i_m256i mix = { 0 };

    const int size = sizeof(mix) / sizeof(int);
    const int last = size - 1;
    const int half = size >> 1;
    const int dbls = (size << 1);
    const int quad = dbls << 1;

    int buff[2 * dbls] = { 0 };

    for (int repeat = 0; repeat < n / 2; repeat++)
    {
        bool any_even_set = false;
        bool any_odd_set = false;

        for (int part = 0; part < n; part += dbls)
        {
            int* ptr = a + part;

            __m512i veven = _mm512_i32gather_epi32(ipe, ptr, half);
            __m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, half);

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            __m512i upl = _mm512_unpacklo_epi32(min, max);
            _mm512_store_epi32((__m512i*)(& mix),upl);
            __m256i upll = mix.m256s.low;
            __m256i uplh = mix.m256s.high;

            __m512i uph = _mm512_unpackhi_epi32(min, max);
            _mm512_store_epi32((__m512i*)(&mix), uph);
            __m256i uphl = mix.m256s.low;
            __m256i uphh = mix.m256s.high;

            mix.m256s.low = upll;
            mix.m256s.high = uphl;

            __m512i uplx = mix.m512;

            mix.m256s.low = uplh;
            mix.m256s.high = uphh;

            __m512i uphx = mix.m512;

            __m256i fl = _mm256_lddqu_si256((__m256i*)(ptr + 0));
            __m256i fh = _mm256_lddqu_si256((__m256i*)(ptr + half));
            __m256i ll = _mm256_lddqu_si256((__m256i*)(ptr + size));
            __m256i lh = _mm256_lddqu_si256((__m256i*)(ptr + size + half));

            if (!_mm256_testz_si256(_mm256_cmpeq_epi32(upll, fl), ones)
                || !_mm256_testz_si256(_mm256_cmpeq_epi32(uplh, fh), ones))
            {
                _mm512_storeu_si512(ptr, uplx);
                any_even_set |= true;
            }

            if (!_mm256_testz_si256(_mm256_cmpeq_epi32(uphl, ll), ones)
                || !_mm256_testz_si256(_mm256_cmpeq_epi32(uphh, lh), ones))
            {
                _mm512_storeu_si512(ptr + size, uphx);
                any_even_set |= true;
            }
        }
        for (int part = 0; part < n; part += dbls)
        {
            bool islast = dbls >= n - part - 1;

            //this is the last
            if (islast)
            {
                memcpy_s(buff, sizeof(buff), a + part, dbls * sizeof(int));
                buff[dbls] = buff[dbls - 1];
            }

            int* par = a + part;
            int* ptr = islast ? buff : par;
            int* pur = (islast ? par + 1 : ptr + 1);

            __m512i veven = _mm512_i32gather_epi32(ipt, ptr, half);
            __m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, half);

            __m512i min = _mm512_min_epi32(vodd_, veven);
            __m512i max = _mm512_max_epi32(vodd_, veven);

            __m512i upl = _mm512_unpacklo_epi32(min, max);
            _mm512_store_epi32((__m512i*)(&mix), upl);
            __m256i upll = mix.m256s.low;
            __m256i uplh = mix.m256s.high;

            __m512i uph = _mm512_unpackhi_epi32(min, max);
            _mm512_store_epi32((__m512i*)(&mix), uph);
            __m256i uphl = mix.m256s.low;
            __m256i uphh = mix.m256s.high;

            mix.m256s.low = upll;
            mix.m256s.high = uphl;

            __m512i uplx = mix.m512;

            mix.m256s.low = uplh;
            mix.m256s.high = uphh;

            __m512i uphx = mix.m512;

            __m256i fl = _mm256_lddqu_si256((__m256i*)(ptr + 0));
            __m256i fh = _mm256_lddqu_si256((__m256i*)(ptr + half));
            __m256i ll = _mm256_lddqu_si256((__m256i*)(ptr + size));
            __m256i lh = _mm256_lddqu_si256((__m256i*)(ptr + size + half));

            if (!_mm256_testz_si256(_mm256_cmpeq_epi32(upll, fl), ones)
                || !_mm256_testz_si256(_mm256_cmpeq_epi32(uplh, fh), ones))
            {
                _mm512_storeu_si512(pur, uplx);
                any_odd_set |= true;
            }
            if (!_mm256_testz_si256(_mm256_cmpeq_epi32(uphl, ll), ones)
                || !_mm256_testz_si256(_mm256_cmpeq_epi32(uphh, lh), ones))
            {
                _mm512_storeu_si512(pur + size, uphx);
                any_odd_set |= true;
                //TODO:
            }
        }
        if (!(any_odd_set || any_even_set)) break;
    }
}

const int DATA_SIZE = 128;

int data0[DATA_SIZE] = { 0 };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };

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
        printf("original data:\n");
        for (int i = 0; i < DATA_SIZE; i++) {
            data3[i]
                = data2[i]
                = data1[i]
                = data0[i]
                = (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
            printf("%d ", data0[i]);
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

        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d ", data0[i]);
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

        printf("correct:%s\n", CheckSequence(data0, data1, DATA_SIZE) ? "true" : "false");
        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d ", data1[i]);
        }
        printf("\n\n");
    }


    {
        t0 = _Query_perf_counter();
        printf("for odd even sort:\n");
        {
            OddEvenSort(data1, DATA_SIZE);
            //        QuickSort(data1, 0, DATA_SIZE);
        }
        printf("time:%lf(ms)\n",
            ((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

        printf("correct:%s\n", CheckSequence(data0, data1, DATA_SIZE) ? "true" : "false");
        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d ", data1[i]);
        }
        printf("\n\n");
    }

    {
        printf("for fast odd even sort 256:\n");
        t0 = _Query_perf_counter();
        {
            FastOddEvenSort256(data2, DATA_SIZE);
        }
        printf("time:%lf(ms)\n",
            ((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
        printf("correct:%s\n", CheckSequence(data0, data2, DATA_SIZE) ? "true" : "false");

        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d ", data2[i]);
        }
        printf("\n\n");

    }


    {
        printf("for fast odd even sort 512:\n");
        t0 = _Query_perf_counter();
        {
            FastOddEvenSort512(data3, DATA_SIZE);
        }
        printf("time:%lf(ms)\n",
            ((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

        printf("correct:%s\n", CheckSequence(data0, data2, DATA_SIZE) ? "true" : "false");

        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d ", data2[i]);
        }
        printf("\n\n");
    }
    return 0;
}
