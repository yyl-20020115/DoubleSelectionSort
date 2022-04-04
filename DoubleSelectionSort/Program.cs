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
    int N = data.Length;
    int H = N >> 1;
    for (int fi = 0; fi <= H; fi++)
    {
        if (fi == H)
        {
            if (data[fi] > data[fi + 1])
            {
                Swap(data, fi, fi + 1);
            }
        }
        else
        {
            int ri = N - fi - 1;
            int minIndex = fi, maxIndex = ri;
            for (int j = fi + 1; j < N - fi; j++)
            {
                if (data[j] < data[minIndex])
                {
                    minIndex = j;
                }
                if (data[j] > data[maxIndex])
                {
                    maxIndex = j;
                }
            }
            if (fi != minIndex) Swap(data, fi, minIndex);
            if (ri != maxIndex) Swap(data, ri, maxIndex);
        }
    }

    for(int fi = 0; fi < N-1; fi++)
    {
        if(data[fi]> data[fi + 1])
        {
            Swap(data,fi,fi + 1);
        }
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

var data0 = new int[4096];// new int[16] { 6, 11, 4, 7, 0, 2, 9, 8, 1, 14, 10, 15, 13, 12, 3, 5 };

for (int i = 0; i < data0.Length; i++)
{
    data0[i] = Random.Shared.Next(data0.Length);
}
var datam = new int[data0.Length];
var data1 = new int[data0.Length];
var data2 = new int[data0.Length];
var data3 = new int[data0.Length];
data0.CopyTo(datam, 0);

data0.CopyTo(data1, 0);
data0.CopyTo(data2, 0);
data0.CopyTo(data3, 0);

/// <summary>
/// Array.Sort
/// </summary>
watch.Start();

Array.Sort(data0);

watch.Stop();

double t0 = watch.ElapsedMilliseconds;

Console.WriteLine("Array.Sort[t={0}ms]:{1}", t0, string.Join(',', data0.Take(16)) + "...");

/// <summary>
/// SingleSelectionSort
/// </summary>
watch.Restart();

SingleSelectionSort(data1);

watch.Stop();

double t1 = watch.ElapsedMilliseconds;

Console.WriteLine("Single Selection Sort[t={0}ms,correct={1}]:{2}",t1, Enumerable.SequenceEqual(data0,data1), string.Join(',', data1.Take(16)) + "...");

/// <summary>
/// DoubleSelectionSort
/// </summary>
watch.Restart();

DoubleSelectionSort(data2);

watch.Stop();

double t2 = watch.ElapsedMilliseconds;

Console.WriteLine("Double Selection Sort[t={0}ms,correct={1}]:{2}", t2, Enumerable.SequenceEqual(data0, data2), string.Join(',', data2.Take(16)) + "...");

var wrong = false;
var wp = -1;
for(int i = 0; i < data0.Length; i++)
{
    if(data0[i] != data2[i])
    {
        if(wp==-1) wp = i;
        wrong = true;
        Console.WriteLine("wrong at={0},data1({0})={1},data2({0})={2}", i,data0[i],data2[i]);
       
    }
}

if (wrong)
{
    DoubleSelectionSort(datam);
}

double ef = (1.0 / t2 - 1.0 / t1) / (1.0 / t1);

Console.WriteLine("Efficiency Boost:{0:F2}%", ef*100.0);

/// <summary>
/// FastSelectionSort
/// </summary>

watch.Restart();

FastSelectionSort(data3);

watch.Stop();

double t3 = watch.ElapsedMilliseconds;

Console.WriteLine("Fast Selection Sort[t={0}ms,correct={1}]:{2}", t3, Enumerable.SequenceEqual(data0, data3), string.Join(',', data3.Take(16)) + "...");


Console.WriteLine("Finished.");
