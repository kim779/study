/**
	@file		ProgDir.h
	@brief		폴더 구조
	@author		sashia
	@date		2009/08/17
*/

#if !defined(AFX_PROGDIR_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_PROGDIR_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
	@class		CProgDir
	@brief		폴더 구조
	@author		sashia
	@date		2009/08/13
*/
class CProgDir
{
public:
	CProgDir();
	virtual ~CProgDir();

	static	CString		m_strPathRoot;	//	root Path.
	static	CString		m_strPathImg;
	static	CString		m_strPathBin;
	static	CString		m_strPathData;

public:
	static	void		MakeDir();

	static	void		SetRootPath(CString strPath)	{ m_strPathRoot = strPath;	};
	static	void		SetImgPath(CString strPath)		{ m_strPathImg = strPath;	};
	static	void		SetBinPath(CString strPath)		{ m_strPathBin = strPath;	};
	static	void		SetDataPath(CString strPath)	{ m_strPathData = strPath;	};

	static	CString		GetRootPath()	{ return m_strPathRoot;	};
	static	CString		GetImgPath()	{ return m_strPathImg;	};
	static	CString		GetBinPath()	{ return m_strPathBin;	};
	static	CString		GetDataPath()	{ return m_strPathData;	};
};

#endif // !defined(AFX_PROGDIR_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
