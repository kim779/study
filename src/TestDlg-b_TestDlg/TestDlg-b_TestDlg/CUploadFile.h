#pragma once

#include "pch.h"


#define DF_FILESIZE (1024 * 10)
//#define DF_FILESIZE (1024 )
#define DF_FIRST	'F'
#define DF_MIDDLE	'M'
#define DF_LAST		'L'

struct st_mid_PMHOUPLD
{
	char	flow;
	char	name[32];
	char	dlen[5];
	char	data[14000];  //DF_FILESIZE
};
#define	L_MID	sizeof(struct st_mid_PMHOUPLD)
#define	L_xMID	sizeof(struct st_mid_PMHOUPLD) - 14000	// - data size

class CUploadFile
{
public:
	CUploadFile();
	virtual ~CUploadFile();
	CWnd* m_pWizard;
//	_param  m_param;

	CFile	m_file;
	int	m_offset;
	int	m_size;

	CString m_strFileName;
	CString m_filename;

	BOOL upload(LPCTSTR sPath);
	int	  sendData();
	int sendTR(CString strName, BYTE type, char* datb, int datl);
};

