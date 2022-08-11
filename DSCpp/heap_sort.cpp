
void HeapAdjust(int data[], int position, int n)
{
	int temporary = data[position];
	int childpos = 2 * position + 1; //���ӽ���λ�á�(i+1 Ϊ��ǰ���������Һ��ӽ���λ��)
	while (childpos < n) {
		if (childpos + 1 < n && data[childpos] < data[childpos + 1]) { // ����Һ��Ӵ�������(�ҵ��ȵ�ǰ����������ĺ��ӽ��)
			++childpos;
		}
		if (data[position] < data[childpos]) {  // ����ϴ���ӽ����ڸ����
			data[position] = data[childpos]; // ��ô�ѽϴ���ӽ�������ƶ����滻���ĸ����
			position = childpos;		 // ��������s ,������������һ������λ��
			childpos = 2 * position + 1;
		}
		else {			 // �����ǰ�������������������Һ��ӣ�����Ҫ������ֱ���˳�
			break;
		}
		data[position] = temporary;			// ��ǰ�������Ľ��ŵ������ĺ��ӽ��λ����
	}
}
void HeapSort(int data[], int n)
{
	/**
	 * ��ʼ�ѽ��е���
	 * ��H[0..length-1]���ɶ�
	 * ������֮���һ��Ԫ�������е���С��Ԫ��
	 */	//��ʼ��
	 //���һ���к��ӵĽڵ��λ�� i=  (length -1) / 2
	for (int i = (n - 1) >> 1; i >= 0; --i)
		HeapAdjust(data, i, n);
	//�����һ��Ԫ�ؿ�ʼ�����н��е���
	for (int i = n - 1; i > 0; --i)
	{
		//�����Ѷ�Ԫ��H[0]�Ͷ������һ��Ԫ��
		int temporary = data[i];
		data[i] = data[0];
		data[0] = temporary;
		//ÿ�ν����Ѷ�Ԫ�غͶ������һ��Ԫ��֮�󣬶�Ҫ�Զѽ��е���
		HeapAdjust(data, 0, i);
	}
}
