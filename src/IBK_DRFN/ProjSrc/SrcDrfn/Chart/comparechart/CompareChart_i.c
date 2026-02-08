

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CompareChart.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_CompareChart,0x932D4874,0x2B3C,0x4A37,0xBA,0x46,0x0E,0x78,0x55,0x2D,0xDC,0xC1);


MIDL_DEFINE_GUID(IID, DIID_IWndMapView,0x340A11D9,0xAAE9,0x4C33,0xAD,0x56,0x6E,0x7E,0xBD,0x48,0x60,0x31);


MIDL_DEFINE_GUID(CLSID, CLSID_WndMapView,0xDAE59411,0xEF4A,0x49C3,0x9B,0x86,0xAD,0x3A,0x2B,0x5A,0xA1,0x0F);


MIDL_DEFINE_GUID(IID, DIID_IStdDialog,0x506C0A8C,0xCBC2,0x4d27,0xBD,0x67,0xD4,0x53,0x1A,0xB6,0xF5,0xA8);


MIDL_DEFINE_GUID(CLSID, CLSID_StdDialog,0x15FC7B68,0xFB3A,0x4b5d,0xA6,0x2D,0xF1,0xBB,0x96,0xF9,0x2D,0x27);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



