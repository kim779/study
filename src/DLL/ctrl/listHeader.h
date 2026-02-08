#if !defined(AFX_LISTHEADER_H__15D271C2_28E6_11D5_941A_00001CD7EE63__INCLUDED_)
#define AFX_LISTHEADER_H__15D271C2_28E6_11D5_941A_00001CD7EE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CExListHeader : public CHeaderCtrl
{
public:
	CExListHeader();
	virtual ~CExListHeader();

	//{{AFX_VIRTUAL(CExListHeader)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExListHeader)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	InitBitmap(CBitmap& bm);

private:
	CImageList*	m_ImageList;

};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_LISTHEADER_H__15D271C2_28E6_11D5_941A_00001CD7EE63__INCLUDED_)
