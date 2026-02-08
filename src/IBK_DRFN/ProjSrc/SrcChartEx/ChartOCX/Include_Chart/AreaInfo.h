#ifndef __AREA_INFO__
#define	__AREA_INFO__

//20081129 이문수 >>
class CAreaInfo
{
public:

	int	m_nLeft;
	int	m_nCenter;
	int	m_nRight;

	int	m_nStart;
	int	m_nEnd;	

	int	m_nPrevStart;
	int	m_nPrevEnd;

public:

	CAreaInfo()
	{
		m_nLeft			= 0;
		m_nCenter		= 0;
		m_nRight		= 0;
		
		m_nStart		= 0;
		m_nEnd			= 0;
		m_nPrevStart	= 0;
		m_nPrevEnd		= 0;
	}

	CAreaInfo(CAreaInfo & Info)
	{
		Copy(Info);
	}

	CAreaInfo & operator=(CAreaInfo & Info)
	{
		Copy(Info);
	}

	void Copy(CAreaInfo & Info)
	{
		m_nLeft			= Info.m_nLeft;
		m_nCenter		= Info.m_nCenter;
		m_nRight		= Info.m_nRight;
		m_nStart		= Info.m_nStart;
		m_nEnd			= Info.m_nEnd;
		m_nPrevStart	= Info.m_nPrevStart;
		m_nPrevEnd		= Info.m_nPrevEnd;
	}
};
typedef CList < CAreaInfo *, CAreaInfo * > CAreaInfoList;

class CAreaInfoListManager
{

public:

	BOOL			m_bXShift;
	CAreaInfoList	m_AreaList;


public:

	CAreaInfoListManager()
	{
		m_bXShift = FALSE;
	}

	~CAreaInfoListManager()
	{
		while(!m_AreaList.IsEmpty()) delete m_AreaList.RemoveHead();
	}	

};
//20081129 이문수 <<

#endif
