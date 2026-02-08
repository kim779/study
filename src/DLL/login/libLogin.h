#ifndef _LIBLOGIN_H_
#define	_LIBLOGIN_H_

#ifdef	__cplusplus
extern "C" {
#endif

__declspec(dllexport) void* APIENTRY axLogin(void* data);
__declspec(dllexport) void  APIENTRY axLoginLedger(void* ledger);

__declspec(dllexport) void* APIENTRY axLedger(void* data);
__declspec(dllexport) void  APIENTRY axLedgerEx(void* ledger, void* data);
__declspec(dllexport) void* APIENTRY axGetLedger(void* ledger, int pos, int length);
__declspec(dllexport) void  APIENTRY axSetLedger(void* ledger, int pos, int length, void* data);
__declspec(dllexport) void* APIENTRY axGetLedgerEx(void* ledger, int id);
__declspec(dllexport) void* APIENTRY axGetLedgerData(void* ledger, void* data, int id);
__declspec(dllexport) void* APIENTRY axLoginData(int id);

#ifdef	__cplusplus
}
#endif

#endif
