// This is the demo for Double Selection Sort algorithm
// This algorithm improves the efficiency of the selection sort algorithm up to 50%.
//
// Single Selection Sort[t=26ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Double Selection Sort[t=17ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Efficiency Boost:52.94%



using System.Diagnostics;
using System.Numerics;

const int SampleDataSize = 4096;
const int VerficationRepeats = 1000;
const bool DoVerfications = false;


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


bool FastSelectionSort(int[] data)
{
    int width = Vector<int>.Count;
    int[] positions = new int[width];
    int N = data.Length;
    if (N % width > 0) return false;

    do
    {
        var first = new Vector<int>(data, 0);
        for (int segment_start = width; segment_start < N; segment_start += width)
        {
            var current = new Vector<int>(data, segment_start);

            var result = Vector.Min(first, current);
            var compare = Vector.Equals(current, result);
            for (int i = 0; i < width; i++)
            {
                //if the Min value equals to the current value, update the position
                if (compare[i] == -1)
                {
                    positions[i] = segment_start + i;
                }
            }
        }
        for (int i = 0; i < width; i++)
        {
            var p = positions[i];
            if (p != 0)
            {
                Swap(data, p, 0);
            }
        }
    }
    while (false);



    return true;
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
data0.CopyTo(data1, 0);
data0.CopyTo(data2, 0);
data0.CopyTo(data3, 0);
data0.CopyTo(data4, 0);
data0.CopyTo(data5, 0);

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

double ef = (1.0 / t3 - 1.0 / t2) / (1.0 / t2);

Console.WriteLine("Efficiency Boost:{0:F2}%", ef*100.0);

/// <summary>
/// FastSelectionSort
/// </summary>

//watch.Restart();

//FastSelectionSort(data3);

//watch.Stop();

//double t3 = watch.ElapsedMilliseconds;

//Console.WriteLine("Fast Selection Sort[t={0}ms,correct={1}]:{2}", t3, Enumerable.SequenceEqual(data0, data3), string.Join(',', data3.Take(16)) + "...");


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
