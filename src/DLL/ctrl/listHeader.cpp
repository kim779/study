#include "stdafx.h"
#include "listHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExListHeader::CExListHeader()
{
	m_ImageList = NULL;
}

CExListHeader::~CExListHeader()
{
	if (m_ImageList)
	{
		m_ImageList->DeleteImageList();
		delete m_ImageList;
	}
}

BEGIN_MESSAGE_MAP(CExListHeader, CHeaderCtrl)
	//{{AFX_MSG_MAP(CExListHeader)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExListHeader::InitBitmap(CBitmap& bm)
{
	CBitmap& bitmap = bm;

	m_ImageList = new CImageList();
	m_ImageList->Create(16, 16, ILC_COLORDDB | ILC_MASK, 4, 1); 
	m_ImageList->Add(&bitmap, RGB(255, 0, 255)); 
	SetImageList(m_ImageList); 

/*	HDITEM hdi;

	UINT g_uHDRStyles[3];
	for (int ii = 0; ii < this->GetItemCount(); ii++)
	{
		GetItem(ii, &hdi);
		hdi.fmt |= g_uHDRStyles[ii%3] | HDF_IMAGE;
		hdi.mask |= HDI_IMAGE | HDI_FORMAT;
		hdi.iImage =  ii;
		SetItem(ii, &hdi);
	}
*/
}