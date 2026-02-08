// DynamicBitmapLoad.h: interface for the CDynamicBitmapLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNAMICBITMAPLOAD_H__7CF24845_E608_4C12_BF99_136C455300CB__INCLUDED_)
#define AFX_DYNAMICBITMAPLOAD_H__7CF24845_E608_4C12_BF99_136C455300CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDynamicBitmapLoadButton;
class CDynamicBitmapLoad  
{
public:
	void SetFilePath(const CString& filePath);
	CDynamicBitmapLoad();
	virtual ~CDynamicBitmapLoad();

	void LoadBitmap(CDynamicBitmapLoadButton* pbutton, const CString& upfilepath, const CString& downfilepath);

private:
	int m_nRow;
	int m_nButtonGap;
	CPoint m_ptOrgButton;
	CString m_strTotalPath;

	void LoadButtonBitmap(LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal );
	void AttachBitmapToButton(CPoint& btOrgPt, int btWidth, int btHeight, CDynamicBitmapLoadButton* button, CString upImageFileName, CString downImageFileName);
	bool GetImageSize(int& x, int& y);
	void ButtonMoveWindow(CDynamicBitmapLoadButton* button, CPoint& btOrgPt, int btWidth, int btHeight);
	void AutoLoad(CDynamicBitmapLoadButton* button, CString upImageFileName, CString downImageFileName);
	CString GetFilePath(CString fileName);
};

#endif // !defined(AFX_DYNAMICBITMAPLOAD_H__7CF24845_E608_4C12_BF99_136C455300CB__INCLUDED_)
