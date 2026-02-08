// BmpFileMaker.h: interface for the CBmpFileMaker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPFILEMAKER_H__05369A6B_94C5_4EE7_995D_024AC4483DED__INCLUDED_)
#define AFX_BMPFILEMAKER_H__05369A6B_94C5_4EE7_995D_024AC4483DED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin/I000000/_IChartOCX.h"	// for IChartOCX

class CBmpFileMaker  
{
public:
	CBmpFileMaker::CBmpFileMaker( HWND p_hOcxWnd)	{	m_hOcxWnd = p_hOcxWnd;	}

public:
	void MakeJpgFile(const int nWidth,
								const int nHeight,
								CString& strFileName,
								IChartOCX *p_pIChartOCX);
	void MakeBmpFile(const int nWidth,
					 const int nHeight,
					 const CString& strFileName,
					 IChartOCX *p_pIChartOCX);

private:
	void WriteToFile(const CDC& memoryDC,
					 CBitmap& bitmap,
					 const CString& strFileName,
					 IChartOCX *p_pIChartOCX);

	bool WriteDIB( LPTSTR szFile, HANDLE hDIB);
	HANDLE DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal,
					IChartOCX *p_pIChartOCX);

	void LoadBMP(const CString fileName, UINT nWidth, UINT nHeight, BYTE *pBitmapBuffer);

	// (2006/3/28 - Seung-Won, Bae) Caution! pBitmapBuffer must be free. (delete [] pBitmapBuffer;)
	void SaveBMP24(const CString fileName,
				   const UINT nWidth,
				   const UINT nHeight,
				   BYTE *pBitmapBuffer,
				   IChartOCX *p_pIChartOCX);

	void LoadJPG(const CString fileName, UINT nWidth, UINT nHeight, BYTE *pBitmapBuffer);
	void SaveJPG(const CString fileName, 
				 const int nWidth,
				 const int nHeight, 
				 BYTE *pBitmapBuffer, 
				 BOOL color,
				 IChartOCX *p_pIChartOCX);

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND	m_hOcxWnd;
};

#endif // !defined(AFX_BMPFILEMAKER_H__05369A6B_94C5_4EE7_995D_024AC4483DED__INCLUDED_)
