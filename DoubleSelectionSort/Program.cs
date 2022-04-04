// This is the demo for Double Selection Sort algorithm
// This algorithm improves the efficiency of the selection sort algorithm up to 50%.
//
// Single Selection Sort[t=26ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Double Selection Sort[t=17ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Efficiency Boost:52.94%



using System.Diagnostics;
using System.Numerics;

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

const int SampleSize = 4096;

var watch = new Stopwatch();

var data0 = new int[SampleSize];// new int[16] { 6, 11, 4, 7, 0, 2, 9, 8, 1, 14, 10, 15, 13, 12, 3, 5 };

for (int i = 0; i < data0.Length; i++)
{
    data0[i] = Random.Shared.Next(data0.Length);
}
var data1 = new int[data0.Length];
var data2 = new int[data0.Length];
var data3 = new int[data0.Length];
data0.CopyTo(data1, 0);
data0.CopyTo(data2, 0);
data0.CopyTo(data3, 0);

/// <summary>
/// Verification tests
/// </summary>
const int Repeats = 1000;

Console.WriteLine("Verification tests for {0} iterations, this may take some time.", Repeats);
int fails = 0;
for (int c = 0; c < Repeats; c++)
{
    var data5 = new int[data0.Length];
    for (int i = 0; i < data5.Length; i++)
    {
        data5[i] = Random.Shared.Next(data5.Length);
    }
    var data6 = new int[data5.Length];
    data5.CopyTo(data6, 0);

    Array.Sort(data5);
    DoubleSelectionSort(data6);
    if (!Enumerable.SequenceEqual(data5, data6))
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


Console.WriteLine("Finished.");
Console.ReadKey();
