#pragma once


// CSTListCtrl

class CSTListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSTListCtrl)

public:
	CSTListCtrl();
	virtual ~CSTListCtrl();

	CString m_szFolderName;
	CString m_szFullPath;
	int		m_nSTFolderType;	// 사용자(1), 제공형태(0)
	int		m_nFolderIndx;
	LOGFONT	m_logFont;	
	CFont	m_fontBold;
	CFont*	m_pOldFont;
	CDC*	m_pDC;
	BOOL	m_bUseBoldFont;
	CString m_szRealFolderName;

	void InitFont(BOOL bUseBoldFont);

	CDWordArray m_UserSelState;
	void  SetUserSetting(int nType, int nIndex, BOOL bValue);
	BOOL  GetUserSetting(int nType, int nIndex);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
};
