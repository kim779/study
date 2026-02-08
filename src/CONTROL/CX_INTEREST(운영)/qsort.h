#ifndef _H_QSORT
#define _H_QSORT

template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			const int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);

		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}


template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii{}, jj{};
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];
	do {
		if (bAscending) 
		{
			while (pArr[jj].name < str.name) jj++;
			while (pArr[ii].name > str.name) ii--;
		} 
		else 
		{
			while (pArr[jj].name > str.name) jj++;
			while (pArr[ii].name < str.name) ii--;
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}

			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);
}
#endif
