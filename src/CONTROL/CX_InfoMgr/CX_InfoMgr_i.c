

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_InfoMgr.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_CX_InfoMgr,0x41254F9C,0x4856,0x48C7,0xBC,0x03,0xEF,0x96,0x0C,0x3D,0x11,0x3F);


MIDL_DEFINE_GUID(IID, DIID_IControlWnd,0x7ECC238F,0xF8E1,0x4D3D,0xB6,0x86,0x66,0x33,0xD7,0x1C,0xAA,0x7F);


MIDL_DEFINE_GUID(CLSID, CLSID_ControlWnd,0x464E51F9,0x5EBB,0x4D55,0x9B,0xC9,0x05,0x4E,0x2A,0x2F,0x4F,0x7F);


MIDL_DEFINE_GUID(IID, DIID_IInfoWnd,0x8148CB88,0x1196,0x41FE,0x9B,0x83,0x7A,0xB9,0x93,0x4D,0x07,0xDA);


MIDL_DEFINE_GUID(CLSID, CLSID_InfoWnd,0x19D982F7,0x8F7E,0x4EA3,0x95,0x9E,0x02,0x95,0xC8,0x35,0x65,0xAC);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



