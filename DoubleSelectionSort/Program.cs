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

Console.WriteLine("Array Size = {0}", SampleDataSize);

void Swap(int[] a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void SingleSelectionSort(int[] data)
{
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

var watch = new Stopwatch();

var data0 = new int[SampleDataSize];// new int[16] { 6, 11, 4, 7, 0, 2, 9, 8, 1, 14, 10, 15, 13, 12, 3, 5 };

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
        if (!Enumerable.SequenceEqual(data_vf1, data_vf2))
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

Console.WriteLine("Single Selection Sort[t={0}ms,correct={1}]:{2}",t2, Enumerable.SequenceEqual(data1,data2), string.Join(',', data2.Take(16)) + "...");

/// <summary>
/// DoubleSelectionSort
/// </summary>
watch.Restart();

DoubleSelectionSort(data3);

watch.Stop();

double t3 = watch.ElapsedMilliseconds;

Console.WriteLine("Double Selection Sort[t={0}ms,correct={1}]:{2}", t3, Enumerable.SequenceEqual(data1, data3), string.Join(',', data3.Take(16)) + "...");

double ef1 = (1.0 / t3 - 1.0 / t2) / (1.0 / t2);

Console.WriteLine("Double Selection Sort Efficiency Boost:{0:F2}%", ef1 * 100.0);

/// <summary>
/// FastSingleSelectionSort
/// </summary>
int[] FastSingleSelectionSort(int[] data)
{
    int width = Vector<int>.Count;
    int N = data.Length;
    
    if (N % width > 0) 
        throw new ArgumentException($"data size should have aligned to {width}");

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
            var dt = new Vector<int>(data, j);
            var rt = Vector.LessThan(dt, min);
            var any = false;
            for(int s = 0; s < width; s++)
            {
                if (rt[s] == -1)
                {
                    positions[s] = j + s;
                    buffer[s] = dt[s];
                    any = true;
                }
                else
                {
                    buffer[s] = min[s];
                }
            }
            if (any)
            {
                min = new Vector<int>(buffer);
            }
        }
        for(int q = 0; q < width; q++)
        {
            if(positions[q] != i + q)
            {
                Swap(data, i + q, positions[q]);
            }
        }
    }

    int[] result = new int[data.Length];
    for (int q = 0; q < width; q++)
    {
        positions[q] = q;
    }
    int tp = 0;
    while (tp<result.Length)
    {
        int? minpos = null;
        int? minval = null;
        for (int q = 0; q < width; q++)
        {
            int index = positions[q];
            if (index >= data.Length) continue;
            int value = data[index];
            if (minval == null || value < minval)
            {
                minval = value;
                minpos = q;
            }
        }
        if (minpos is int _minpos && minval is int _minval)
        {
            positions[_minpos] += width;
            result[tp++] = _minval;
        }
        else
        {
            break;
        }
    }

    return result;
}


watch.Restart();

data6 = FastSingleSelectionSort(data6);

watch.Stop();

double t6 = watch.ElapsedMilliseconds;

Console.WriteLine("Fast Single Selection Sort[t={0}ms,correct={1}]:{2}", t6, Enumerable.SequenceEqual(data1, data6), string.Join(',', data6.Take(16)) + "...");

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

Console.WriteLine("Quick Sort[t={0}ms,correct={1}]:{2}", t4, Enumerable.SequenceEqual(data1, data4), string.Join(',', data4.Take(16)) + "...");


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

Console.WriteLine("Stack Quick Sort[t={0}ms,correct={1}]:{2}", t5, Enumerable.SequenceEqual(data1, data5), string.Join(',', data5.Take(16)) + "...");


Console.WriteLine("Finished.");
Console.ReadKey();
