#ifndef	_LIBMAP_H
#define	_LIBMAP_H

#pragma once

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam);
__declspec(dllexport) char* WINAPI axGetMac(char* pdata);

#endif	// _LIBMAP_H