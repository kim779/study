#ifndef __DRAW_DATA__
#define	__DRAW_DATA__

//20081129 이문수 >>
////////////////////////////////////////////////////////////
//Data
#include <afxtempl.h>

typedef CList < int, int > CIntList;
class CDrawBaseData
{
public:

	int		m_nLeft;
	int		m_nCenter;
	int		m_nRight;

public:

	CDrawBaseData()
	{
		m_nLeft = m_nCenter = m_nRight = 0;
	}

	CDrawBaseData(int nLeft, int nCenter, int nRight)
	{
		m_nLeft = nLeft, m_nCenter = nCenter, m_nRight = nRight;
	}

	virtual ~CDrawBaseData(){;}
};

class CDrawTickData : public CDrawBaseData
{

public:

	CDrawTickData()
	{
		m_nHighIndex = -1;
		m_nLowIndex  = -1;
		m_nLeft = m_nCenter = m_nRight = 0;
		m_pHighHigh = NULL;	m_pHighLow = NULL;	m_pHighStart= NULL;	m_pHighEnd = NULL;
		m_pLowHigh = NULL;	m_pLowLow = NULL;	m_pLowStart= NULL;	m_pLowEnd = NULL;
	}

	CDrawTickData(int nLeft, int nCenter, int nRight):CDrawBaseData(nLeft, nCenter, nRight)
	{
		m_nHighIndex = -1;
		m_nLowIndex  = -1;
		m_pHighHigh = NULL;	m_pHighLow = NULL;	m_pHighStart= NULL;	m_pHighEnd = NULL;
		m_pLowHigh = NULL;	m_pLowLow = NULL;	m_pLowStart= NULL;	m_pLowEnd = NULL;
	}

	virtual ~CDrawTickData(){ m_DrawDataItemFromDTIList.RemoveAll();}

	CIntList	m_DrawDataItemFromDTIList;


public:

	//기준선 상단중에서 고,저
	int				m_nHighIndex;	
	POINT *			m_pHighHigh;
	POINT *			m_pHighLow;
	POINT *			m_pHighStart;
	POINT *			m_pHighEnd;

	//기준선 하단 중에서 고,저
	int				m_nLowIndex;	
	POINT *			m_pLowHigh;
	POINT *			m_pLowLow;
	POINT *			m_pLowStart;
	POINT *			m_pLowEnd;

};

class CDrawSimpleLineData : public CDrawBaseData
{

public:

	CDrawSimpleLineData()
	{
		m_nIndex = -1;
		m_nLeft = m_nCenter = m_nRight = 0;
		m_pHigh = NULL;	m_pLow = NULL;	m_pStart= NULL;	m_pEnd = NULL;		
	}

	CDrawSimpleLineData(int nLeft, int nCenter, int nRight):CDrawBaseData(nLeft, nCenter, nRight)
	{
		m_nIndex = -1;		
		m_pHigh = NULL;	m_pLow = NULL;	m_pStart= NULL;	m_pEnd = NULL;
	}

	virtual ~CDrawSimpleLineData(){m_DrawDataItemFromDTIList.RemoveAll();}

	CIntList	m_DrawDataItemFromDTIList;


public:

	//시,고,저,종
	int				m_nIndex;	
	POINT *			m_pHigh;
	POINT *			m_pLow;
	POINT *			m_pStart;
	POINT *			m_pEnd;

};


class CDrawBarType
{
public:

	int			m_nType;

	int			m_nHigh;

	CIntList	m_DrawDataItemFromDTIList;

public:

	CDrawBarType()
	{
		m_nType = 0;
		m_nHigh = 0;
	}


};
typedef CList < CDrawBarType * , CDrawBarType * > CDrawBarTypeList;


class CDrawSimpleBarData : public CDrawBaseData
{
public:

	CRect				m_rect;

	CDrawBarTypeList	m_DrawBarTypeList;

public:
	CDrawSimpleBarData()
	{
		m_rect = CRect(0, 0, 0, 0);
	}


	virtual ~CDrawSimpleBarData()
	{
		while(!m_DrawBarTypeList.IsEmpty())
			delete m_DrawBarTypeList.RemoveHead();
	}
	
};

typedef CList < CDrawBaseData * , CDrawBaseData * > CDrawDataList;

////////////////////////////////////////////////////////////
//Manager

