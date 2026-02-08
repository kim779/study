#ifndef __SUIPRECOMMON_H_
#define __SUIPRECOMMON_H_


typedef int (*PFN_SUI_STATUS)(long lCode, DWORD dwErrorCode, void* pParam);

#define SUI_PREPARING_SUPDATEI		1
#define SUI_BEGIN_SUPDATEI			2
#define SUI_BEGIN_DOWNLOADINGFILE	5
#define SUI_DOWNLOADINGFILE			6
#define SUI_END						7


#endif // __SUIPRECOMMON_H_