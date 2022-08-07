// This is the demo for Double Selection Sort algorithm
// This algorithm improves the efficiency of the selection sort algorithm up to 50%.
//
// Single Selection Sort[t=26ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Double Selection Sort[t=17ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Efficiency Boost:52.94%


using DoubleSelectionSort;
using System.Diagnostics;
using System.Numerics;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;

const int SampleDataSize = 65536;
const int VerficationRepeats = 1000;
const bool DoVerfications = false;

var watch = new Stopwatch();
var data0 = new int[SampleDataSize];

for (int i = 0; i < data0.Length; i++)
{
    data0[i] = Random.Shared.Next(data0.Length);
}
var data1 = new int[data0.Length];
var data2 = new int[data0.Length];
var data3 = new int[data0.Length];
var data4 = new int[data0.Length];
var data5 = new int[data0.Length];
var data6 = new int[data0.Length];
var data7 = new int[data0.Length];
var data8 = new int[data0.Length];
var data9 = new int[data0.Length];
var data10 = new int[data0.Length];
var data11 = new int[data0.Length];


data0.CopyTo(data1, 0);
data0.CopyTo(data2, 0);
data0.CopyTo(data3, 0);
data0.CopyTo(data4, 0);
data0.CopyTo(data5, 0);
data0.CopyTo(data6, 0);
data0.CopyTo(data7, 0);
data0.CopyTo(data8, 0);
data0.CopyTo(data9, 0);
data0.CopyTo(data10, 0);
data0.CopyTo(data11, 0);



Console.WriteLine("Array Size = {0}", SampleDataSize);

bool SequenceEqual(int[] a, int[] b)
{
    if (a == null) throw new ArgumentNullException(nameof(a));
    if (b == null) throw new ArgumentNullException(nameof(b));
    if (a.Length != b.Length) return false;
    int N = a.Length;
    int W = Vector<int>.Count;
    int R = N % W;
    int T = N - R;
    for (int i = T; i < N; i++)
    {
        if (a[i] != b[i]) return false;
    }
    for(int i = 0; i < T; i += W)
    {
        var x = new Vector<int>(a, i);
        var y = new Vector<int>(b, i);
        if (!System.Numerics.Vector.EqualsAll(x, y)) return false;
    }

    return true;
}
void Swap(int[] a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}
int[] DoCollect(int[] data, int width, bool copy_tail)
{
    if (width <= 1) return data;

    int N = data.Length;
    int R = N % width;
    int[] positions = new int[width];
    int[] result = new int[N];
    if (copy_tail && R > 0)
    {
        Array.Copy(data, N - R, result, N - R, R);
    }
    for (int i = 0; i < width; i++)
    {
        positions[i] = i;
    }
    int p = 0;
    while (p < result.Length)
    {
        int? minpos = null;
        int? minval = null;
        for (int i = 0; i < width; i++)
        {
            int index = positions[i];
            if (index >= data.Length) continue;
            int value = data[index];
            if (minval == null || value < minval)
            {
                minval = value;
                minpos = i;
            }
        }
        if (minpos is int _minpos && minval is int _minval)
        {
            positions[_minpos] += width;
            result[p++] = _minval;
        }
    }
    return result;
}


void SingleSelectionSort(int[] data)
{
    if (data == null || data.Length <= 1)
    {
        return;
    }
    else if (data.Length == 2)
    {
        int min = Math.Min(data[0], data[1]);
        int max = min == data[0] ? data[1] : data[0];
        data[0] = min;
        data[1] = max;
        return;
    }

    for (int i = 0; i < data.Length - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < data.Length; j++)
        {
            if (data[j] < data[minIndex])
            {                   
                minIndex = j;                
            }
        }
        if(i!=minIndex) Swap(data, i, minIndex);
    }
}

