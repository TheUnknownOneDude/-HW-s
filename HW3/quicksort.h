//
// Created by 1 on 25.04.2022.
//

#ifndef MAIN_CPP_QUICKSORT_H
	#define MAIN_CPP_QUICKSORT_H

template< typename T >
int partition(T *arr, int l, int r, bool descending)
{
	T pivot = arr[(l + r) / 2];
	int li = l;
	int ri = r;

	if (!descending)
	{
		while (li <= ri)
		{
			while (arr[li] < pivot)
			{
				li++;
			}
			while (arr[ri] > pivot)
			{
				ri--;
			}
			if (li >= ri)
			{
				break;
			}
			swap(arr[li++], arr[ri--]);
		}
	}
	else
	{
		while (li <= ri)
		{
			while (arr[li] > pivot)
			{
				li++;
			}
			while (arr[ri] < pivot)
			{
				ri--;
			}
			if (li >= ri)
			{
				break;
			}
			swap(arr[li++], arr[ri--]);
		}
	}
	return ri;
}

template< typename T, bool descending >
void quicksort(T *a, int l, int r)
{
	if (l < r)
	{
		int q = partition(a, l, r, descending);
		quicksort< T, descending >(a, l, q);
		quicksort< T, descending >(a, q + 1, r);
	}
}

#endif	  // MAIN_CPP_QUICKSORT_H
