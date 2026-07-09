

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jul 06 09:17:12 2017
 */
/* Compiler settings for fx_codectrl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


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
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_fx_codectrl,0x9717A53F,0x325A,0x456E,0x99,0xEF,0x57,0xD5,0x28,0x03,0x29,0xD5);


MIDL_DEFINE_GUID(IID, DIID_IfxCodeCtrl,0x656726DE,0x5FD2,0x4929,0x8F,0x97,0xF5,0x74,0x64,0xD0,0xAD,0x50);


MIDL_DEFINE_GUID(CLSID, CLSID_fxCodeCtrl,0x779AEC8F,0x2D8A,0x4FD9,0xB5,0xCA,0xE9,0xB0,0x1B,0x4D,0xA1,0xBE);


MIDL_DEFINE_GUID(IID, DIID_IControlwnd,0x3EBBD33B,0x2F57,0x4ACA,0x8C,0x33,0xD8,0x22,0x19,0xDC,0xE4,0xF4);


MIDL_DEFINE_GUID(CLSID, CLSID_Controlwnd,0x765AC45F,0x31E4,0x43E4,0xA1,0xC7,0x33,0xCF,0x26,0xC7,0x40,0xF2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



