// XLSFile.h: interface for the CDrXLSFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRXLSFILE_H__5598116C_144A_4401_AD77_AE80FD6A5206__INCLUDED_)
#define AFX_DRXLSFILE_H__5598116C_144A_4401_AD77_AE80FD6A5206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define EXCEL_CELL_NUMBER			0x03
#define EXCEL_CELL_LABEL			0x04

#define EXCEL_SHADING				0x80

#define EXCEL_BORDER_BOTTOM			0x40
#define EXCEL_BORDER_TOP			0x20
#define EXCEL_BORDER_RIGHT			0x10
#define EXCEL_BORDER_LEFT			0x08

//alignment
#define EXCEL_ALIGN_GENERAL			0x00
#define EXCEL_ALIGN_LEFT 			0x01
#define EXCEL_ALIGN_CENTER 			0x02
#define EXCEL_ALIGN_RIGHT 			0x03
#define EXCEL_ALIGN_FILL 			0x04
#define EXCEL_ALIGN_MULTIPLAN 		0x07

#define EXCEL_DIMENSIONS_BIFF5		0x0200
#define EXCEL_BOF_BIFF5				0x0809
#define EXCEL_INDIVIDUAL_VERSION	0x0200
#define EXCEL_WORKSHEET				0x10
#define EXCEL_EOF					0xa

class CDrXLSFile  
{
public:
	BYTE GetBorder() const {return m_bBorder;}
	BYTE GetTextAlign() const {return m_bTextAlign;}
	
	void SetBorder(BYTE bBorder);
	void SetTextAlign(BYTE bTextAlign);

	UINT GetSizeOf(LPCSTR lpszVal);
	void Write(UINT iRow, UINT iCol, LPCSTR lpszText);
	void Write(UINT iRow, UINT iCol, LPCSTR lpszText, bool bNumber);
	void AddColRowInfo(UINT iRow, UINT iCol);
	void AddAttributeInfo();
	void WriteRecordHeader(WORD FRecordType, WORD FDataLength);
	void WriteNumber(int iRow, int iCol, double dValue);
	void WriteLabel(UINT iRow, UINT iCol, LPCSTR lpszText);
	BOOL OpenFile(LPCSTR lpszFilePath);
	void CloseFile();
	CDrXLSFile();
	virtual ~CDrXLSFile();

protected:
	FILE *fp;

private:
	BYTE	m_bTextAlign;
	BYTE	m_bBorder;
};

#endif // !defined(AFX_XLSFILE_H__5598116C_144A_4401_AD77_AE80FD6A5206__INCLUDED_)
