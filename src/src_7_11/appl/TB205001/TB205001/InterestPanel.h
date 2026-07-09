#pragma once

#include "NButton.h"
#include "Panel.h"
#include "GpEdit.h"
#include "../../../h/jmcode.h"
// CInterestPanel 대화 상자입니다.

#define WM_BTNMSG WM_USER + 888
class CCodeData
{
public:
	CString strName;
	CString strCode;
	CString strsuik;
	CString strMangi;
	bool	bFlag;
	COLORREF clrData;
};

typedef struct _CJcode{
	CString code;	// 채권 코드... 
	CString name;	// 채권이름...
	CString		kind;  // 채권종류
	CString     idct;  //지표구분
	CString		valday;  //발행일
	CString		mtday; //만기일
	CString	    mtry; //만기구분	       
	CString		itrt;  //금리 
	CString		intertype; //이자유형
	CString		credit;  //신용등급
	CString		minthree;  //민평3
	CString		minfore;  //민평4
	CString		update;  //업데이트시각
}CJCODESTR;

class CInterestPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CInterestPanel)

public:
	CInterestPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInterestPanel();
	CMap <CString, LPCTSTR, CMap<CString,LPCTSTR, CCodeData, CCodeData>*,  CMap<CString,LPCTSTR, CCodeData, CCodeData>*>  m_mapGroupCodemap;
	CArray<CJCODE*, CJCODE*> m_arrayCJCode;
	CStringArray  m_arrGroup, m_arrTrGrp;
	CMapStringToString m_mapcode, m_mapGroupck;
	BOOL m_bFirst;
	bool m_bSearching;
	int	 m_iGroupSelect;
	CString m_oldGrm;
	CFont* m_pfont;
	CString m_strGnameInSave;
	CString m_selectgroup;

	CString GetGroupFromEdit();

	BOOL DirectoryExist(LPCTSTR pszDir);
	BOOL CheckGroupName(CString strGroup);
	BOOL LoadCJCode();

	bool addcodeCheck(CString& bgroup);

	void RemoveGroupInArr(CString strgroup);
	void AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strFlag, CString strClr);
	void AddListOfCodeArray(int nIdx, CString sCode, CString sName, CString strFlag, CString strClr);
	void LoadFile(CString strgroup = _T(""));
	void RemoveSpChar(CString &strValue);
	void AddGroup(CString strGroup = _T(""));
	void ClearGroupList();
	void LoadGroupOfFile(int nSavePos = 0);	
	void SaveFile(int nSavePos = 0);
	void Init();
	void Initbtn();
	void addlistGroup(CString strgroup);
	void addCode(CString strcode);
	void DeleteItem();
	void RemovePrevName(CString& strName);
	void ReflashSelectList(int nItem);
	void ReflashSelectList(CString strgroup = _T(""));
	void FillCodelistByGroup(CString strgroup);
	bool CheckCodeInMap(CString strgroup,CString strcode);
	void RemoveCodeInGroup(CString strgroup, CString strcode);
	void ResetGrouplistNo();
	void ResetCodelistNo();
	bool sendTR(CString trC, char* pBytes, int nBytes, int gubn);

	void applyArrToGlist(CListCtrl& list, CStringArray& arr, CStringArray& chkarr);
	void applyArrToClist(CListCtrl& list, CStringArray& arr, CStringArray& chkarr);
	void GetledgerH(struct _ledgerH* pLedgerH, int trGubn);
	
	bool CheckGpName(CString strgroup, int gubn = 0);
	void EnableBtn(bool bEnable);

	void SearchInterList();             //관심그룹리스트조회
	void oubSearchList(char* pdata);   //관심그룹 리스트 조회결과

	void DeleteAllInterList();     //다수의 관심그룹 삭제
	void oubRemoveAllgroup(char* pdata);  //다수의 관심그룹 삭제 결과

	void InterListTR(int gubn, CString oldname, CString newname);  //관심 그룹명 추가변경 삭제
	void oubInterList(char* pdata);     //관심그룹 리스트 저장 삭제 등등 결과

	void InterListOrderSave(int gubn);	  //관심그룹 순서 저장
	void oubInterListOrder(char* pdata); //관심그룹 리스트 순서 저장결과

	void InterListCodeTR(int gubn, CString strgroup);  //관심그룹  종목 저장 삭제 조회
	void oubInterListCode(char* pdata); //관심그룹 종목 조회 저장 삭제 등등 결과

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INTEREST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listGroup;
	CListCtrl m_listCode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedBtnAddgroup();
	afx_msg void OnItemchangedListCode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedListGroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListCode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnGremove();
	afx_msg void OnBnClickedBtnCremove();
	afx_msg void OnBnClickedClose();
	afx_msg void OnClickListGroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	CNButton m_btnAddGroup;
	CNButton m_btnCDown;
	CNButton m_btnCRemove;
	CNButton m_btnCUp;
	CNButton m_btnGDown;
	CNButton m_btnGRemove;
	CNButton m_btnGroup;
	CNButton m_btnGUp;
	CNButton m_btnSearch;
	CNButton m_btnSave;
	CNButton m_btnCancel;
	CNButton m_btnAllLoad;
	CNButton m_btnAllSave;

	CPen* m_pPenblue, *m_pPenblack;

	CBrush  *m_pbrush, *m_pbrushbottom;
	CPanel  *m_pPanelleft, *m_pPanelright;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnGroup();
	afx_msg void OnDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult);

	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnCup();
	afx_msg void OnBnClickedBtnCdown();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedAllSave();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CGpEdit m_gpedit;
};
