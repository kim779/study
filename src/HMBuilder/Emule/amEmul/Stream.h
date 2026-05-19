// Stream.h: interface for the CStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAM_H__2B203507_DEB9_47AB_AA2E_E9EC58B94E76__INCLUDED_)
#define AFX_STREAM_H__2B203507_DEB9_47AB_AA2E_E9EC58B94E76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../h/axis.h"
#include "../dll/form/ambase.h"

#define	FS		0x7f
#define	iFS		'='
#define	uFS		'@'
#define	gFS		0x1b
#define	L_FILEDs		5
#define	L_FLEX		4
#define	maxIOs		64*1024



class CStream  
{
public:
	CStream(class CClient* client);
	virtual ~CStream();

// Attributes
public:
	int	m_sndL;
	char	m_sndB[L_axisH+maxIOs];
	bool	m_lock;
	
protected:
	class CGuard*	m_guard;
	class CClient*	m_client;
	
	deque<string>				m_Deque;
	map<string, class CScreen*>		m_MScreen;
	//map<string, string>			m_MLEN;
	//map<string, CamBase*>			m_MTR;
	//vector<string>				m_vINFO;
	//vector<string>				m_vOUT;
	//vector<int>				m_vGridLen;

// Operations
public:
	bool	clear();
	bool	DataClear(CString sTrname);
	bool	InStream(bool byKey = false);
	bool	InStream(CString sTr, class CScreen* screen, bool byKey = false);
	void	OutStream(struct _axisH* axisH, char* datB, int datL);

	bool	SetGuide(CScreen* screen, CString guide, COLORREF rgb = -1);
	void	ParseFCC(CScreen* screen, int key, struct _FCC* fcc, int col = -1, int row = -1);

// Implementation
protected:
	void	GetDataH(CScreen* screen, char* sysB, int& sysL);
	int		GetDataNRM(CScreen* screen, char* iosB, bool tab, bool skip = false);
	int		GetDataOOP(CScreen* screen, char* iosB, bool skip = false);
	int		GetDataOOP2(CScreen* screen, char* iosB, bool skip = false);
	void	MakeStream(bool byKey = false);
	void	MakeStream(CString sTr, CScreen* screen);

	bool	GetScreen(class CScreen*& screen, struct _axisH* axisH);
	BOOL	GetScreen(class CScreen*& screen, struct _axisH* axisH, int& index);
	int		SetDataNRM(CScreen* screen, char* axisB, int axisL, bool skip = false);
	int		SetDataOOP(CScreen* screen, char* axisB, int axisL, bool skip = false);
	int		SetDataTAB(CScreen* screen, char* axisB, int axisL, bool skip = false);

	bool	SetDataH(CScreen* screen, char* datB, int& datH);

	void	SetCells(CScreen* screen, CamBase* form, int nRows, int nCols, char* axisB, int& idx, int axisL);
	void	SetCells(CScreen* screen, CamBase* form, CString& text);
	void	SetTable(CScreen* screen, int key, int nCols, int nRows, char* datB, int datL, int& idx);
	bool	ParseCC(CScreen*& screen, char* datB, int& key, int& idx);
	bool	FormCC(CScreen*& screen, int& key);
	void	SetCC(CamBase* form, int cc, bool set, int col = -1, int row = -1);
	void	ParseSCC(CScreen* screen, CamBase* form, struct _SCC* scc, int col = -1, int row = -1);
	int		ParseRCC(CScreen* screen, char* datB);
};

#endif // !defined(AFX_STREAM_H__2B203507_DEB9_47AB_AA2E_E9EC58B94E76__INCLUDED_)
