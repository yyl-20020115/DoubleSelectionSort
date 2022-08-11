bool DoubleSelectionSort(int data[], int n)
{
	if (data == 0 || n < 1)
	{
		return false;
	}
	int staIndex = 0;
	int endIndex = n - 1;
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
		else
		{
			data[staIndex] = minValue;
			data[endIndex] = maxValue;

			bool both = maxIndex == staIndex && minIndex == endIndex;
			if (!both && maxIndex != staIndex && minIndex != endIndex)
			{
				data[minIndex] = staValue;
				data[maxIndex] = endValue;
			}
			if (!both && maxIndex == staIndex)
			{
				data[minIndex] = endValue;
			}
			if (!both && minIndex == endIndex)
			{
				data[maxIndex] = staValue;
			}
		}

		endIndex--;
		staIndex++;
	}
	return true;
}
