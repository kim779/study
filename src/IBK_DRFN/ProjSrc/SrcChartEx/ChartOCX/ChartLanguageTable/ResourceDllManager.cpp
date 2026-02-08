// ResourceDllManager.cpp: implementation of the CResourceDllManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLanguageTable.h"
#include "ResourceDllManager.h"

#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char *CResourceDllManager::m_szLanguageIDs[ IMetaTable::LANGUAGE_ID_END] =
{
	"ENG",
	"KOR",
};

CResourceDllManager::CResourceDllManager()
{

}

CResourceDllManager::~CResourceDllManager()
{
}

HMODULE CResourceDllManager::LoadResourceDLL( HMODULE p_hMainModule, IMetaTable::E_LANGUAGEID p_eLanguageID)
{
	assert( p_hMainModule);
	
	char szModuleFilePath[ _MAX_PATH + 1];
	if( !GetModuleFileName( p_hMainModule, szModuleFilePath, _MAX_PATH)) return p_hMainModule;

	ResourceModules *pResModules = NULL;
	if( !m_mapResModules.Lookup( szModuleFilePath, pResModules))
	{
		pResModules = ( ResourceModules *)new ResourceModules;
		m_mapResModules.SetAt( szModuleFilePath, pResModules);
		for( int i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++) ( *pResModules)[ i] = NULL;
	}

	if( ( *pResModules)[ p_eLanguageID]) return ( *pResModules)[ p_eLanguageID];

	HRSRC hResource = FindResource( p_hMainModule, m_szLanguageIDs[ p_eLanguageID], "LNG_RES");
	if( !hResource) return p_hMainModule;

	HGLOBAL hResModule = LoadResource( p_hMainModule, hResource);
	if( !hResModule) return p_hMainModule;

	VOID *pResModule = LockResource( hResModule);
	if( !pResModule) return p_hMainModule;

	char szTempPath[ _MAX_PATH + 1];
	if( !GetTempPath( _MAX_PATH, szTempPath))
	{
		UnlockResource( hResModule);
		return p_hMainModule;
	}

	char szResFileName[ _MAX_PATH + 1];
	if( !GetTempFileName(	szTempPath,			// dir. for temp. files 
							"CHT",				// temp. file name prefix 
							0,					// create unique name 
							szResFileName))		// buffer for name 
	{
		DWORD dwError = GetLastError();
		UnlockResource( hResModule);
		return p_hMainModule;
	}

	HANDLE hResFile = CreateFile(	szResFileName,			// file name 
									GENERIC_WRITE,			// open for read/write 
									0,						// do not share 
									NULL,					// no security 
									CREATE_ALWAYS,			// overwrite existing file
									FILE_ATTRIBUTE_NORMAL,	// normal file 
									NULL);					// no attr. template 
	if( INVALID_HANDLE_VALUE == hResFile)
	{
		UnlockResource( hResModule);
		return p_hMainModule;
	}

	DWORD dwWrited = 0;
	WriteFile( hResFile, pResModule, SizeofResource( p_hMainModule, hResource), &dwWrited, NULL);
	CloseHandle( hResFile);
	UnlockResource( hResModule);

	( *pResModules)[ p_eLanguageID] = LoadLibrary( szResFileName);
	return ( *pResModules)[ p_eLanguageID];
}

void CResourceDllManager::UnloadResourceDLL( void)
{
	int i = 0;
	CString strModulePath;
	char szModuleFilePath[ _MAX_PATH + 1];
	ResourceModules *pResModules = NULL;
	POSITION psnResModules = m_mapResModules.GetStartPosition();
	while( psnResModules)
	{
		m_mapResModules.GetNextAssoc( psnResModules, strModulePath, pResModules);
		for( i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++) if( _DEFAULT_RESOURCE_ID != i && ( *pResModules)[ i])
		{
			if( !GetModuleFileName( ( *pResModules)[ i], szModuleFilePath, _MAX_PATH))
				szModuleFilePath[ 0] = '\0';
			FreeLibrary( ( *pResModules)[ i]);
			if( szModuleFilePath[ 0]) DeleteFile( szModuleFilePath);
		}
		delete [] pResModules;
	}
	m_mapResModules.RemoveAll();
}