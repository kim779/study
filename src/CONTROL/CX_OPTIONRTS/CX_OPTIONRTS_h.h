

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_OPTIONRTS.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __CX_OPTIONRTS_h_h__
#define __CX_OPTIONRTS_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMainwnd_FWD_DEFINED__
#define __IMainwnd_FWD_DEFINED__
typedef interface IMainwnd IMainwnd;

#endif 	/* __IMainwnd_FWD_DEFINED__ */


#ifndef __Mainwnd_FWD_DEFINED__
#define __Mainwnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class Mainwnd Mainwnd;
#else
typedef struct Mainwnd Mainwnd;
#endif /* __cplusplus */

#endif 	/* __Mainwnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_OPTIONRTS_LIBRARY_DEFINED__
#define __CX_OPTIONRTS_LIBRARY_DEFINED__

/* library CX_OPTIONRTS */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_OPTIONRTS,0x287A8C3E,0x4453,0x4CE8,0x92,0xD9,0x31,0xC5,0x61,0xBD,0xBC,0x9D);

#ifndef __IMainwnd_DISPINTERFACE_DEFINED__
#define __IMainwnd_DISPINTERFACE_DEFINED__

/* dispinterface IMainwnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMainwnd,0xF22C8A42,0xD70A,0x4E59,0xBD,0x1F,0xE6,0xD9,0xC0,0x6A,0x9D,0xED);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F22C8A42-D70A-4E59-BD1F-E6D9C06A9DED")
    IMainwnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMainwndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMainwnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMainwnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMainwnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMainwnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMainwnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMainwnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMainwnd * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IMainwndVtbl;

    interface IMainwnd
    {
        CONST_VTBL struct IMainwndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMainwnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMainwnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMainwnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMainwnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMainwnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMainwnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMainwnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMainwnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Mainwnd,0x3DDEC5B9,0xEBA9,0x4B7B,0xAC,0xA7,0x4A,0xAE,0xB4,0x1E,0x4E,0x59);

#ifdef __cplusplus

class DECLSPEC_UUID("3DDEC5B9-EBA9-4B7B-ACA7-4AAEB41E4E59")
Mainwnd;
#endif
#endif /* __CX_OPTIONRTS_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


