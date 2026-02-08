#if !defined(AFX_DISP_VIEW__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
#define AFX_DISP_VIEW__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CDispView wrapper class

class CDispView : public COleDispatchDriver
{
public:
	CDispView();

//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable
//		IIx_Rows=0,

		// Method
		IIx_ShowDialog=0,
		IIx_MsgBoxID,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);
//}} Dispid자동처리
	
public:
// Attributes

	
// Operations
	short ShowDialog(LPCTSTR szKey, short nIndex) ;
	short MsgBoxID(short nID, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags) ;
};

#endif // !defined(AFX_DISP_VIEW__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
