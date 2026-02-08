#pragma once
// MapWnd.h : header file
//

#include "sheet.h"
/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* parent);
	virtual ~CMapWnd();

// Attributes
public:
	CString _sDomino;
	CWnd* m_parent{};
	std::unique_ptr<CSheet>		m_sheet;
	CButton		m_btnSave;		// save button
	CButton		m_btnOK;		// ok button
	CButton		m_btnCC;		// cancel button
	bool		m_bOpentoMap{};	// 맵에서 설정화면 열었는지 확인
	CStatic		m_stSave;		// 저장관련 유의사항
#ifdef	CATCHDIV
	CButton		m_btnCatch;		// cancel button
#endif
	CString		m_user;			// user id
	CString		m_root;			

	int		m_nblc{};
	CStringArray	m_nblary;
	CStringArray	m_gidary;

	CStringArray	m_nblConfigary;

	enum	{ statRUN = 0, statWAIT = 1 } m_status;

	bool		m_autosave{};
	CString		m_guide;	
	int		m_uploadCount{};		// 업로드 횟수
	bool		m_isUploadEnd{};		// 업로드 계속 할 것인지 여부

	bool		m_bBtnSave{};			//2016.08.18 KSJ 저장 버튼을 눌렀는지
	bool		_binitGroup{};
// Operations
public:
//	CString send(CString sqen, CString type);

	void	uploadAction(bool init = false);
//	void	uploadActionGroup(bool init = false);
	void	uploadNewGroup();
	void	uploadSequence();
	void	uploadEachGroup(int gno);
	void	uploadEndGroup(int gno);
	void	uploadBackup();
	void	uploadRecovery();
	void	uploadCancel();
	void	uploadOK();

	void	dnloadAction();
	void	dnloadActionX();
	void	dnloadActionR();



	//ELW 추가 모듈
	void	elwIssueDnLoadAction(int selItem);
	void	elwAssetDnLoadAction(int selItem, CString item);
	void	recommandDnLoadAction();

	void	sequenceOubsUP(char* datB, int datL);
	void	parsingOubsUP(char* datB, int datL);
	void	parsingOubsDN(char* datB, int datL);
	void	recoveryOubsUP(char* datB, int datL);
	void	cancelOubsUP(char* datB, int datL);

	void	backupOubsUP(char* datB, int datL);
	void	endOubs(char* datB, int datL);
	void	OKOubs(char* datB, int datL);
	
	void	updnWaitAction(int wait);
	void	parsingDomino(CString datB);
	CString	Variant(int comm, CString data);
	COLORREF GetAxColor(UINT nIndex);

protected:
	CString parseX(CString &srcstr, CString substr);
	void	sendTR(CString trCode, char* datB, int datL, int key = 0);
	CFont*	GetAxFont(CString fName, int point, int style);
	CString	idTOstring(UINT id);
	void	GuideMsg(CString msg);
	bool	GuideMsg(CString msg, bool question);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnSave();
	afx_msg void OnBtnOK();
	afx_msg void OnBtnCC();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnUpDnAction(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void initSaveFile(char* datB = nullptr, bool isSequence = false);
	BOOL ExistFile(CString fullfile);
	void Request_GroupList();
	void Request_GroupCode(int iseq);

	BOOL m_bBookFileProcess{};
	BOOL CheckBookFileProcess();
	BOOL CheckFileModificationTime(const CString& filePath);
public:
	CString m_slog;
	BOOL ChangeBookFile(int index, CString strFileA, CString strFileB, CStringArray& strarr);
	BOOL ExitBookFile(CString strBookFile);
};

