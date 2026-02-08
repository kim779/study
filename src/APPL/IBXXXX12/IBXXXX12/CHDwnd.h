#pragma once
#include <afxwin.h>

class CHDwnd : public CWnd
{
public:
    CHDwnd();
    virtual ~CHDwnd();
    HWND	 m_hMain{};  //axis mainframe의 핸들

    struct st_RequestHwnd
    {
        HWND hwnd;
        int key;
        char* pMid;
        BOOL bComplete;
    };
    CArray < st_RequestHwnd*, st_RequestHwnd*> m_hwndArr;

    CString m_slog;

    CWnd* m_pParent{};
    
    BOOL CreateMap(CWnd* pParent);
    CString Parser(CString& srcstr, CString substr);
    int        KeyTran(int ikey);                                               //조회결과를 요청화면에 보내줄때 메인통해서 보내주기 때문에 약속된 키로 변환 필요
    void     CheckMainMsg();
    void     sendTR(CString trCode, char* datB, int datL, int key);

    void	    Request_GroupList();											//관심그룹 조회
    void     Request_GroupCode(char* pdata);           //관심그룹내부 종목코드 조회
    void     Request_uploadEachGroup(int gno);


    void    DB_uploadBackup();                                              //DB 백업 TR  (관심설정 오픈시 호출)
    void    DB_NewGroupAppend(char* pdata);                     //새그룹등록
    void    DB_GroupCodeSave(char* pdata);                         //그룹종목 등록
public:
    LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
    LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

