#ifndef __AXISGDLG_LIB_COMMON_H__
#define __AXISGDLG_LIB_COMMON_H__
#include <afxtempl.h>

/********************************************************************/
// CONTROL MESSAGE class
/********************************************************************/
#include "indiinfo.h"

#define DLG_SETUPVIEW	0
#define DLG_SETUPENV	1
#define DLG_MAINCHART	2
#define DLG_INDICHART	3

#define TYPE_COLOR	0
#define TYPE_WIDTH	1
#define TYPE_KIND	2

#define PARBIT		0x02000000
#define RGBBIT		0x00FFFFFF
#define RGBTOPAL(X)	(PARBIT | X)
#define PALTORGB(X)	(RGBBIT & X)



#define WM_MANAGE	WM_USER + 2003
#define MNG_REDRAW		0x00000001
#define MNG_ALLSAVE		0x00000002
#define MNG_DEFAULT		0x00000004
#define MNG_DEFAULT_CUSTOM_SAVE	0x00000008
#define MNG_DEFAULT_CUSTOM_LOAD	0x00000010
#define MNG_UNION_PREVIEW	100

/********************************************************************/
// Defines class
/********************************************************************/

// from INT to Char;
#define I2B(x, y)	x[0] = (y >> 24) & 0xff; \
			x[1] = (y >> 16) & 0xff; \
			x[2] = (y >> 8) & 0xff;	 \
			x[3] = (y & 0xff)

// from Short to Char;
#define S2B(x,y)	x[0] = (y >> 8) & 0xff;  \
			x[1] = (y & 0xff)

// from char to int
#define B2I(x)		(((unsigned char)x[0] << 24) | ((unsigned char)x[1] << 16) | ((unsigned char)x[2] << 8) | (unsigned char)x[3])

// from char to short
#define B2S(x)		(((unsigned char)x[0] << 8) | (unsigned char)x[1])


/**************************************************************************************/
// Template Information - Function
/**************************************************************************************/

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

	for ( int ii = 0 ; ii < nSize ; ii++ )
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
template <class BASE_CLASS, class KEY, class VALUE>
class CIHMaps : public CTypedPtrMap<BASE_CLASS, KEY, VALUE>
{
public:
	CIHMaps(int nBlockSize = 10) : CTypedPtrMap<BASE_CLASS, KEY, VALUE>(nBlockSize) {}

	~CIHMaps() 
		{ RemoveAll(); }

	void RemoveAll() 
	{	
		VALUE		pObject = NULL;	
		POSITION	pos = NULL;
		KEY		string;
		pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, string, pObject);

			if (pObject != NULL)
			{
				delete pObject;
				pObject = NULL;	
			}
		}

		CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveAll();
	}

	BOOL RemoveKey(KEY key) 
	{
		VALUE		pObject = NULL;	

		if (this->Lookup(key, pObject))
		{
			if (pObject != NULL)
			{
				delete pObject;
				pObject = NULL;
			}

			return CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveKey(key);	
		}
		
		return FALSE;
	}

	BOOL IsExist(KEY key)
	{
		VALUE		pObject = NULL;	

		if (Lookup(key, pObject))
		{
			return TRUE;		
		}
		return FALSE;
	}

	VALUE GetAt(int nIndex)
	{
		int		ii = 0;
		KEY		key;
		VALUE		pObject = NULL;
		POSITION	pos = NULL;
		
		pos = this->GetStartPosition();
		
		while (pos)
		{
			this->GetNextAssoc(pos, key, pObject);
			if (nIndex == ii)
				break;
			ii++;
		}

		return pObject;
	}	

	VALUE GetItem(KEY key)
	{
		VALUE		pObject = NULL;
		
		this->Lookup(key, pObject);
		return pObject;
	}
};

template <class BASE_CLASS, class KEY, class VALUE>
class CIHApartMaps : public CTypedPtrMap<BASE_CLASS, KEY, VALUE>
{
public:
	CIHApartMaps(int nBlockSize = 10) : CTypedPtrMap<BASE_CLASS, KEY, VALUE>(nBlockSize) {}
	~CIHApartMaps() { RemoveAll(); }

	void RemoveAll() 
	{	
		VALUE		pObject = NULL;	
		POSITION	pos = NULL;
		KEY		string;
		pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, string, pObject);

