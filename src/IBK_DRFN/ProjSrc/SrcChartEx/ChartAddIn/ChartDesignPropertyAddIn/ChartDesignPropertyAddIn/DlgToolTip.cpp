// DlgToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"

#include "DlgToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgToolTip dialog


CDlgToolTip::CDlgToolTip()//(CWnd* pParent /*=NULL*/)
{
	m_nHeight				= 0;
	m_nWidth				= 0;
	m_nFontSize				= 12;	//14	// original size
	m_currentControlID		= 0;
	m_pParentWnd			= NULL;

	m_point.x = 0;
	m_point.y = 0;

	m_aControlInfo.RemoveAll();
	//{{AFX_DATA_INIT(CDlgToolTip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CDlgToolTip::~CDlgToolTip()
{
	ToolTipInfo *pInfo = NULL;
	int nSize = m_aControlInfo.GetSize();
	for( int i = 0; i < nSize; i++ )
	{
		pInfo = (ToolTipInfo*)m_aControlInfo.GetAt( i );
		delete pInfo;
	}

	m_aControlInfo.RemoveAll();
}
/////////////////////////////////////////////////////////////////////////////
// CDlgToolTip message handlers

void CDlgToolTip::Create(CWnd* pWnd)
{
	m_pParentWnd = pWnd;
}

void CDlgToolTip::ErasePreviousToolTipDisplay( UINT nControlID )
{
	//This assertion fails because you did not call Create() first.
	//m_pParentWnd was not initialized.
	ASSERT(m_pParentWnd);

	//if erase already, do not do it again
	if((m_nHeight == 0) || (m_nWidth == 0))
		return;
	
	CRect oInfoRect(0,0,0,0);
	CPoint point(m_point.x, m_point.y);

	CalculateInfoBoxRect(nControlID, &oInfoRect, point);
	m_pParentWnd->InvalidateRect(&oInfoRect);
	m_pParentWnd->UpdateWindow();

	m_nHeight = 0;
	m_nWidth = 0;
}


void CDlgToolTip::CalculateHeightAndWidth(CStringArray& straInfos)
{
	//This assertion fails because you did not call Create() first.
	//m_pParentWnd was not initialized.
	ASSERT(m_pParentWnd);
	m_nTotalLine = 0;
	int nMaxLength = 0;
	int nLength;
	int nLine = straInfos.GetSize();
	for(int i=0; i<nLine; i++)
	{
		nLength = (straInfos[i]).GetLength();
		if(nLength > nMaxLength)
			nMaxLength = nLength;
	}

	//LOGFONT lFont;
	//GetObject(GetStockObject(ANSI_VAR_FONT), sizeof(lFont), &lFont);

//	nLine++;  //닫기라는 버튼을 말기 위해서...
	m_nTotalLine = nLine;		// holder for maximum line of information
	m_maxCharInLine = (nMaxLength +1);		// holder for longest char info
	m_nHeight = (12 + m_nTotalLine * (m_nFontSize +1)); // 12 + nLine * (m_nFontSize - 1);	//m_nFontSize is the font size
	m_nWidth = 10 + (int)(6 * (nMaxLength+1));	//aproximately 5 pixel per char ///7.5
}

void CDlgToolTip::CalculateInfoBoxRect(UINT nControlID, CRect* pInfoRect, CPoint& point)
{
	ASSERT(m_pParentWnd);
	
	CRect		oRect(0,0,0,0);
	CRect		oParentWindowRect(0,0,0,0);
	
	m_pParentWnd->GetWindowRect(&oParentWindowRect);
	m_pParentWnd->ScreenToClient(&oParentWindowRect);
	
	CWnd* pWnd = m_pParentWnd->GetDlgItem(nControlID);
	ASSERT(pWnd);
	pWnd->GetWindowRect(&oRect);
	m_pParentWnd->ScreenToClient(&oRect);

	//check if the box fit in the parent dialog 
	SetFontSize( 12 );  //14
	int nBottom = oRect.bottom - ((oRect.bottom - oRect.top)/2) + m_nHeight;
	if(nBottom <= oParentWindowRect.bottom)
	{
//		pInfoRect->top = oRect.bottom - (oRect.bottom - oRect.top)/2;
//		pInfoRect->bottom = pInfoRect->top + m_nHeight;

//		if((oParentWindowRect.bottom - oRect.bottom) < m_nHeight)	{
//			pInfoRect->top = oRect.top - m_nHeight ;
			pInfoRect->top = (point.y +10);
			pInfoRect->bottom = pInfoRect->top + m_nHeight;
//		}
//		else {
//			pInfoRect->top = oRect.bottom ;
//			pInfoRect->bottom = pInfoRect->top + m_nHeight;
//		}

	}
	else
	{
		// change the size of the font as well as the info box if all
		// info data cannot be viewed
		if( m_nHeight > (oParentWindowRect.bottom - oParentWindowRect.top) )
		{
			SetFontSize( 12 );  //12
			m_nHeight = (12 + m_nTotalLine * (m_nFontSize +1));	//m_nFontSize is the font size
			m_nWidth = 10 + (int)(6 * m_maxCharInLine);	//7
		}
		// end
		pInfoRect->top = oRect.top - m_nHeight ;
		pInfoRect->bottom = pInfoRect->top + m_nHeight;

//		pInfoRect->bottom = oParentWindowRect.bottom - 1;
//		pInfoRect->top = pInfoRect->bottom - m_nHeight;
	}

	//check if the box fit in the parent dialog
//	int nRight = (oRect.left + oRect.right)/2 + m_nWidth;
	int nRight = point.x;
	if((nRight+ m_nWidth) <= oParentWindowRect.right)
	{
		pInfoRect->left = point.x;
		pInfoRect->right = pInfoRect->left + m_nWidth;

//		pInfoRect->left = (oRect.left + oRect.right)/2;
//		pInfoRect->right = pInfoRect->left + m_nWidth;

	}
	else
	{
//		int nLeft = (oRect.left + oRect.right)/2 - m_nWidth;
		int nLeft = point.x;
//		if(nLeft <= oParentWindowRect.left)
		if((nLeft-m_nWidth) <= oParentWindowRect.left)
		{
			pInfoRect->left = oParentWindowRect.left + 1;
			pInfoRect->right = pInfoRect->left + m_nWidth;
		}
		else
		{
			pInfoRect->right = point.x;
			pInfoRect->left = pInfoRect->right - m_nWidth;

//			pInfoRect->right = (oRect.left + oRect.right)/2;
//			pInfoRect->left = pInfoRect->right - m_nWidth;

		}
	}

	m_rcToolTip.top = pInfoRect->top;
	m_rcToolTip.left = pInfoRect->left;
	m_rcToolTip.right = pInfoRect->right;
	m_rcToolTip.bottom = pInfoRect->bottom;

	ASSERT(pInfoRect->top <= pInfoRect->bottom);
	ASSERT(pInfoRect->left <= pInfoRect->right);
}

void CDlgToolTip::ShowToolTip( UINT nControlID, CPoint& point )
{		
	ToolTipInfo *pToolTip = IsControlIDExisting( nControlID );
	if( pToolTip == NULL )
		return;
				
	DisplayInfo( pToolTip , (CPoint)point);
}


void CDlgToolTip::ShowToolTip( CPoint& point )
{						
	CWnd* pWnd = m_pParentWnd->ChildWindowFromPoint(point);
	if( pWnd )
	{
		UINT nControlID = (UINT)pWnd->GetDlgCtrlID();
//		if( m_currentControlID != nControlID )
		{
			ErasePreviousToolTipDisplay( m_currentControlID );
			ShowToolTip( nControlID, (CPoint)point );
			m_point.x = point.x;
			m_point.y = point.y;
		}

/*
		if(point.x >= m_ptButtonSize.left && point.y >= m_ptButtonSize.top && point.x <= m_ptButtonSize.right && point.y <= m_ptButtonSize.bottom)
		{
			HideToolTip(point);
		}
		else
		{
			UINT nControlID = (UINT)pWnd->GetDlgCtrlID();
	//		if( m_currentControlID != nControlID )
			{
				ErasePreviousToolTipDisplay( m_currentControlID );
				ShowToolTip( nControlID, (CPoint)point );
				m_point.x = point.x;
				m_point.y = point.y;
			}
		}
*/
	}
}

void CDlgToolTip::DisplayInfo( ToolTipInfo* pToolTip , CPoint& point)
{
	TRACE("DisplayINfo\n");
	if( pToolTip->nInfoSize <= 0 )
		return;

	ASSERT(m_pParentWnd);
	CDC* pDC = m_pParentWnd->GetDC();

	CRect		oInfoRect;
	CBrush		oBrush, *pOldBrush, oBorderBrush;
	int			nX, nY;
	TEXTMETRIC	TM;
	int			nTextHigh;
	CFont		oFont, *pOldFont;
	CWnd*		pWnd = NULL;

	pDC->SetBkMode(TRANSPARENT);
	
	oBrush.CreateSolidBrush( pToolTip->nBackColor );

	pOldBrush = pDC->SelectObject( &oBrush );
	pDC->SetTextColor( pToolTip->nTextColor );

	//calculate the width and height of the box dynamically
	CalculateHeightAndWidth( pToolTip->nControlInfo );
	CalculateInfoBoxRect( pToolTip->nControlID, &oInfoRect, point);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = 0x81; // 한글 Character Set
	lf.lfHeight = 12; // 20 point 크기
	strcpy(lf.lfFaceName, "Microsoft Sans Serif"); // 궁서체로 font setting
	oFont.CreateFontIndirect(&lf); 

//	oFont.CreateFont(m_nFontSize, 6, 0, 0, FW_REGULAR, 0, 0, 0, 0, 0, 0, 0, 0, "굴림체");  //"Courier New"
	pOldFont = pDC->SelectObject(&oFont);
	
	pDC->FillRect(&oInfoRect, &oBrush);
	pDC->SelectObject(pOldBrush);
	oBrush.DeleteObject();

	oBorderBrush.CreateSolidBrush( pToolTip->nTextColor );
	pOldBrush = pDC->SelectObject(&oBorderBrush);
	DrawEdge(pDC->m_hDC, &oInfoRect, BDR_RAISEDINNER | BDR_SUNKENOUTER, BF_RECT);
	
	pDC->SetTextAlign(TA_LEFT);
	pDC->GetTextMetrics(&TM);
	nTextHigh = TM.tmHeight + TM.tmExternalLeading - 2;
	nX = oInfoRect.left + 8;
	nY = oInfoRect.top + 5; 

	for( register UINT i = 0; i < pToolTip->nInfoSize; i++)
	{
		pDC->TextOut(nX, nY, pToolTip->nControlInfo[i]);
		nY += m_nFontSize +1;  //-1
	}

/////////////
	//닫기를 쓰기위한 과정.
	m_ptButtonSize.left = oInfoRect.right - (m_nFontSize*3);
	m_ptButtonSize.top = nY+1;
	m_ptButtonSize.right = oInfoRect.right;
	m_ptButtonSize.bottom = oInfoRect.bottom;
//	pDC->TextOut(m_ptButtonSize.left+1, m_ptButtonSize.top+1, "닫기");
//////////////

	pDC->SelectObject(pOldFont);
	oFont.DeleteObject();
	pDC->SelectObject(pOldBrush);
	oBorderBrush.DeleteObject();	
	m_pParentWnd->ReleaseDC(pDC);
}	

BOOL CDlgToolTip::AddControlInfo( UINT contolID, CDWordArray &dwaInfo, COLORREF back, COLORREF text )
{
	ToolTipInfo *pToolTip = new ToolTipInfo;
	ASSERT( pToolTip != NULL );

	int nSize = dwaInfo.GetSize();
	if( pToolTip <= 0 )	// no information, don't add to the list and return FALSE
	{
		delete pToolTip;
		return FALSE;
	}

	ML_SET_LANGUAGE_RES();
	CString strTooltip;
	pToolTip->nControlInfo.RemoveAll();
	pToolTip->nInfoSize  = nSize;
	pToolTip->nControlID = contolID;
	for( register int i = 0; i < nSize; i++ )
	{
		strTooltip.LoadString( dwaInfo[i]);
		pToolTip->nControlInfo.Add( strTooltip);
	}
	// initialize colors, use default if not supplied
	pToolTip->nBackColor = back;
	pToolTip->nTextColor = text;

	// add to the list
	m_aControlInfo.Add( pToolTip );

	return TRUE;
}

ToolTipInfo* CDlgToolTip::IsControlIDExisting( UINT controlID )
{
	ToolTipInfo *pToolTip = NULL;
	int nSize = m_aControlInfo.GetSize();
	for( register int i = 0; i < nSize; i++ )
	{
		pToolTip = (ToolTipInfo*)m_aControlInfo.GetAt( i );
		if( pToolTip->nControlID == controlID )		// if found!
		{
			m_currentControlID = controlID;
			return pToolTip;
		}
	}
	m_currentControlID = 0;		// if not found, reset the current control ID to refresh the display

	return NULL;
}

BOOL CDlgToolTip::RemoveControlInfo( UINT controlID )
{
	ToolTipInfo *pToolTip = NULL;
	int nSize = m_aControlInfo.GetSize();
	for( register int i = 0; i < nSize; i++ )
	{
		pToolTip = (ToolTipInfo*)m_aControlInfo.GetAt( i );
		if( pToolTip->nControlID == controlID )		// if found!
		{				
			if( m_currentControlID == controlID )
				ErasePreviousToolTipDisplay( m_currentControlID );

			m_aControlInfo.RemoveAt( i );	// remove the entry
			delete pToolTip;
			break;		// break from the loop
		}
	}

	return TRUE;
}

void CDlgToolTip::HideToolTip(CPoint &point)
{
	CWnd* pWnd = m_pParentWnd->ChildWindowFromPoint(point);
	if( pWnd )
	{
//		if(m_ptButtonSize.top <= point.y && m_ptButtonSize.left <= point.x && m_ptButtonSize.bottom >= point.y && m_ptButtonSize.right >= point.x) 
		if((m_point.x - 1) >= point.x || (m_point.x +1) <= point.x || (m_point.y - 1) >= point.y || (m_point.y + 1) <= point.y) 
		{
			UINT nControlID = (UINT)pWnd->GetDlgCtrlID();
	//		if( m_currentControlID == nControlID )
			{
				ErasePreviousToolTipDisplay( m_currentControlID );
				m_point.x = point.x;
				m_point.y = point.y;
			}
		}
	}
}

