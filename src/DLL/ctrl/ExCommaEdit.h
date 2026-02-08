#if !defined(AFX_FLOATEDIT_H__449B9681_9A73_11D2_B5AC_5254ABDD288C__INCLUDED_)
#define AFX_FLOATEDIT_H__449B9681_9A73_11D2_B5AC_5254ABDD288C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif

class AFX_EXT_CLASS CCommaEdit : public CEdit
{
public:
	CCommaEdit();
	virtual ~CCommaEdit();

	double GetValue(void);
	void SetValue(double dblValue);

	int GetDecimals(void);
	void SetDecimals(int nDecimals);

	void EnableCommas(BOOL bNewHasCommas);

	void SetComma(char cNewComma);
	char GetComma(void);

	void SetDecimalPoint(char cNewDecimal);
	char GetDecimalPoint(void);

	void LocalSettings(void);
                       
private:

	double	m_dblCurrentValue;
	BOOL	m_bCommas;
	char	m_cComma;
	char	m_cDecimalPoint;
	int	m_nDecimals;
	CString m_strSavedString;
	int	m_nSavedSelection{};


	//{{AFX_VIRTUAL(CCommaEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


private:
	void removeFirstComma(CString &);
	void processSign( UINT nCharacter);
	void processCharacter( UINT cCurrent);
	void toString(double dblValue, CString & strCurrent);

	void putCommas(CString strSource, CString & strDestination);
	void removeCommas( CString & strSource);

	void deleteCharLeft(CString& strSource, int nSelection, int nNumber);
	void deleteCharRight(CString& strSource, int nSelection, int nNumber);

	BOOL bIsRightCharacterSign(CString strCurrent, int nSelection);
	BOOL bIsLeftDecimal(CString strSource, int nSelection);

	BOOL bIsLeftCharacterComma(CString strSource, int nSelection);
	BOOL bIsRightCharacterComma(CString strSource, int nSelection);

	BOOL bIsLeftCharacterDecimal(CString strSource, int nSelection);
	BOOL bIsRightCharacterDecimal(CString strSource, int nSelection);

	void getStringAndSelection(CString& strCurrent, int & nSelection);
	void setStringAndSelection(CString strNew, int nSelection);

	void getIntegerAndDecimal(CString strCurrent, CString & strInteger, CString & strDecimal);
	void saveStringAndSelection(void);
	void deleteSelection(void);

	void processDigit(UINT cCurrent);
	void processDelete();
	void processBackspace();
	void processDecimalPoint();

	void getLocaleInformation(char & cDecimal, char & cComma);

	BOOL hasSelection(void);

	CString midString(CString strIn, int nStart, int nLength);


protected:
	//{{AFX_MSG(CCommaEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_FLOATEDIT_H__449B9681_9A73_11D2_B5AC_5254ABDD288C__INCLUDED_)
