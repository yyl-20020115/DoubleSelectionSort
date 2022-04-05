// This is the demo for Double Selection Sort algorithm
// This algorithm improves the efficiency of the selection sort algorithm up to 50%.
//
// Single Selection Sort[t=26ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Double Selection Sort[t=17ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Efficiency Boost:52.94%



using System.Diagnostics;
using System.Numerics;

const int SampleDataSize = 65536;
const int VerficationRepeats = 1000;
const bool DoVerfications = false;

var watch = new Stopwatch();


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
        if (!Vector.EqualsAll(x, y)) return false;
    }

    return true;
}

void Swap(int[] a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
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
data0.CopyTo(data1, 0);
data0.CopyTo(data2, 0);
data0.CopyTo(data3, 0);
data0.CopyTo(data4, 0);
data0.CopyTo(data5, 0);
data0.CopyTo(data6, 0);

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

/// <summary>
/// FastSingleSelectionSort
/// </summary>
int[] FastSingleSelectionSort(int[] data)
{
    int width = Vector<int>.Count;
    int N = data.Length;
    int R = N % width;
    if (R > 0) //Sort tail
    {
        int T = N - R;
        for(int i = N-1; i >= T; i--)
        {
            int maxIndex = i;
            int max = data[maxIndex];
            for(int j = i - 1; i >= 0; j--)
            {
                if (data[j] > max)
                {
                    max=data[j];
                    maxIndex = j;
                }
            }
            if (maxIndex != i)
            {
                Swap(data, maxIndex, i);
            }
        }
        N = T;
    }

    int[] buffer = new int[width];
    int[] positions = new int[width];

    for (int i = 0; i < N - width; i+= width)
    {
        for (int q = 0; q < width; q++)
        {
            positions[q] = i + q;
        }
        var min = new Vector<int>(data,i);        

        for (int j = i + width; j < N; j+=width)
        {
            min.CopyTo(buffer);
            var dt = new Vector<int>(data, j);
            var rt = Vector.LessThan(dt, min);
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
    if (R > 0) //Sort tail
    {
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
        N = T;
    }



    int[] buffer = new int[width];
    int[] positions = new int[width];

    //TODO:

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
void QuickSort(int[] array, int low, int high)
{
    if (low >= high) return;

    int x = low, y = high;
    int key = array[x];
    while (x < y)
    {
        while (array[y] >= key && y > x)
            --y;
        array[x] = array[y];
        while (array[x] <= key && y > x)
            ++x;
        array[y] = array[x];
    }
    array[x] = key;
    if (low < x - 1)
    {
        QuickSort(array, low, x - 1);
    }
    if (x + 1 < high)
    {
        QuickSort(array, x + 1, high);
    }
}

watch.Restart();

QuickSort(data4, 0, data4.Length - 1);

watch.Stop();

double t4 = watch.ElapsedMilliseconds;

Console.WriteLine("Quick Sort[t={0}ms,correct={1}]:{2}", t4, SequenceEqual(data1, data4), string.Join(',', data4.Take(16)) + "...");


void StackQuickSort(int[] array, int low, int high,Stack<(int,int)> stack)
{
    if (low >= high) return;

    stack.Push((low, high));

    while(stack.Count>0)
    {
        (low,high) = stack.Pop();
        int x = low, y = high;
        int key = array[x];
        while (x < y)
        {
            while (array[y] >= key && y > x)
                --y;
            array[x] = array[y];
            while (array[x] <= key && y > x)
                ++x;
            array[y] = array[x];
        }
        array[x] = key;

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

StackQuickSort(data5, 0, data5.Length - 1, new(32));

watch.Stop();

double t5 = watch.ElapsedMilliseconds;

Console.WriteLine("Stack Quick Sort[t={0}ms,correct={1}]:{2}", t5, SequenceEqual(data1, data5), string.Join(',', data5.Take(16)) + "...");


Console.WriteLine("Finished.");
Console.ReadKey();
