// This is the demo for Double Selection Sort algorithm
// This algorithm improves the efficiency of the selection sort algorithm up to 50%.
//
// Single Selection Sort[t=26ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Double Selection Sort[t=17ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Efficiency Boost:52.94%


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
void QuickSort(int[] data, int low, int high)
{
    if (low >= high) return;

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
        QuickSort(data, low, x - 1);
    }
    if (x + 1 < high)
    {
        QuickSort(data, x + 1, high);
    }
}

watch.Restart();

QuickSort(data4, 0, data4.Length - 1);

watch.Stop();

double t4 = watch.ElapsedMilliseconds;

Console.WriteLine("Quick Sort[t={0}ms,correct={1}]:{2}", t4, SequenceEqual(data1, data4), string.Join(',', data4.Take(16)) + "...");


void StackQuickSort(int[] data)
{
    int low = 0;
    int high = data.Length - 1;

    Stack<(int, int)> stack = new(32);


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
    

    Stack<(int[], int[])> stack = new();
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
            //this step should be in parallel
            if (a[d + p + 0] > a[d + p + 1])
            {
                Swap(a, d + p + 0, d + p + 1);
            }
        }
    }
    return a;
}
int[] interlace_indices = new int[] { 0, 2, 4, 6 };
unsafe int[] FastOddEvenSort(int[] a)
{
    if(!Avx2.X64.IsSupported)
    {
        return a;
    }
    else if(a.Length == 0)
    {
        return a;
    }
    else if (a.Length < Vector<int>.Count)
    {
        var b = new int[Vector<int>.Count];
        Array.Copy(a, b, a.Length);
        a = b;
    }

    var size = Vector<int>.Count;
    var half = size >> 1;
    fixed (int* pa = a, pi = interlace_indices)
    {
        var ie = Sse2.LoadVector128(pi); 
        for (var repeat = 0; repeat < a.Length; repeat++)
        {
            var p = repeat % 2;
            for (var part = 0; part < a.Length; part += size)
            {
                var ve = Avx2.GatherVector128(pa + part + (p << 1), ie, 4);
                var vo = Avx2.GatherVector128(pa + part + 1, ie, 4);
                var re = p == 0 ? Sse2.CompareGreaterThan(ve, vo) : Sse2.CompareGreaterThan(vo, ve);
                for (var i = 0; i < ((p == 1 && part + size == a.Length) ? half - 1 : half); i++)
                {
                    var v = Vector128.GetElement(re, i);
                    var j = i << 1;
                    if (v == -1)
                    {
                        Swap(a, part + p + j + 0, part + p + j + 1);
                    }
                }
            }
        }
    }

    return a;
}


watch.Restart();
data10 = OddEvenSort(data10);

watch.Stop();

double t10 = watch.ElapsedMilliseconds;

Console.WriteLine("Odd Even Sort[t={0}ms,correct={1}]:{2}", t10, SequenceEqual(data1, data10), string.Join(',', data10.Take(16)) + "...");

double ef4 = (1.0 / t10 - 1.0 / t7) / (1.0 / t7);
Console.WriteLine("Odd Even Sort Efficiency Boost:{0:F2}%", ef4 * 100.0);


watch.Restart();
//var data12 = new int[] { 3, 5, 2, 4, 1, 7, 8, 6, 12, 11, 10, 13, 15, 18, 16, 17 };
data11 = FastOddEvenSort(data11);

watch.Stop();

double t11 = watch.ElapsedMilliseconds;

Console.WriteLine("Fast Odd Even Sort[t={0}ms,correct={1}]:{2}", t11, SequenceEqual(data1, data11), string.Join(',', data11.Take(16)) + "...");

double ef5 = (1.0 / t11 - 1.0 / t10) / (1.0 / t10);
Console.WriteLine("Fast Odd Even Sort Efficiency Boost:{0:F2}%", ef5 * 100.0);


Console.WriteLine("Finished.");
Console.ReadKey();