/// <summary>
/// Double Selection Sort Algorithm
/// Within single iteration, we get both max and min from the 
/// searching range, and then swap the min with the element
/// before the range, and swap the max with the element after 
/// the range. Therefore we can sort from both direction,
/// and get the double speed.
/// </summary>
void DoubleSelectionSort(int[] data)
{
    if (data == null || data.Length <= 1)
    {
        return;
    }
    else if(data.Length == 2)
    {
        int min = Math.Min(data[0], data[1]);
        int max = min == data[0] ? data[1] : data[0];
        data[0] = min;
        data[1] = max;
        return;
    }
    int staIndex = 0;
    int endIndex = data.Length - 1;
    while (staIndex < endIndex)
    {
        int minIndex = staIndex;
        int maxIndex = endIndex;

        int staValue = data[staIndex];
        int endValue = data[endIndex];
        int minValue = data[minIndex];
        int maxValue = data[maxIndex];

        for (int j = staIndex; j <= endIndex; j++)
        {
            if (data[j] < minValue)
            {
                minValue = data[j];
                minIndex = j;
            }
            if (data[j] > maxValue)
            {
                maxValue = data[j];
                maxIndex = j;
            }
        }
        if (minValue == maxValue)
        {
            break;
        }
        else if (maxIndex == staIndex 
              && minIndex == endIndex)
        {
            data[staIndex] = minValue;
            data[endIndex] = maxValue;
        }
        else if (maxIndex == staIndex)
        {
            data[staIndex] = minValue;
            data[endIndex] = maxValue;
            data[minIndex] = endValue;
        }
        else if (minIndex == endIndex)
        {
            data[staIndex] = minValue;
            data[endIndex] = maxValue;
            data[maxIndex] = staValue;
        }
        else
        {
            data[staIndex] = minValue;
            data[endIndex] = maxValue;
            data[minIndex] = staValue;
            data[maxIndex] = endValue;
        }

        endIndex--;
        staIndex++;
    }
}

if (DoVerfications)
{
    /// <summary>
    /// Verification tests
    /// </summary>

    Console.WriteLine("Verification tests for {0} iterations, this may take some time.", VerficationRepeats);
    int fails = 0;
    for (int c = 0; c < VerficationRepeats; c++)
    {
        var data_vf1 = new int[data0.Length];
        for (int i = 0; i < data_vf1.Length; i++)
        {
            data_vf1[i] = Random.Shared.Next(data_vf1.Length);
        }
        var data_vf2 = new int[data_vf1.Length];
        data_vf1.CopyTo(data_vf2, 0);

        Array.Sort(data_vf1);
        DoubleSelectionSort(data_vf2);
        if (!SequenceEqual(data_vf1, data_vf2))
        {
            Console.WriteLine("ERROR SORTING ARRAY! AT {0}", c);
            fails++;
        }
    }
    if (fails == 0)
    {
        Console.WriteLine("Verification tests passed!");
    }
    else
    {
        Console.WriteLine("Verification tests failed for {0} times!", fails);
    }
}

/// <summary>
/// Array.Sort
/// </summary>
watch.Start();

Array.Sort(data1);

watch.Stop();

double t1 = watch.ElapsedMilliseconds;

Console.WriteLine("Array.Sort[t={0}ms]:{1}", t1, string.Join(',', data1.Take(16)) + "...");

/// <summary>
/// SingleSelectionSort
/// </summary>
watch.Restart();

SingleSelectionSort(data2);

watch.Stop();

double t2 = watch.ElapsedMilliseconds;

Console.WriteLine("Single Selection Sort[t={0}ms,correct={1}]:{2}",t2, SequenceEqual(data1,data2), string.Join(',', data2.Take(16)) + "...");

/// <summary>
/// DoubleSelectionSort
/// </summary>
watch.Restart();

DoubleSelectionSort(data3);

watch.Stop();

double t3 = watch.ElapsedMilliseconds;

Console.WriteLine("Double Selection Sort[t={0}ms,correct={1}]:{2}", t3, SequenceEqual(data1, data3), string.Join(',', data3.Take(16)) + "...");

double ef1 = (1.0 / t3 - 1.0 / t2) / (1.0 / t2);

Console.WriteLine("Double Selection Sort Efficiency Boost:{0:F2}%", ef1 * 100.0);


