// DrXLSFile.cpp: implementation of the CDrXLSFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrXLSFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrXLSFile::CDrXLSFile()
{
	fp = NULL;

	m_bTextAlign	= 0;
	m_bBorder		= 0;
}

CDrXLSFile::~CDrXLSFile()
{

}

void CDrXLSFile::CloseFile()
{
	ASSERT(fp != NULL);
	WriteRecordHeader(EXCEL_EOF,0);
	fclose(fp);
	fp = NULL;
}

BOOL CDrXLSFile::OpenFile(LPCSTR lpszFilePath)
{
	if( fp != NULL ) 
		CloseFile();
	fp = fopen(lpszFilePath,"wb");

	if(fp == NULL)
		return FALSE;

	WORD Buffer[5];

	// *** File version & type ***

	Buffer[ 0 ] = EXCEL_INDIVIDUAL_VERSION;
	Buffer[ 1 ] = EXCEL_WORKSHEET;
	Buffer[ 2 ] = 0;
	Buffer[ 3 ] = 0;
	Buffer[ 4 ] = 0;

	WriteRecordHeader(EXCEL_BOF_BIFF5,6);
	fwrite( Buffer, 6, 1, fp);

	// *** Dimension record ***
	WORD FMinSaveRecs = 0;
	WORD FMaxSaveRecs = 65535;
	WORD FMinSaveCols = 0;
	WORD FMaxSaveCols = 256;

	Buffer[ 0 ] = FMinSaveRecs; // ** First row
	Buffer[ 1 ] = FMaxSaveRecs; // ** Last Row + 1
	Buffer[ 2 ] = FMinSaveCols; // ** First column
	Buffer[ 3 ] = FMaxSaveCols; // ** Last column + 1
	Buffer[ 4 ] = 0;

	WriteRecordHeader(EXCEL_DIMENSIONS_BIFF5,10);
	fwrite( Buffer, 10, 1, fp);

	return TRUE;
}

void CDrXLSFile::WriteLabel(UINT iRow, UINT iCol, LPCSTR lpszText)
{
	ASSERT(fp != NULL);
	char Buffer[255];
	byte  StringLength;
	
	if( strlen(lpszText) > 255 ){
		strncpy(Buffer,lpszText,GetSizeOf(Buffer));
		StringLength = GetSizeOf( Buffer );
	}
	else{
		strcpy(Buffer,lpszText);
		StringLength = GetSizeOf( lpszText );
	}
    
	WriteRecordHeader(EXCEL_CELL_LABEL, 8+StringLength);

     AddColRowInfo(iRow,iCol);		// 4 byte
     AddAttributeInfo();			// 3 byte

	 fwrite( &StringLength,  sizeof(char), 1, fp); // 1byte
	 fwrite( Buffer,  sizeof(char), StringLength, fp); 
}

void CDrXLSFile::WriteNumber(int iRow, int iCol, double dValue)
{	
	ASSERT(fp != NULL);
	WriteRecordHeader(EXCEL_CELL_NUMBER,15);
	AddColRowInfo(iRow,iCol);
	AddAttributeInfo();
	fwrite( &dValue,  8, 1, fp);
}

void CDrXLSFile::WriteRecordHeader(WORD FRecordType, WORD FDataLength)
{
	ASSERT(fp!=NULL);
	WORD Buffer[2];
	Buffer[0] = FRecordType;
	Buffer[1] = FDataLength;
	fwrite( Buffer,  sizeof(WORD), 2, fp);
}

void CDrXLSFile::AddColRowInfo(UINT iRow, UINT iCol)
{
	ASSERT(fp!=NULL);
	WORD Buffer[2];
	Buffer[ 0 ] = iRow;
	Buffer[ 1 ] = iCol;
	fwrite( Buffer,  sizeof(WORD), 2, fp);
}

void CDrXLSFile::AddAttributeInfo()
{
	ASSERT(fp!=NULL);
	byte Buffer[3];
	Buffer[ 0 ] = 0;
	Buffer[ 1 ] = 0;
	Buffer[ 2 ] = 0;

	Buffer[ 2 ] |= m_bTextAlign | m_bBorder;
	
	fwrite( Buffer,  sizeof(byte), 3, fp);
}

//Before Modify : 2003.04.02
//void CDrXLSFile::Write(UINT iRow, UINT iCol, LPCSTR lpszText)
//{
//	ASSERT(fp != NULL);
//	BOOL bNumber = true;
//	for(int i=0;i<255;i++){
//		if( lpszText[i] == '\0' ) break;
//		else{
//			if( !(lpszText[i] >= '0' && lpszText[i] <= '9') && !(lpszText[i] == 'e' && lpszText[i] == 'E') ){
//				bNumber = false;			
//				break;
//			}
//		}
//	}
//	if( bNumber )
//		WriteNumber(iRow,iCol,atof(lpszText));
//	else
//		WriteLabel(iRow,iCol,lpszText);
//}
//After Modify : 2003.04.02
void CDrXLSFile::Write(UINT iRow, UINT iCol, LPCSTR lpszText)
{
	ASSERT(fp != NULL);
	BOOL bNumber = true;
	for(int i=0;i<255;i++)
	{
		//////////////////////////////////////////////////////////////
		//* 날짜 : 2003.04.02, yjy 수정                               
		//* 수정 내용 : excel writing시 writing되는 data의 Sorting을
		//				 위해 '-'와 '.'를 Number로 writing되게 하였습니다.
		//////////////////////////////////////////////////////////////
		if( (lpszText[i] == '\0') 
			||(lpszText[i] == '-')
			||(lpszText[i] == '.') )
		{
			break;
		}
		else
		{
			if( !(lpszText[i] >= '0' && lpszText[i] <= '9') && !(lpszText[i] == 'e' && lpszText[i] == 'E') )
			{
				bNumber = false;			
				break;
			}
		}
	}
	if( bNumber )
		WriteNumber(iRow,iCol,atof(lpszText));
	else
		WriteLabel(iRow,iCol,lpszText);
}

void CDrXLSFile::Write(UINT iRow, UINT iCol, LPCSTR lpszText, bool bNumber)
{
	if( bNumber )
		WriteNumber(iRow,iCol,atof(lpszText));
	else
		WriteLabel(iRow,iCol,lpszText);
}

UINT CDrXLSFile::GetSizeOf(LPCSTR lpszVal)
{
	for(int i=0;i<255;i++){
		if( lpszVal[i] == '\0' ) return i;
	}
	return 255;
}

void CDrXLSFile::SetTextAlign(BYTE bTextAlign)
{
	m_bTextAlign = bTextAlign;
}

void CDrXLSFile::SetBorder(BYTE bBorder)
{
	m_bBorder = bBorder;
}
