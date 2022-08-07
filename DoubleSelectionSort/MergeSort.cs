namespace DoubleSelectionSort;

public static class MergeSort
{
    public static int[] TestData = { 4, 2, 8, 0, 5, 7, 1, 3, 9 };

    public static int[] Sort(int[] data)
        => Sort(data, 0, data.Length - 1);
    public static int[] Sort(int[] data, int start, int end)
    {
        //当start==end时，此时分组里只有一个元素，所以这是临界点
        if (start < end)
        {
            //分成左右两个分组，再进行递归
            var mid = (start + end) / 2;
            //左半边分组
            Sort(data, start, mid);
            //右半边分组
            Sort(data, mid + 1, end);
            //递归之后再归并归并一个大组
            Merge(data, start, mid, end);
        }
        return data;
    }

    //归并方法
    public static void Merge(int[] data, int start, int mid, int end)
    {
        //左边分组的起点i_start，终点i_end，也就是第一个有序序列
        var i_start = start;
        var i_end = mid;
        //右边分组的起点j_start，终点j_end，也就是第二个有序序列
        var j_start = mid + 1;
        var j_end = end;
        //额外空间初始化，数组长度为end-start+1
        var working = new int[end - start + 1];
        var len = 0;
        //合并两个有序序列
        while (i_start <= i_end && j_start <= j_end)
        {
            //当arr[i_start]<arr[j_start]值时，将较小元素放入额外空间，反之一样
            if (data[i_start] < data[j_start])
            {
                working[len] = data[i_start];
                len++;
                i_start++;
            }
            else
            {
                working[len] = data[j_start];
                len++;
                j_start++;
            }
        }

        //i这个序列还有剩余元素
        while (i_start <= i_end)
        {
            working[len] = data[i_start];
            len++;
            i_start++;
        }
        //j这个序列还有剩余元素
        while (j_start <= j_end)
        {
            working[len] = data[j_start];
            len++;
            j_start++;
        }
        //辅助空间数据覆盖到原空间
        Array.Copy(working, 0, data, start, working.Length);
    }
}
