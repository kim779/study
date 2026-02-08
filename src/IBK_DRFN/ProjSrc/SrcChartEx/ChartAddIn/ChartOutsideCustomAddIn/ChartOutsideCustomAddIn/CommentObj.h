// CommentObj.h: interface for the CCommentItem class.
//
//////////////////////////////////////////////////////////////////////
//#include "CISBitmap.h"

#if !defined(AFX_COMMENTOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_)
#define AFX_COMMENTOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_

//#include "Linear.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxwin.h>

#define COMMENT_HEIGHT	32
#define COMMENT_WIDTH	50

class CCommentItem
{
public:
	CCommentItem();
	virtual ~CCommentItem();

	int			m_nIndex;
//	long		m_lOrdCnt;
//	long		m_lOrdSeqNo;
//	long		m_lOrdPrice;
	CString		m_strTradeType;
	CString		m_strDateTime;
	CString		m_strComment;
	BOOL		m_bBuy;
	CString		m_strPrice;
	double		m_fAmount;
	CString		m_strPositionID;		// 포지션번호
	CString		strFXCMPosID;			// 체결번호

//	long		m_lSumOrdCnt;

//	CCommentItem		*m_pSubItem;

	CString		m_strDate;

	void		Draw(CDC* pDC);
	BOOL		Initialize();
	void		SetDate(long lDate);
	void		SetPosition(CDC* pDC, CPoint *pPoint);

	BOOL		IsInRegion(CPoint pt);
	CString		GetToolTipText();

protected:
	CRect		m_rc;
};

#endif // !defined(AFX_COMMENTOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_)
