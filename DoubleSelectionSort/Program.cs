// This is the demo for Double Selection Sort algorithm
// This algorithm improves the efficiency of the selection sort algorithm up to 50%.
//
// Single Selection Sort[t=26ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Double Selection Sort[t=17ms]:0,1,3,4,4,5,6,7,9,9,12,13,16,17,17,19...
// Efficiency Boost:52.94 %



using System.Diagnostics;

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
}


Stopwatch sw = new();

var rand = Random.Shared;
var data0 = new int[4096];// new int[16] { 6, 11, 4, 7, 0, 2, 9, 8, 1, 14, 10, 15, 13, 12, 3, 5 };
for (int i = 0; i < data0.Length; i++)
{
    data0[i] = rand.Next(data0.Length);
}
var data1 = new int[data0.Length];
var data2 = new int[data0.Length];
data0.CopyTo(data1, 0);
data0.CopyTo(data2, 0);

sw.Start();

SingleSelectionSort(data1);

sw.Stop();

double t1 = sw.ElapsedMilliseconds;

Console.WriteLine("Single Selection Sort[t={0}ms]:{1}",t1, string.Join(',', data1.Take(16)) + "...");


sw.Restart();

DoubleSelectionSort(data2);

sw.Stop();

double t2 = sw.ElapsedMilliseconds;

Console.WriteLine("Double Selection Sort[t={0}ms]:{1}", t2, string.Join(',', data2.Take(16)) + "...");

double ef = (1.0 / t2 - 1.0 / t1) / (1.0 / t1);

Console.WriteLine("Efficiency Boost:{0:F2}%", ef*100.0);

Console.WriteLine("Finished.");
