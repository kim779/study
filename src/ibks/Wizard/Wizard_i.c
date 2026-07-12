

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Wizard.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_WizardLib,0x18c830e8,0x7bd4,0x4d87,0xb3,0x2e,0x6f,0xdb,0x4e,0xdd,0x61,0x07);


MIDL_DEFINE_GUID(IID, DIID__DWizard,0x72d9dc6c,0x90be,0x4e43,0x9e,0x19,0x48,0xb9,0x68,0x83,0x0a,0xe7);


MIDL_DEFINE_GUID(IID, DIID__DWizardEvents,0xbf500ab3,0x0b43,0x43e8,0xad,0xc4,0xf9,0x44,0xd2,0x26,0xda,0x5f);


MIDL_DEFINE_GUID(CLSID, CLSID_Wizard,0xfe2fc2d9,0x055b,0x4095,0x88,0x06,0xc9,0x83,0x67,0x18,0x78,0x68);


MIDL_DEFINE_GUID(IID, DIID_IxScreen,0x89CFEA19,0x83BB,0x4DB3,0xBA,0x70,0xD5,0x09,0x79,0x4C,0x10,0x91);


MIDL_DEFINE_GUID(CLSID, CLSID_xScreen,0x8B184965,0x2CFD,0x4134,0x9D,0x99,0x59,0xDD,0xEA,0x5E,0x0F,0xA5);


MIDL_DEFINE_GUID(IID, DIID_IxSystem,0x557CAD32,0x82F6,0x4BA8,0xA5,0x5E,0xDD,0xF8,0xD3,0x78,0xDC,0xBE);


MIDL_DEFINE_GUID(CLSID, CLSID_xSystem,0x774C1579,0x3E4A,0x485F,0x9E,0xBC,0xAD,0x47,0xBC,0xA5,0x4B,0x60);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



