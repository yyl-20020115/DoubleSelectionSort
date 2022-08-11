void InsertionSort(int data[], int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int j = i; j > 0 && data[j] < data[j - 1]; j--)
		{
			int t = data[j];
			data[j] = data[j - 1];
			data[j - 1] = t;
		}
	}
}
