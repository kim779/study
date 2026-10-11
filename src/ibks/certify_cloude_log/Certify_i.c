

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Certify.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_CertifyLib,0x5254d530,0x2db7,0x4224,0xa2,0xf7,0xe3,0x30,0xd0,0xdf,0xee,0x54);


MIDL_DEFINE_GUID(IID, DIID__DCertify,0x45b73aba,0xcdd7,0x4cb0,0xa9,0xf3,0xd5,0x38,0x6e,0x3c,0xfa,0xcd);


MIDL_DEFINE_GUID(IID, DIID__DCertifyEvents,0x605e9aa3,0xdfb7,0x4ab1,0xbd,0x82,0xcd,0xa3,0x2d,0xa4,0xf5,0x0a);


MIDL_DEFINE_GUID(CLSID, CLSID_Certify,0x3277f0b4,0xb8be,0x42aa,0x9b,0x58,0x2f,0xa8,0x88,0x86,0xac,0x64);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



