#ifndef _LIBLOGIN_H_
#define	_LIBLOGIN_H_

#ifdef	__cplusplus
extern "C" {
#endif

__declspec(dllexport) void*  APIENTRY axLogin(void* data);
__declspec(dllexport) void  APIENTRY axLoginLedger(void* ledger);
__declspec(dllexport) void* APIENTRY axLedger(void* data);

#ifdef	__cplusplus
}
#endif

#endif
