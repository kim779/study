#ifndef	_LIBMAP_H
#define	_LIBMAP_H

#pragma once

__declspec(dllexport) CWnd*  WINAPI axCreate(CWnd *parent, void* pParam);
__declspec(dllexport) void  WINAPI axPreview(CDC *pDC, CWnd *parent, void* pParam);

#endif	// _LIBMAP_H