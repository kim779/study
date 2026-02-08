// ChartCommentAddInImp.h: interface for the CChartCommentAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTCOMMENTADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTCOMMENTADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "../Include_AddIn_381931/I381931/_ChartAddInBase.h"		// for CChartAddInBase
#include "SM_CheJanStruct.h"
#include "CommentObj.h"

class CChartCommentAddInImp : public CChartAddInBase
{
public:
	CChartCommentAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartCommentAddInImp();

	static CString		m_strAddInItemName;
	virtual BOOL		InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	virtual BOOL		InvokeAddIn( int p_nCommandType, long p_lData);	

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

protected:
	CArray<CCommentItem*, CCommentItem*>	m_arrCommentList;

	BOOL		m_bRun;
	BOOL		m_bLog;
	BOOL		m_bReverse;
	double		m_dViewMin, m_dViewMax;
	CRect		m_rctGraphRegion, m_rctGraphDrawingRegion;

	CToolTipCtrl m_ToolTip;
	CString		 m_strToolTipText;

	CFont		m_boldFont;

	BOOL		GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion);
	int			GetIndexFromValue(CString strTime, int nStartIndex, int nEndIndex, long& lDate);
	void		TokenizeEx(CString strSrc, CStringArray* pArr, const char* pszTokens);
	void		SetCommentData(CString strData);
	void		SetCommentData(FX_CHE_STRUCT* pDataPosition);
	void		ResetData();
//	void		AddupDrawData(BOOL bBuy, int nStart, int nEnd);

	BOOL		SetItemStatusOnMouseMove(CPoint point);

	void		InitToolTip();
	void		AddToolInTooltip(const CString& strText, const bool bActivate);
	void		SetToolTipText(const CString& strText);
	void		SetToolTipText(const CString& strText, const bool bActivate);

protected:
	virtual void	OnDrawAfterGraph( HDC p_hDC);		// 차트보다 앞 쪽에 그릴 때
	virtual void	OnResetChart( void);	// 차트가 리셋될 때 호출됨
	virtual BOOL	OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
};

#endif // !defined(AFX_CHARTCOMMENTADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