/// <summary>
/// FastSingleSelectionSort
/// </summary>
int[] FastSingleSelectionSort(int[] data)
{
    int width = Vector<int>.Count;
    int N = data.Length;
    int R = N % width;
    int T = N - R;
    for (int i = N - 1; i >= T; i--)
    {
        int maxIndex = i;
        int max = data[maxIndex];
        for (int j = i - 1; i >= 0; j--)
        {
            if (data[j] > max)
            {
                max = data[j];
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            Swap(data, maxIndex, i);
        }
    }

    int[] buffer = new int[width];
    int[] positions = new int[width];

    for (int i = 0; i < T - width; i+= width)
    {
        for (int q = 0; q < width; q++)
        {
            positions[q] = i + q;
        }
        var min = new Vector<int>(data,i);        

        for (int j = i + width; j < T; j+=width)
        {
            min.CopyTo(buffer);
            var dt = new Vector<int>(data, j);
            var rt = System.Numerics.Vector.LessThan(dt, min);
            var any = false;
            for(int s = 0; s < width; s++)
            {
                if (rt[s] != 0)
                {
                    positions[s] = j + s;
                    buffer[s] = dt[s];
                    any = true;
                }
            }
            if (any)
            {
                min = new Vector<int>(buffer);
            }
        }
        for(int q = 0; q < width; q++)
        {
            int a = positions[q];
            int b = i + q;
            if (a != b)
            {
                Swap(data, a, b);
            }
        }
    }

    return DoCollect(data, width, true);
}
/// <summary>
/// FastDoubleSelectionSort
/// </summary>
int[] FastDoubleSelectionSort(int[] data)
{
    int width = Vector<int>.Count;
    int N = data.Length;
    int R = N % width;
    int T = N - R;
    for (int i = N - 1; i >= T; i--)
    {
        int maxIndex = i;
        int max = data[maxIndex];
        for (int j = i - 1; i >= 0; j--)
        {
            if (data[j] > max)
            {
                max = data[j];
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            Swap(data, maxIndex, i);
        }
    }

    int[] minBuffer = new int[width];
    int[] minIndices = new int[width];
    int[] maxBuffer = new int[width];
    int[] maxIndices = new int[width];
    int[] staIndices = new int[width];
    int[] endIndices = new int[width];
    int[] staValues = new int[width];
    int[] endValues = new int[width];
    for (int i = 0; i < T - width; i += width)
    {
        for (int q = 0; q < width; q++)
        {
            minIndices[q] = i + q;
        }
        for (int q = 0; q < width; q++)
        {
            maxIndices[q] = T - i + q;
        }
        minIndices.CopyTo(staIndices, 0);
        maxIndices.CopyTo(endIndices, 0);

        for (int q = 0; q < width; q++)
        {
            staValues[q] = data[i + q];
        }
        for (int q = 0; q < width; q++)
        {
            endValues[q] = data[T - i + q];
        }


        var minValues = new Vector<int>(data, i);
        var maxValues = new Vector<int>(data, T - i);

        for (int j = i + width; j < T; j += width)
        {
            minValues.CopyTo(minBuffer);
            maxValues.CopyTo(maxBuffer);

            var dt = new Vector<int>(data, j);
            var lrt = System.Numerics.Vector.LessThan(dt, minValues);
            var grt = System.Numerics.Vector.GreaterThan(dt, maxValues);

            var anylt = false;
            var anygt = false;
            for (int s = 0; s < width; s++)
            {
                if (lrt[s] != 0)
                {
                    minIndices[s] = j + s;
                    minBuffer[s] = dt[s];
                    anylt = true;
                }
            }
            if (anylt)
            {
                minValues = new Vector<int>(minBuffer);
            }
            for (int s = 0; s < width; s++)
            {
                if (grt[s] != 0)
                {
                    maxIndices[s] = j + s;
                    maxBuffer[s] = dt[s];
                    anygt = true;
                }
            }
            if (anygt)
            {
                maxValues = new Vector<int>(maxBuffer);
            }

        }
        for (int q = 0; q < width; q++)
        {
            if (minValues[q] == maxValues[q])
            {
                break;
            }
            else if (maxIndices[q] == staIndices[q]
                  && minIndices[q] == endIndices[q])
            {
                data[staIndices[q]] = minValues[q];
                data[endIndices[q]] = maxValues[q];
            }
            else if (maxIndices[q] == staIndices[q])
            {
                data[staIndices[q]] = minValues[q];
                data[endIndices[q]] = maxValues[q];
                data[minIndices[q]] = endValues[q];
            }
            else if (minIndices[q] == endIndices[q])
            {
                data[staIndices[q]] = minValues[q];
                data[endIndices[q]] = maxValues[q];
                data[maxIndices[q]] = staValues[q];
            }
            else
            {
                data[staIndices[q]] = minValues[q];
                data[endIndices[q]] = maxValues[q];
                data[minIndices[q]] = staValues[q];
                data[maxIndices[q]] = endValues[q];
            }
        }
    }

    return DoCollect(data, width, true);

}

watch.Restart();

data6 = FastSingleSelectionSort(data6);

watch.Stop();

double t6 = watch.ElapsedMilliseconds;

Console.WriteLine("Fast Single Selection Sort[t={0}ms,correct={1}]:{2}", t6, SequenceEqual(data1, data6), string.Join(',', data6.Take(16)) + "...");

double ef2 = (1.0 / t6 - 1.0 / t2) / (1.0 / t2);
Console.WriteLine("Fast Single Selection Sort Efficiency Boost:{0:F2}%", ef2 * 100.0);

///
///Quick Sort
///
void QuickSort(int[] data)
{
    QuickSortImpl(data, 0, data.Length - 1);
}
void QuickSortImpl(int[] data, int low, int high)
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
void DoubleQuickSort(int[] data, int lower, int upper)
{
    if (lower >= upper) return;

    int foreward = lower, backward = upper;
    int smaller = data[foreward];
    int bigger = data[backward];
    if(bigger < smaller)
    {
        Swap(data, foreward, backward);
        smaller = data[foreward];
        bigger = data[backward];
    }

    while (data[backward] >= smaller && backward > foreward)
        --backward;
    //found the one much smaller than smaller one

    while (data[foreward] <= bigger && backward > foreward)
        ++foreward;
    //found the one much bigger than the bigger one
    int even_smaller = data[backward];
    int even_bigger = data[foreward];


}


watch.Restart();

QuickSort(data4);

watch.Stop();

double t4 = watch.ElapsedMilliseconds;

Console.WriteLine("Quick Sort[t={0}ms,correct={1}]:{2}", t4, SequenceEqual(data1, data4), string.Join(',', data4.Take(16)) + "...");


void StackQuickSort(int[] data)
{
    int low = 0;
    int high = data.Length - 1;

    var stack = new Stack<(int, int)>(32);

    stack.Push((low, high));

    while(stack.Count>0)
    {
        (low,high) = stack.Pop();
        int x = low, y = high;
        int key = data[x];
        while (x < y)
        {
            while (data[y] >= key && y > x)
                --y;
            data[x] = data[y];
            while (data[x] <= key && y > x)
                ++x;
            data[y] = data[x];
        }
        data[x] = key;

        if (low < x - 1)
        {
            stack.Push((low, x - 1));
        }
        if (x + 1 < high)
        {
            stack.Push((x + 1, high));
        }      
    }
}


watch.Restart();

StackQuickSort(data5);

watch.Stop();

double t5 = watch.ElapsedMilliseconds;

Console.WriteLine("Stack Quick Sort[t={0}ms,correct={1}]:{2}", t5, SequenceEqual(data1, data5), string.Join(',', data5.Take(16)) + "...");

int[] FastStackQuickSort(int[] data)
{
    int N = data.Length;
    int W = Vector<int>.Count;
    int R = N % W;
    int T = N - R;
    for (int i = N - 1; i >= T; i--)
    {
        int maxIndex = i;
        int max = data[maxIndex];
        for (int j = i - 1; i >= 0; j--)
        {
            if (data[j] > max)
            {
                max = data[j];
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            Swap(data, maxIndex, i);
        }
    }

    int[] keys = new int[W];
    int[] xs = new int[W];
    int[] ys = new int[W];
    

    LinkedListStack<(int[], int[])> stack = new();
    for(int q = 0; q < W; q++)
    {
        xs[q] = q;
        ys[q] = T - q;
    }

    stack.Push((xs.ToArray(), ys.ToArray()));

    while (stack.Count > 0)
    {
        var (low, high) = stack.Pop();

        xs = low;
        ys = high;
        
        for (int q = 0; q < W; q++)
        {
            keys[q] = data[xs[q]];
        }
        //TODO:
        //while (x < y)
        //{
        //    while (data[y] >= key && y > x)
        //        --y;
        //    data[x] = data[y];
        //    while (data[x] <= key && y > x)
        //        ++x;
        //    data[y] = data[x];
        //}
        //for (int q = 0; q < W; q++)
        //{
        //    data[xs[q]] = keys[q];
        //}

        //if (low < x - 1)
        //{
        //    stack.Push((low, x - 1));
        //}
        //if (x + 1 < high)
        //{
        //    stack.Push((x + 1, high));
        //}
    }
    return DoCollect(data, W, true);
}




/// <summary>
/// Insertion Sort
/// </summary>
void InsertionSort(int[] a)
{
    int N = a.Length;
    for (int i = 1; i < N; i++)
    {
        for (int j = i; j > 0 && a[j] < a[j - 1]; j--)
        {
            int t = a[j];
            a[j] = a[j - 1];
            a[j - 1] = t;
        }
    }
}

watch.Restart();

InsertionSort(data7);

watch.Stop();

double t7 = watch.ElapsedMilliseconds;

Console.WriteLine("Insertion Sort[t={0}ms,correct={1}]:{2}", t7, SequenceEqual(data1, data7), string.Join(',', data7.Take(16)) + "...");


/// <summary>
/// FastInsertionSort
/// </summary>
int[] FastInsertionSort(int[] data)
{
    int N = data.Length;
    int W = Vector<int>.Count;
    int R = N % W;
    int T = N - R;
    for (int i = N - 1; i >= T; i--)
    {
        int maxIndex = i;
        int max = data[maxIndex];
        for (int j = i - 1; i >= 0; j--)
        {
            if (data[j] > max)
            {
                max = data[j];
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            Swap(data, maxIndex, i);
        }
    }

    for (int i = W; i < T; i += W)
    {
        for (int j = i; j > 0; j -= W)
        {
            var dj = new Vector<int>(data, j);
            var dm = new Vector<int>(data, j - W);
            var dk = System.Numerics.Vector.LessThan(dj, dm);
            for (int k = 0; k < W; k++)
            {
                if (dk[k] != 0)
                {
                    int t = data[j + k];
                    data[j + k] = data[j - W + k];
                    data[j - W + k] = t;
                }
            }
        }
    }

    return DoCollect(data, W, true);
}

watch.Restart();

data8 = FastInsertionSort(data8);

watch.Stop();

double t8 = watch.ElapsedMilliseconds;

Console.WriteLine("Fast Insertion Sort[t={0}ms,correct={1}]:{2}", t8, SequenceEqual(data1, data8), string.Join(',', data8.Take(16)) + "...");

double ef3 = (1.0 / t8 - 1.0 / t7) / (1.0 / t7);
Console.WriteLine("Fast Insertion Sort Efficiency Boost:{0:F2}%", ef3 * 100.0);


void GnomeSort(int[] a)
{
    int p = 0;
    while (p < a.Length)
    {
        if (p == 0 || a[p] >= a[p - 1])
        {
            p++;
        }
        else
        {
            Swap(a, p, p - 1);
            p--;
        }
    }
}

int[] OddEvenSort(int[] a)
{
    for(int c= 0; c < a.Length; c++)
    {
        int p = c % 2;
        for (int d = 0; d < a.Length - p - p; d += 2)
        {
            //this is for odd length
            if (d + p + 1 >= a.Length) break;
            //this step should be in parallel
            if (a[d + p + 0] > a[d + p + 1])
            {
                Swap(a, d + p + 0, d + p + 1);
            }
        }
    }
    return a;
}

unsafe int[] FastOddEvenSort(int[] t)
{
    var interlace_indices_even = new int[] { 0, 2, 4, 6, 8, 10, 12, 14 };
    var interlace_indices_even_ext = new int[] { 2, 4, 6, 8, 10, 12, 14, 16 };
    var interlace_indices_odd = new int[] { 1, 3, 5, 7, 9, 11, 13, 15 };

    var ones = Vector256<int>.AllBitsSet;
    var size = (byte)Vector256<int>.Count;
    var last = size - 1;
    var half = (byte)(size >> 1);
    var @double = (byte)(size << 1);
    var quad = (byte)@double << 1;
    if (!Avx2.X64.IsSupported)
    {
        return t;
    }
    else if(t.Length == 0)
    {
        return t;
    }
    else if (t.Length < @double || t.Length % @double>0)
    {
        Array.Sort(t);
        return t;
    }
    var a = t;
    var mod = a.Length % @double;
    if (mod == 0)
    {
        a = new int[a.Length + 1];
        Array.Copy(t, a, t.Length);
    }

    var buff = new int[@double + 1]; 
    fixed (int* pa = a, 
        po = interlace_indices_odd, 
        pe = interlace_indices_even, 
        pt = interlace_indices_even_ext,
        pb = buff)
    {
        var ipe = Avx.LoadVector256(pe);
        var ipt = Avx.LoadVector256(pt);
        var ipo = Avx.LoadVector256(po);
        for (var repeat = 0; repeat < t.Length/2; repeat++)
        {
            var any_even_set = false;
            var any_odd_set = false;

            for (var part = 0; part < t.Length; part += @double)
            {
                var ptr = pa + part;

                var veven = Avx2.GatherVector256(ptr, ipe, half);
                var vodd_ = Avx2.GatherVector256(ptr, ipo, half);

                var min = Avx2.Min(vodd_, veven);
                var max = Avx2.Max(vodd_, veven);

                var upl = Avx2.UnpackLow(min, max);
                var upll = upl.GetLower();
                var uplh = upl.GetUpper();

                var uph = Avx2.UnpackHigh(min, max);
                var uphl = uph.GetLower();
                var uphh = uph.GetUpper();

                var uplx = Vector256.Create(upll, uphl);
                var uphx = Vector256.Create(uplh, uphh);

                var front = Avx.LoadVector256(ptr);
                var later = Avx.LoadVector256(ptr + size);

                if (!Avx.TestZ(Avx2.Subtract(uplx, front), ones))
                {
                    Avx.Store(ptr, uplx);
                    any_even_set |= true;
                }

                if (!Avx.TestZ(Avx2.Subtract(uphx, later), ones))
                {
                    Avx.Store(ptr + size, uphx);
                    any_even_set |= true;
                }
            }
            for (var part = 0; part < t.Length; part += @double)
            {
                var islast = @double >= t.Length - part - 1;

                //this is the last
                if (islast)
                {
                    Array.Copy(a, part, buff, 0, @double);
                    buff[@double] = buff[@double - 1];
                }
                var par = pa + part;
                var ptr = islast ? pb : par;

                var vskip = Avx2.GatherVector256(ptr, ipt, half);
                var vodd_ = Avx2.GatherVector256(ptr, ipo, half);

                var min = Avx2.Min(vodd_, vskip);
                var max = Avx2.Max(vodd_, vskip);

                var upl = Avx2.UnpackLow(min, max);
                var upll = upl.GetLower();
                var uplh = upl.GetUpper();

                var uph = Avx2.UnpackHigh(min, max);
                var uphl = uph.GetLower();
                var uphh = uph.GetUpper();

                var uplx = Vector256.Create(upll, uphl);
                var uphx = Vector256.Create(uplh, uphh);

                var pur = (islast ? par + 1 : ptr + 1);

                var front = Avx.LoadVector256(pur);
                var later = Avx.LoadVector256(pur + size);
                if (!Avx.TestZ(Avx2.Subtract(uplx, front), ones))
                {
                    Avx.Store(pur, uplx);
                    any_odd_set |= true;
                }
                if (!Avx.TestZ(Avx2.Subtract(uphx, later), ones))
                {
                    Avx.Store(pur + size, uphx);
                    any_odd_set |= true;
                }
            }
            if (!(any_odd_set || any_even_set)) break;
        }
    }
    if (a != t)
    {
        Array.Copy(a, t, t.Length);
    }

    return t;
}

#if false
data11 = new int[1<<12];
for (int i = 0; i < data11.Length; i++)
{
    data11[i] = Random.Shared.Next(data11.Length);
}
data1 = new int[data11.Length];

data11.CopyTo(data1, 0);

Array.Sort(data1);

data10 = new int[data11.Length];
data11.CopyTo(data10, 0);
#endif

watch.Restart();
data10 = OddEvenSort(data10);
Array.Sort(data10);

watch.Stop();

double t10 = watch.ElapsedMilliseconds;

Console.WriteLine("Odd Even Sort[t={0}ms,correct={1}]:{2}", t10, SequenceEqual(data1, data10), string.Join(',', data10.Take(16)) + "...");

double ef4 = (1.0 / t10 - 1.0 / t7) / (1.0 / t7);
Console.WriteLine("Odd Even Sort Efficiency Boost:{0:F2}%", ef4 * 100.0);


watch.Restart();

data11 = FastOddEvenSort(data11);

watch.Stop();

double t11 = watch.ElapsedMilliseconds;

var bx = SequenceEqual(data1, data11);
//if (!bx)
//{
//    for(int i = 0; i < data1.Length; i++)
//    {
//        if (data1[i] != data11[i])
//        {
//            Console.WriteLine($"Wrong data at{i},data1[{i}]={data1[i]},data11[{i}]={data11[i]};");
//        }
//    }
//}
Console.WriteLine("Fast Odd Even Sort[t={0}ms,correct={1}]:{2}", t11, bx, string.Join(',', data11.Take(16)) + "...");

double ef5 = (1.0 / t11 - 1.0 / t10) / (1.0 / t10);
Console.WriteLine("Fast Odd Even Sort Efficiency Boost:{0:F2}%", ef5 * 100.0);


Console.WriteLine("Finished.");
Console.ReadKey();

