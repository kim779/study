// DumpLogger.h: interface for the CDumpLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUMPLOGGER_H__F88F14FB_C314_4FA9_8479_D0240992B75D__INCLUDED_)
#define AFX_DUMPLOGGER_H__F88F14FB_C314_4FA9_8479_D0240992B75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.07
//
//	목적	:	이전의 Memory Dump Logging Library를 Class Header Library로 개선한다.
///////////////////////////////////////////////////////////////////////////////
class CDumpLogger : public CObject  
{
public:
	CDumpLogger( const char *p_szLogFileKey = "MDump", BOOL p_bMSecLogFile = FALSE, const char *p_szLogFolderPath = "C:\\Logs")
	{
		// 0. Parameter 값을 조정하기 위하여, 보조 Variable을 이용한다.
		const char *szLogFileKey	= p_szLogFileKey;
		const char *szLogFolderPath	= p_szLogFolderPath;
		// 1. 먼저 Logging하는 시각을 확인한다.
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);
		// 2. Log File Name을 생성한다.
		// 2.1 Log Folder Path를 확인한다.
		if( szLogFolderPath == NULL || *szLogFolderPath == '\0') szLogFolderPath = "C:\\Logs";
		// 2.2 Log File Key를 확인한다.
		if( szLogFileKey == NULL || *szLogFileKey == '\0') szLogFileKey = "MDump";
		// 2.3 Log File Name을 구성한다.
		CString strFileName;
		if( p_bMSecLogFile) strFileName.Format( "%s\\%s_%04d%02d%02d_%02d%02d%02d_%03d.log", szLogFolderPath, szLogFileKey, sTime.wYear, 
			sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond, sTime.wMilliseconds);
		else strFileName.Format( "%s\\%s_%04d%02d%02d.log", szLogFolderPath, szLogFileKey, sTime.wYear, 
			sTime.wMonth, sTime.wDay);
		// 3. szLogFolderPath를 관리할 필요가 없도록 미리 처리한다.
		::CreateDirectory( szLogFolderPath, NULL);

		// 3. Log File을 Open한다.
		m_hLogFile = _lopen( ( LPCTSTR)strFileName, OF_READWRITE);
		if( m_hLogFile != HFILE_ERROR)
		{
			// 3.1 이전 File이 있으면 File 끝으로 이동한다.
			if( _llseek( m_hLogFile, 0L, FILE_END) == HFILE_ERROR) return;
		}
		// 3.2 이전 File이 없으면 새로이 생성한다.
		else m_hLogFile = _lcreat( ( LPCTSTR)strFileName, 0);
	}
	virtual ~CDumpLogger()
	{
		// 6. Dump File을 닫는다.
		if( m_hLogFile != HFILE_ERROR) _lclose( m_hLogFile);
	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.07
//
//	목적	:	기본적인 Log File Key와 LogFolderPath를 지정받아 LogFileName을 관리한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	HFILE		m_hLogFile;			// Log File Handle

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.07
//
//	목적	:	설정된 Logging File에 Dump Log를 기록하는 Interface를 제공한다.
//
//	주의	:	1. 다른 Logger와 File을 공유할 경우를 감안하여, Constructor에서 Open하여 그 상태를 유지하지 않고,
//					Log Write시마다 즉시 열어서 처리한다.
//
//	수정	:	(2003.07.08) 16 Byte Line Dump 처리중, 마지막 Character가 Multi-Byte Character일 경우,
//					다음 Character와의 조합을 감안하여 ' '(Space)를 하나 더 추가시칸다.
///////////////////////////////////////////////////////////////////////////////
public:
	void WriteToDumpLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
	{
		if( m_hLogFile == HFILE_ERROR) return;

		// 1. 먼저 Logging하는 시각을 확인한다.
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);

		// 4. 주어진 Section Value을 확인하여 Section String을 구성한다.
		CString strBuffer;
		if( p_szSection && *p_szSection != '\0')
		{
			CString strCurTime;
			strCurTime.Format("\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
				sTime.wMilliseconds);
			strBuffer = strCurTime + p_szSection + "]\r\n";
			_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
		}

		int i = 0;
		// 5. Memory를 Dump한다.
		for(  i = 0; i < p_nDataLen; i++)
		{
			// 5.1 기본 Byte를 Dump한다.
			strBuffer.Format( "%02x ", ( unsigned char)p_szData[ i]);
			_hwrite( m_hLogFile, ( LPCSTR)strBuffer, 3);

			// 5.2 16자리를 Dump한 뒤, 실 Data를 기록한다.
			if( i % 16 == 15) 
			{
				_hwrite( m_hLogFile, "\t\t", 2);
				_hwrite( m_hLogFile, p_szData + i - 15, 16);
			
				// (2003.07.08) 16 Byte Line Dump 처리중, 마지막 Character가 Multi-Byte Character일 경우,
				//		다음 Character와의 조합을 감안하여 ' '(Space)를 하나 더 추가시칸다.
				if( p_szData[ i] & 0x80) _hwrite( m_hLogFile, " ", 1);

				_hwrite( m_hLogFile, "\r\n", 2);
			}
		}
		// 5.3 마지막 Byte Dump에 대한 실 Data를 기록한다.
		if( i % 16 != 0) 
		{
			_hwrite( m_hLogFile, "\t\t", 2);
			_hwrite( m_hLogFile, p_szData + i - i % 16, p_nDataLen - ( i - i % 16));

			// (2003.07.08) 16 Byte Line Dump 처리중, 마지막 Character가 Multi-Byte Character일 경우,
			//		다음 Character와의 조합을 감안하여 ' '(Space)를 하나 더 추가시칸다.
			if( p_szData[ p_nDataLen - 1] & 0x80) _hwrite( m_hLogFile, " ", 1);

			_hwrite( m_hLogFile, "\r\n", 2);
		}
	}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.04.06