#define	TICK_DRAW_DATA_TYPE			1
#define	SIMPLE_LINE_DRAW_DATA_TYPE	2
#define	SIMPLE_BAR_DRAW_DATA_TYPE	3


class CDrawBaseDataManager
{
public:

	int				m_nType;

	POSITION		m_pos;

	CDrawDataList	m_DrawDataList;

	BOOL			m_bShowFullData;//전체보기 여부

	int				m_nDTIStart;

	int				m_nDTIEnd;

	POSITION		m_posDTIStart;

	POSITION		m_posDTIEnd;

public:

	CDrawBaseDataManager()
	{
		m_nType = 0;

		m_nDTIStart = -1;

		m_nDTIEnd	= -1;

		m_posDTIStart=NULL;

		m_posDTIEnd  =NULL;

		m_bShowFullData=FALSE;
	}

	virtual ~CDrawBaseDataManager();		

public:

	virtual POSITION		DrawDataMoveToHead(){return NULL;}


	virtual POSITION		DrawDataMoveToNext(POSITION pos){return NULL;}


	virtual POSITION		DrawDataMoveToPrev(POSITION pos){return NULL;}


	virtual POSITION		DrawDataMoveToTail(){ return NULL;}	



	virtual POSITION		AppendDrawData(int nLeft, int nCenter, int nRight){return NULL;}


	virtual BOOL			AddDrawDataItem(POSITION pos, int nDrawDataItem){return FALSE;}


	virtual BOOL			DeleteFirstDrawDataItem(POSITION pos){return FALSE;}
};


class CDrawTickDataManager : public CDrawBaseDataManager
{
public:


	CDrawTickDataManager();

	CDrawTickDataManager(int nSize, int nBase);

	virtual ~CDrawTickDataManager();


public:


	//1.			BongDraw에서 사용
	int				m_nLineHighSize;
	int				m_nLineLowSize;

	int				m_nPolyHighSize;
	int				m_nPolyLowSize;

	int				m_nPolyHighRealSize;
	int				m_nPolyLowRealSize;

	POINT *			m_pLineHigh;
	POINT *			m_pLineLow;

	DWORD *			m_pPolyHigh;
	DWORD *			m_pPolyLow;


	int				m_nDrawDataSize;
	
	int				m_nBase;//기준가 (기준가 기준 상승선과 하락선 색 칠하기)

public:


	virtual POSITION		DrawDataMoveToHead();


	virtual POSITION		DrawDataMoveToNext(POSITION pos);


	virtual POSITION		DrawDataMoveToPrev(POSITION pos);


	virtual POSITION		DrawDataMoveToTail();


	virtual POSITION		AppendDrawData(int nLeft, int nCenter, int nRight);


	virtual BOOL			AddDrawDataItem(POSITION pos, int nDrawDataItem);


	virtual BOOL			DeleteFirstDrawDataItem(POSITION pos);
	
public:
	int		SetBase(int	nBase)
	{
		m_nBase = nBase;
	}

	BOOL	AllocBuffer(int nSize);
	
	BOOL	FreeBuffer();
		
};

class CDrawSimpleLineDataManager : public CDrawBaseDataManager
{
public:


	CDrawSimpleLineDataManager();

	CDrawSimpleLineDataManager(int nSize);

	virtual ~CDrawSimpleLineDataManager();


public:


	//1.			BongDraw에서 사용
	int				m_nLineSize;
	
	int				m_nRealSize;
	
	POINT *			m_pLine;	

	int				m_nDrawDataSize;

public:
	
	BOOL	AllocBuffer(int nSize);
	
	BOOL	FreeBuffer();
		
};

class CDrawSimpleBarDataManager : public CDrawBaseDataManager
{
public:
	CDrawSimpleBarDataManager();

	virtual ~CDrawSimpleBarDataManager();

public:
	
	POSITION		m_posDTIEndClose;

	POSITION		m_posDTIEndOpen;

	POSITION		m_posDTIEndVolume;

	POSITION		m_posDTIStartClose;

	POSITION		m_posDTIStartOpen;

	POSITION		m_posDTIStartVolume;

};

class CDrawDataParentManager
{

public:
	CDrawBaseDataManager * m_pDrawDataManager;

public:

	CDrawDataParentManager()
	{
		m_pDrawDataManager = NULL;
	}

	virtual ~CDrawDataParentManager()
	{
		delete m_pDrawDataManager;
		m_pDrawDataManager = NULL;
	}

};

//20081129 이문수 <<



#endif