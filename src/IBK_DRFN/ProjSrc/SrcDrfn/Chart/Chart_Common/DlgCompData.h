#if !defined(AFX_DLGCOMPDATA_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_)
#define AFX_DLGCOMPDATA_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_

//#include "../Chart_Common/DlgCompData.h"

#include <AFXTEMPL.H>

DECLARE_INTERFACE(IDlgCompDataHandle)
{
	CString m_szDataFile;

	STDMETHOD_(void, LoadData)(long pDataKey) PURE;
	STDMETHOD_(void, SaveData)(long pDataKey) PURE;

	//기타 개별저장이 필요한 저장.
	STDMETHOD_(void, SaveSelGroup)(long pDataKey) PURE;
	STDMETHOD_(void, SaveShowGroupInfo)(long pDataKey) PURE;
	STDMETHOD_(void, SaveSelCodeInfo)(long pDataKey) PURE;
	STDMETHOD_(void, SaveTimeUse)(long pDataKey) PURE;

	// 종료시등에서 데이터 클리어할 때 Call
	STDMETHOD_(void, ClearAll)(long pDataKey) PURE;

	// 그룹정보를 클리어한다. 
	//List_CDlgCompGroupData* pGroupList
	STDMETHOD_(void, ClearGroupList)(long pGroupList) PURE;

	// 그룹정보를 복사한다.
	//List_CDlgCompGroupData* pOrg, List_CDlgCompGroupData* pTarget
	STDMETHOD_(int,  CopyGroupList)(long pOrg, long pTarget) PURE;
	
	// 처음실행일때의 디폴트처리용
	STDMETHOD_(void, DoCheckFirst)() PURE;
};

class CDlgCompSubItem
{
public:
	CString		m_szCode;
	CString		m_szCodeName;
	COLORREF	m_clr;
	int			m_nDrawStyle;
				//	Chart OCX에서 사용하는 코드.
				//		enum LINEDRAWSTYLE		// LINESTYLE
				//		{
				//			Solid_Draw = 0,			// 실선
				//			Dot_Draw,				// 좌표점형
				//			DottedLine_Draw,		// 선형 점선
				//			DashDot_Draw,			// 선점형
				//			DashDotDot_Draw			// 선점점형
				//		};
	int			m_nThickness;
};
#define INIT_CDlgCompSubItem(xItem) {xItem.m_clr=RGB(255,0,0); xItem.m_nDrawStyle=0; xItem.m_nThickness=0;}
#define INIT_CDlgCompSubItemP(pItem) {pItem->m_clr=RGB(255,0,0); pItem->m_nDrawStyle=0; pItem->m_nThickness=0;}

typedef CList<CDlgCompSubItem*, CDlgCompSubItem*> List_CDlgCompSubItem;
class CDlgCompGroupData
{
public:
	CString			m_szGroupName;
	List_CDlgCompSubItem m_ItemList;
};

typedef CList<CDlgCompGroupData*, CDlgCompGroupData*> List_CDlgCompGroupData;
typedef struct //CDlgCompData
{
public:
	List_CDlgCompGroupData	m_GroupList;
	int		m_nGroupCnt;	// 그룹Count
	int		m_nBase;		// 비교 기준값. 0:전일종가,1:당일시가,1:현재가
	int		m_nTimer;		// 돌려보기(몇초인지)
	BOOL	m_bCurDisp;		// 현재가(종가)표시 여부
	int		m_nTypeUnit;	// 몇분봉인지.
	
	CString m_szSelectGroup;	// 사용자가 선택한 그룹
	CString	m_szSelCode, m_szSelCodeName;	// 1갭별종목보기 일대 선택된 종목정보.
	BOOL	m_bShowAll;			// 1:기본그룹으로 보기, 0:개별종목보기
	BOOL	m_bTimerUse;		// 돌려보기 사용여부.

	IDlgCompDataHandle*	m_pFn;
} STDlgCompData;

#endif // !defined(AFX_DLGCOMPDATA_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_)
