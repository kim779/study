

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_FundPrinter.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
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

MIDL_DEFINE_GUID(IID, LIBID_CX_FundPrinter,0x35CD300E,0x1E79,0x488B,0x99,0xD0,0xA3,0x83,0xB8,0xBD,0x0E,0xEE);


MIDL_DEFINE_GUID(IID, DIID_IPrinter,0x522CFD24,0x117A,0x4BEE,0xAB,0x78,0x3C,0xEC,0x2B,0x5C,0x0B,0xFB);


MIDL_DEFINE_GUID(CLSID, CLSID_Printer,0xD9FF8DC6,0xE3E8,0x4CDC,0xAA,0x76,0x50,0x09,0x94,0xC2,0x54,0xDD);


MIDL_DEFINE_GUID(IID, DIID_IFPrinter,0x5EBD1A3F,0xD1A8,0x4B36,0x99,0xBC,0xF0,0x76,0x4B,0xA7,0xE3,0x10);


MIDL_DEFINE_GUID(CLSID, CLSID_FPrinter,0x66E84B79,0x2B5C,0x422E,0x9D,0xEB,0xD1,0x5A,0x31,0x35,0x03,0xAF);


MIDL_DEFINE_GUID(IID, DIID_IPrintDlg,0x31F284BA,0x8B08,0x4B11,0xBB,0xB3,0x8A,0xE8,0x6F,0x6A,0x06,0x72);


MIDL_DEFINE_GUID(CLSID, CLSID_PrintDlg,0x71A36926,0xB3C8,0x4ED5,0xBD,0x63,0xA7,0x26,0x77,0x4D,0x04,0xF2);


MIDL_DEFINE_GUID(IID, DIID_IMainWnd,0x059274D8,0xE747,0x48DB,0xBA,0x58,0xCC,0xF0,0x5A,0xD9,0x51,0xD0);


MIDL_DEFINE_GUID(CLSID, CLSID_MainWnd,0x68078129,0x659E,0x4E90,0x81,0x41,0xBF,0x95,0x0A,0x2B,0x51,0xFE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