			if (pObject != NULL)
			{
				delete [] pObject;
				pObject = NULL;
			}
		}

		CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveAll();
	}

	BOOL RemoveKey(KEY key) 
	{
		VALUE		pObject = NULL;	

		if (this->Lookup(key, pObject))
		{
			if (pObject != NULL)
			{
				delete [] pObject;
				pObject = NULL;
			}

			return CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveKey(key);	
		}
		
		return FALSE;
	}

	BOOL IsExist(KEY key)
	{
		VALUE		pObject = NULL;	

		if (Lookup(key, pObject))
		{
			return TRUE;		
		}
		return FALSE;
	}

	VALUE GetAt(int nIndex)
	{
		int		ii = 0;
		KEY		key;
		VALUE		pObject = NULL;
		POSITION	pos = NULL;
		
		pos = this->GetStartPosition();
		
		while (pos)
		{
			this->GetNextAssoc(pos, key, pObject);
			if (nIndex == ii)
				break;
			ii++;
		}

		return pObject;
	}	

	VALUE GetItem(KEY key)
	{
		VALUE		pObject = NULL;
		
		this->Lookup(key, pObject);
		return pObject;
	}
};

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

		for ( ii = 0 ; ii < GetSize() ; ii++ )
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

template<class TYPE, class ARG_TYPE>
class CIHApartArray : public CArray<TYPE, ARG_TYPE>
{
public:
	CIHApartArray() : CArray<TYPE, ARG_TYPE>() {};
	~CIHApartArray() { RemoveAll(); }

	void RemoveAt(int nIndex, int nCount = 1)
	{
		TYPE	pObject = NULL;

		pObject = GetAt(nIndex);

		if (pObject != NULL)
		{
			delete [] pObject;
			pObject = NULL;
		}

		CArray<TYPE, ARG_TYPE>::RemoveAt(nIndex, nCount);
	}

	void RemoveAll()
	{
		TYPE		pObject = NULL;
		POSITION	pos = NULL;
		int		ii = 0;

		for ( ii = 0 ; ii < GetSize() ; ii++ )
		{
			pObject = GetAt(ii);

			if (pObject != NULL)
			{
				delete [] pObject;
				pObject = NULL;
			}
		
		}	

		CArray<TYPE, ARG_TYPE>::RemoveAll();
	}
};

/**************************************************************************************/
// class
/**************************************************************************************/

class CHART_INFO
{
public:
	CHART_INFO() { Reset(); }
	~CHART_INFO() { Reset(); }

	void Reset()
	{
		m_sztitle = _T("");
		m_nKind = 0;
		m_nGroup = 0;
		m_szGroup = _T("");
	}

	CString	m_sztitle;
	int	m_nKind;	
	int	m_nGroup;
	CString	m_szGroup;
};

struct _graphprop
{
	int	kind;	// 그래프 종류
	int	vcnt;	// value의 갯수
	int	lcnt;	// 라인의 갯수
	int	ccnt;	// color count;
};

struct _kindmatch {
	int	kind;
	char	str[32];
	short	group;					
};

// group == 0 Main chart 
// group == 1 추세지표
// group == 2 모멘텀지표
// group == 3 거래량 지표
// group == 4 시장강도지표
// group == 5 채널지표
// group == 6 기타지표

