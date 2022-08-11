
void HeapAdjust(int data[], int position, int n)
{
	int temporary = data[position];
	int childpos = 2 * position + 1; //左孩子结点的位置。(i+1 为当前调整结点的右孩子结点的位置)
	while (childpos < n) {
		if (childpos + 1 < n && data[childpos] < data[childpos + 1]) { // 如果右孩子大于左孩子(找到比当前待调整结点大的孩子结点)
			++childpos;
		}
		if (data[position] < data[childpos]) {  // 如果较大的子结点大于父结点
			data[position] = data[childpos]; // 那么把较大的子结点往上移动，替换它的父结点
			position = childpos;		 // 重新设置s ,即待调整的下一个结点的位置
			childpos = 2 * position + 1;
		}
		else {			 // 如果当前待调整结点大于它的左右孩子，则不需要调整，直接退出
			break;
		}
		data[position] = temporary;			// 当前待调整的结点放到比其大的孩子结点位置上
	}
}
void HeapSort(int data[], int n)
{
	/**
	 * 初始堆进行调整
	 * 将H[0..length-1]建成堆
	 * 调整完之后第一个元素是序列的最小的元素
	 */	//初始堆
	 //最后一个有孩子的节点的位置 i=  (length -1) / 2
	for (int i = (n - 1) >> 1; i >= 0; --i)
		HeapAdjust(data, i, n);
	//从最后一个元素开始对序列进行调整
	for (int i = n - 1; i > 0; --i)
	{
		//交换堆顶元素H[0]和堆中最后一个元素
		int temporary = data[i];
		data[i] = data[0];
		data[0] = temporary;
		//每次交换堆顶元素和堆中最后一个元素之后，都要对堆进行调整
		HeapAdjust(data, 0, i);
	}
}
