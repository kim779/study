

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_MARKETMAP.odl:
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


#ifndef __CX_MARKETMAP_h_h__
#define __CX_MARKETMAP_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMainWnd_FWD_DEFINED__
#define __IMainWnd_FWD_DEFINED__
typedef interface IMainWnd IMainWnd;

#endif 	/* __IMainWnd_FWD_DEFINED__ */


#ifndef __MainWnd_FWD_DEFINED__
#define __MainWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class MainWnd MainWnd;
#else
typedef struct MainWnd MainWnd;
#endif /* __cplusplus */

#endif 	/* __MainWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_MARKETMAP_LIBRARY_DEFINED__
#define __CX_MARKETMAP_LIBRARY_DEFINED__

/* library CX_MARKETMAP */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_MARKETMAP,0xB65F8040,0x9824,0x47F6,0x98,0x41,0x62,0x76,0xF1,0xE5,0xA6,0x70);

#ifndef __IMainWnd_DISPINTERFACE_DEFINED__
#define __IMainWnd_DISPINTERFACE_DEFINED__

/* dispinterface IMainWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMainWnd,0x8201BFF2,0x5214,0x424B,0x81,0xA8,0x2A,0x53,0xF8,0xB1,0xA0,0xB5);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8201BFF2-5214-424B-81A8-2A53F8B1A0B5")
    IMainWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMainWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMainWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMainWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMainWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMainWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMainWnd * This,
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
    } IMainWndVtbl;

    interface IMainWnd
    {
        CONST_VTBL struct IMainWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMainWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMainWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMainWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMainWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMainWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMainWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMainWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMainWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MainWnd,0xD29D0B78,0x4244,0x492B,0xB4,0x63,0x6A,0x0F,0x49,0x91,0x87,0xA1);

#ifdef __cplusplus

class DECLSPEC_UUID("D29D0B78-4244-492B-B463-6A0F499187A1")
MainWnd;
#endif
#endif /* __CX_MARKETMAP_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