static struct _kindmatch match[] = {
// 주차트
	{ GK_JPN,	"봉차트" },
	{ GK_LIN,	"선차트" },
	{ GK_AVOL,	"매물차트"},
	{ GK_BALANCE,	"일목균형표"}, 
	{ GK_CDV,	"CandleVolume"},
	{ GK_EQV,	"EquiVolume"},
	{ GK_THREE,	"삼선전환도"}, 
	{ GK_PNF,	"P&F차트"},
	{ GK_CLOCK,	"역시계곡선"},
	{ GK_PV,	"PV차트"},
	{ GK_BAR,	"바차트" },	
	{ GK_POLE,	"POLE차트" },	
	{ GK_KAGI,	"Kagi차트"},
	{ GK_RENKO,	"Renko차트"},

// 보조차트 - 추세지표
	{GK_PMA, "가격 이평", 1},	
	{GK_NET, "그물차트", 1}, 
	{GK_ADLINE, "A/D Line",1},	
	{GK_ADX, "ADX",	1},	
	{GK_CCI, "CCI",	1},	
	{GK_DMI, "DMI",	1},
	{GK_DEMARK, "Demark", 1},	
	{GK_MACD, "MACD", 1},	
	{GK_MACDOS, "MACD Oscillator", 1}, 
	{GK_PIVOT, "Pivot Line", 1}, 
	{GK_PMAO, "Price Oscillator", 1},	
	{GK_RSI, "RSI", 1}, 
	{GK_RSIWEIGHT, "RSI(가중치)", 1}, // 20090928 : ADD : RSI 지표 추가
	{GK_TRIX, "TRIX", 1}, 
	{GK_WPR, "William's %R", 1},
	{GK_PARB, "Parabolic", 1},

// 보조차트 - 모멘텀지표
	{GK_GAP, "이격도", 2},	
	{GK_MOMENTUM, "모멘텀", 2}, 
	{GK_ABRATIO, "ABRatio", 2},
	{GK_BANDWIDTH, "Band Width", 2},
	{GK_CO, "Chaikin's Oscillator", 2}, 
	{GK_CV, "Chaikin's Volatility", 2}, 
	{GK_EMV, "Ease of Movement", 2},	
	{GK_LFI, "LFI", 2},
	{GK_NCO, "Net Change Oscillator", 2},
	{GK_PROC, "Price Rate-of-Change", 2},
	{GK_SIGMA, "Sigma", 2}, 
	{GK_SONA, "SONAR", 2},	
	{GK_SONAMOMENTUM, "SONA Momentum", 2}, 
	{GK_STOCHS, "Slow Stochastics", 2},
	{GK_STOCHF, "Fast Stochastics", 2},
	
// 보조차트 - 거래량 지표
	{ GK_VOL,	"거래량",		3},
	{ GK_VMA,	"거래량이평",		3},
	{ GK_RRATE,	"회전율",		3},
	{ GK_AMT,	"거래대금",		3},
	{ GK_AMA,	"거래대금이평",		3},
	{ GK_ADR,	"ADR",			3},
	{ GK_MFI,	"Money Flow Index",			3},
	{ GK_OBV,	"OBV",			3},
	{ GK_VMAO,	"Volume Oscillator",	3},
	{ GK_VR,	"VR",		3},
	{ GK_VROC,	"Volume Rate-of-Change",	3},	
	
// 보조차트 - 시장강도 지표
	{ GK_PSY,	"심리도",		4},	
	{ GK_NPSY,	"신심리도",		4},

// 보조차트 - 채널 지표
	{ GK_BOLB,	"Bollinger Band",	5},
	{ GK_ENVL,	"Envelope",		5},

// 보조차트 - 기타지표
	{ GK_FRGNHAVE,  "외국인보유(수량)", 6},
	{ GK_FRGNRATE,  "외국인보유(비율)", 6},
	{ GK_PSNLPBUY,  "개인순매수(수량)", 6},
	{ GK_FRGNPBUY,  "외국인순매수(수량)", 6},
	{ GK_ORGNPBUY,  "기관순매수(수량)", 6},
	{ GK_PSNLTBUY,  "개인순누적매수(수량)", 6},
	{ GK_FRGNTBUY,  "외국인누적순매수(수량)", 6},
	{ GK_ORGNTBUY,  "기관누적순매수(수량)", 6},


// 보조차트 - 합성시에만 보이는
	{ GK_GUKKI,	"꺽은선",		99},
	{ GK_SWING,	"스윙차트",		99},
	
};

/**************************************************************************************/
// Define
/**************************************************************************************/
#define __MAX_TREE_CHILD__	10

/**************************************************************************************/
// Normal Fuction
/**************************************************************************************/

CRect	LIB_MoveWindow(CWnd* pWnd, CPoint pt);
CString LIB_GetStrToken(int nIndex, CString data, char* token);
CString LIB_GetComboText(CComboBox* pCombo);
void	LIB_MessageBox(char* fmt, ...);
CString LIB_MakeComma(CString data);
CString LIB_MakeParen(struct _graph* pGraph, struct _indiInfo* pIndi);
COLORREF LIB_GetReverseColor(COLORREF color);
void LIB_ShowHide(CWnd* pParent, UINT shflag, UINT first, ...);
char* LIB_VerifyMessage(UINT message);
#endif

	




















	