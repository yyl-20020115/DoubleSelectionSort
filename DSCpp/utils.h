#pragma once

inline void Swap(int data[], int i, int j) 
{
	int t = data[i];
	data[i] = data[j];
	data[j] = t;
}
void Merge(int data[], int n, int stride);
bool CheckSequence(int a[], int b[], int n);

inline void Swap(unsigned int data[], int i, int j)
{
	unsigned int t = data[i];
	data[i] = data[j];
	data[j] = t;
}
void Merge(unsigned int data[], int n, int stride);
bool CheckSequence(unsigned int a[], int b[], int n);

