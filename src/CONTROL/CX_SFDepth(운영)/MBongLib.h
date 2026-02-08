#ifndef __MBONG_LIB_H__
#define __MBONG_LIB_H__

/**************************************************************************************/
// Template Information - Function
/**************************************************************************************/
#include <afxtempl.h>
#include <math.h>

template<class T>
void LIB_DeleteWnd(T*& pWnd)
{
	if (pWnd == NULL)
		return;

	if (::IsWindow(pWnd->m_hWnd))
		pWnd->DestroyWindow();

	delete pWnd;
	pWnd = NULL;
}

template<class T>
BOOL LIB_IsWndAlive(T*& pWnd)
{
	if (pWnd == NULL)
		return FALSE;

	return (::IsWindow(pWnd->m_hWnd));	
}

template <class T>
void LIB_memset(T* array, int nSize, double val = 0.0)
{
	if (nSize <= 0)
		return;

	for (int ii = 0 ; ii < nSize ; ii++)
	{
		array[ii] = (T)val;
	}
}

template <class T>
T* LIB_new(T* arg, int nSize)
{	
	arg = new T[nSize];
	return (arg);
}

template <class T>
void LIB_delete(T*& arg)
{
	if (arg == NULL)
		return;

	delete [] arg;
	arg = NULL;
}

/**************************************************************************************/
// Template Information - Collection Classes
/**************************************************************************************/
template<class TYPE, class ARG_TYPE>
class CIHArray : public CArray<TYPE, ARG_TYPE>
{
public:
	CIHArray() : CArray<TYPE, ARG_TYPE>() {};
	~CIHArray() { RemoveAll(); }

	void RemoveAt(int nIndex, int nCount = 1)
	{
		TYPE	pObject = NULL;

		pObject = GetAt(nIndex);

		if (pObject != NULL)
		{
			delete pObject;
			pObject = NULL;
		}

		CArray<TYPE, ARG_TYPE>::RemoveAt(nIndex, nCount);
	}

	void RemoveAll()
	{
		TYPE		pObject = NULL;
		POSITION	pos = NULL;
		int		ii = 0;

		for (ii = 0 ; ii < GetSize() ; ii++)
		{
			pObject = GetAt(ii);

			if (pObject != NULL)
			{
				delete pObject;
				pObject = NULL;
			}
				

		}	

		CArray<TYPE, ARG_TYPE>::RemoveAll();
	}
};
/**************************************************************************************/
// class
/**************************************************************************************/

class CBongData
{
public:
	CBongData()	{ Reset(); }
	~CBongData()	{ Reset(); }
	void Reset()
	{
		m_date = _T("");
		m_siga = 0.0;
		m_koga = 0.0;
		m_jega = 0.0;
		m_jgga = 0.0;
		m_gvol = 0;
	}

	void SetData(struct _Candle* data)
	{
		CString	tmpstr;
		
		m_date.Format("%.*s", sizeof(data->date), data->date);

		tmpstr.Format("%.*s", sizeof(data->siga), data->siga);
		m_siga = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->koga), data->koga);
		m_koga = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->jega), data->jega);
		m_jega = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->jgga), data->jgga);
		m_jgga = atof(tmpstr);

		tmpstr.Format("%.*s", sizeof(data->gvol), data->gvol);
		m_gvol = atof(tmpstr);
	}
/*
	void SetEmptyData(double val)
	{
		m_date = _T("00000000");
		m_siga = m_koga = m_jega = m_jgga = m_gvol = val;
	}
*/
	CString		m_date;
	double		m_siga;
	double		m_koga;
	double		m_jega;
	double		m_jgga;
	double		m_gvol;
};
#endif

		