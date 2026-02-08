#pragma once
// Component.h : header file
//


struct _hjcode
{
	char code[13]{};
	char name[17]{};
	char etc[45]{};
};


/////////////////////////////////////////////////////////////////////////////
// CComponent command target

class CComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CComponent)

	CComponent();           // protected constructor used by dynamic creation
	virtual ~CComponent();

// Attributes
public:
	CString		m_sRoot, m_sUser;
// Operations
public:
	CString Parser(CString &srcstr, CString substr);
	CString loadCdNNmList(CString sGroup);
	CString getMidString(char *pStr, int nFirst, int nLen);
	CString	loadCodeList(CString sGroup);
	CString	loadGroupNames();
	int	getGroupCnt();
	CString	getPrvGroupCode(CString GroupCode);
	CString	getNextGroupCode(CString GroupCode);
	CString	loadGroupList(CString GroupCode, int nCount);
	CString getGroupOrder();
	int	getGroupCodeIdx(CString GroupCode);
	CString	getIdxGroupCode(CString sIndex) ;
	boolean addItemCd(CString GroupCode, CString ItemCd);
	CString getCodeNm(CString ItemCd);
	BOOL	IsCodeInGroup(CString sGroup, CString Code);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponent)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
//protected:

	// Generated message map functions
	//{{AFX_MSG(CComponent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CComponent)
	afx_msg BSTR LoadGroupNames();					// Group명 가져오기
	afx_msg void SetUserName(LPCTSTR sUser);			// 관심그룹 정보를 가져올 User명 설정
	afx_msg BSTR LoadCodeList(LPCTSTR sGroup);			// 관심그룹의 종목코드 가져오기
	afx_msg BSTR LoadCdNNmList(LPCTSTR sGroup);			// 관심그룹의 종목코드와 종목명 가져오기
	afx_msg BSTR LoadGroupList(LPCTSTR GroupCode, long nCount);	// 특정관심그룹(GroupCode) 이후의 갯수만큼의 관심그룹명 가져오기
	afx_msg long GetGroupCnt();					// 관심그룹 개수 반환
	afx_msg BSTR GetNextGroupCode(LPCTSTR nGroupCode);		// 지정 관심그룹코드의 다음 관심그룹코드 가져오기
	afx_msg BSTR GetPrvGroupCode(LPCTSTR GroupCode);		// 지정 관심그룹코드의 이전 관심그룹코드 가져오기. 
										// - ""값을 입력하면 첫번째 관심그룹코드를 반환한다
	afx_msg long GetGroupCodeIdx(LPCTSTR GroupCode);		// 그룹코드의 순서번호 가져오기
	afx_msg BSTR GetIdxGroupCode(LPCTSTR sIndex);			// 순서번호에 있는 그룹코드 가져오기
	afx_msg BOOL AddItemCd(LPCTSTR GroupCode, LPCTSTR ItemCd);	// 관심그룹에 종목코드 추가하기
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
