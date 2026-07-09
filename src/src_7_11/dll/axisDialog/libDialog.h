#ifndef _LIBDIALOG_H_
#define	_LIBDIALOG_H_

#ifdef	__cplusplus
extern "C" {
#endif

__declspec(dllexport) void* APIENTRY axDialog(int kind,  char * root, char* src, char* items, char* defs);

#ifdef	__cplusplus
}
#endif

#endif
