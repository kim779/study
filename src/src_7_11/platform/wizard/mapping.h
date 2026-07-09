// CMapping.h : header file
//

#if !defined(AXIS_MAPPING_H__INCLUDED_)
#define AXIS_MAPPING_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
#include <afxmt.h>

const int maxQUE = 1024 * 5;
const int maxBUF = 512 * 2;
const int actBUF = 8;
const UINT WM_MAPDATA = WM_USER+501;
const UINT WM_CMDDATA = WM_USER+502;

enum fnCommand { axAttach = 1, axDetach, axFilter, axReload, axExit, axInterval };

enum TOmsg { toWizard = 0, toCast = 1 };

struct	_shm
{
	int	attach;				// attach count
	HWND	hwnd[2];			// 0 : wizard, 1:socket
	int	widx;				// write index
	int	ridx;				// read index
	char	actF[actBUF+1];			// action flag
	char	actB[maxBUF+1];			// action buffer
	struct	_mque {
		char	m_code[12+1];		// code
		int	m_nMbols;		// symbol count
		WORD	m_pMbols[maxBUF/2+1];	// symbol buffer
		int	m_nBytes;		// data length
		char	m_pBytes[maxBUF];	// data buffer
		bool	m_trade;		// trade
	} mque[maxQUE+1];
};

class CMapping
{
// Construction
public:
	CMapping() { m_handle = NULL; m_shm = NULL; }
	virtual ~CMapping() {}

public:
	HANDLE		m_handle;
	struct	_shm*	m_shm;
//	CEvent		m_event;

protected:
	CCriticalSection	m_sync;

// updateX_JDH
public:
	int		m_nFuture;
	int		m_nOption;
	int		m_nTotal;

public:
	bool	initMapping();
	void	endMapping();

	void	setHwnd(int idx, HWND hwnd);
	LRESULT	sendMsg(TOmsg to, WPARAM wp = 0, LPARAM lp = 0);
	int	readCount();
	bool	writeData(CString code, int symC, WORD* symB, CString datB, bool trade, bool notify = true);
	bool	readData(CString& code, CWordArray* rtmk, char*& datB, bool& trade);

	int	runProcess(CString home, CString pnam);
	void	killProcess();

	void	sendMsg();
};

#endif // !defined(AXIS_MAPPING_H__INCLUDED_)