//
//	목적	:	Dump가 아니라 일반 String Logging 기능을 제공한다.
///////////////////////////////////////////////////////////////////////////////
public:
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
	{
		if( m_hLogFile == HFILE_ERROR) return;

		// 1. 먼저 Logging하는 시각을 확인한다.
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);

		// 4. 주어진 Section Value을 확인하여 Section String을 구성한다.
		CString strBuffer;
		if( p_szSection && *p_szSection != '\0')
		{
			CString strCurTime;
			// (2005/9/1 - Seung-Won, Bae) Line Skip for Only Section.
			strCurTime.Format("\r\n\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
				sTime.wMilliseconds);
			strBuffer = strCurTime + p_szSection + "]\r\n";
			_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
		}
		// (2005/9/2 - Seung-Won, Bae) in No Section Log, Insert Time in front of Line
		else
		{
			CString strCurTime;
			strCurTime.Format("%02d:%02d:%02d:%03d\t", sTime.wHour, sTime.wMinute, sTime.wSecond, 
				sTime.wMilliseconds);
			_hwrite( m_hLogFile, strCurTime, strCurTime.GetLength());
		}

		// 5. Memory를 Dump한다.
		_hwrite( m_hLogFile, p_szData, p_nDataLen);
	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.07
//
//	목적	:	이전의 Memory Dump Logging Routine을 Static Member로 제공한다.
//
//	내용	:	1. 내부적으로 CDumpLogger Object를 생성하여 처리토록한다.
//
//	수정	:	(2004.04.06, 배승원) Dump가 아니라 일반 String Logging 기능도 제공한다.
///////////////////////////////////////////////////////////////////////////////
public:
	static void DumpMemoryToFile( const char *p_szSection, const char *p_szData, int p_nDataLen, const char *p_szLogFileKey = "MDump", const char *p_szLogFolderPath = "C:\\Logs")
	{
		CDumpLogger dLogger( p_szLogFileKey, FALSE, p_szLogFolderPath);
		dLogger.WriteToDumpLog( p_szSection, p_szData, p_nDataLen);
	}
	static void LogStringToFile( const char *p_szSection, const char *p_szData, int p_nDataLen, const char *p_szLogFileKey = "MDump", const char *p_szLogFolderPath = "C:\\Logs")
	{
		CDumpLogger dLogger( p_szLogFileKey, FALSE, p_szLogFolderPath);
		dLogger.WriteToStringLog( p_szSection, p_szData, p_nDataLen);
	}
	static void LogStringToFile( const char *p_szSection, const char *p_szData, const char *p_szLogFileKey = "MDump", const char *p_szLogFolderPath = "C:\\Logs")
	{
		if( !p_szData) return;
		CDumpLogger dLogger( p_szLogFileKey, FALSE, p_szLogFolderPath);
		dLogger.WriteToStringLog( p_szSection, p_szData, strlen( p_szData));
	}
	static void LogTraceLineToFile( const char *p_szCodeFileName, int p_nCodeLine, const char *p_szLogFolderPath = "C:\\Logs")
	{
		if( !p_szCodeFileName) return;
		CString strLog;
		strLog.Format( "TRACE - %s[%d]\r\n", p_szCodeFileName, p_nCodeLine);
		CDumpLogger dLogger( "CodeTrace", FALSE, p_szLogFolderPath);
		dLogger.WriteToStringLog( NULL, strLog, strLog.GetLength());
	}
};

// (2007/3/19 - Seung-Won, Bae) Exception Log Macro
#define _DUMPLOGGER_EXCEPTION_LOG_DECLARATION	extern CString			g_strFunctionName;												\
												extern CString			g_strFileName;													\
												extern int				g_nLineNumber;													\
												extern CString			g_strLog;														\
												extern CDumpLogger		g_dLogger;														\
												extern CDumpLogger *	g_pLogger
#define _DUMPLOGGER_EXCEPTION_LOG_DEFINE		CString					g_strFunctionName;												\
												CString					g_strFileName;													\
												int						g_nLineNumber;													\
												CString					g_strLog;														\
												CDumpLogger g_dLogger( "CodeTrace", FALSE, "C:\\Logs");									\
												CDumpLogger *g_pLogger = NULL
#define _DUMPLOGGER_EXCEPTION_LOG_START( szFunctionName)																				\
												g_strFunctionName = szFunctionName;														\
												__try																					\
												{
#define _DUMPLOGGER_EXCEPTION_LOG_CHECK( szFileName, nLineNumber)																		\
													g_strFileName = szFileName;															\
													g_nLineNumber = nLineNumber;
#define _DUMPLOGGER_EXCEPTION_LOG_END			}																						\
												__except( EXCEPTION_EXECUTE_HANDLER)													\
												{																						\
													g_strLog.Format( "FileName : %s\r\nFunction Name: %s\r\nLine Number : %d\r\n",		\
														g_strFileName, g_strFunctionName, g_nLineNumber);								\
													g_dLogger.WriteToStringLog( "Chart Exception Log", g_strLog, g_strLog.GetLength());	\
													AfxMessageBox( "Unexpected error!\r\nSee code-trace log, please.");					\
													g_pLogger->WriteToStringLog( NULL, NULL, 0);										\
												}


#endif // !defined(AFX_DUMPLOGGER_H__F88F14FB_C314_4FA9_8479_D0240992B75D__INCLUDED_)
