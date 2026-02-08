

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for DX_OptionSens.odl:
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


#ifndef __DX_OptionSens_h_h__
#define __DX_OptionSens_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IComponent_FWD_DEFINED__
#define __IComponent_FWD_DEFINED__
typedef interface IComponent IComponent;

#endif 	/* __IComponent_FWD_DEFINED__ */


#ifndef __Component_FWD_DEFINED__
#define __Component_FWD_DEFINED__

#ifdef __cplusplus
typedef class Component Component;
#else
typedef struct Component Component;
#endif /* __cplusplus */

#endif 	/* __Component_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DX_OptionSens_LIBRARY_DEFINED__
#define __DX_OptionSens_LIBRARY_DEFINED__

/* library DX_OptionSens */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_DX_OptionSens,0x0E78A383,0xD00D,0x406E,0x8A,0x0C,0x81,0x35,0xF6,0xCA,0x36,0x25);

#ifndef __IComponent_DISPINTERFACE_DEFINED__
#define __IComponent_DISPINTERFACE_DEFINED__

/* dispinterface IComponent */
/* [uuid] */ 


DEFINE_GUID(DIID_IComponent,0x6AA3219B,0xF740,0x4DCA,0xA9,0xFA,0x41,0xE4,0x72,0xAC,0x73,0x85);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6AA3219B-F740-4DCA-A9FA-41E472AC7385")
    IComponent : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IComponentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComponent * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComponent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComponent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComponent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComponent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComponent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComponent * This,
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
    } IComponentVtbl;

    interface IComponent
    {
        CONST_VTBL struct IComponentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComponent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComponent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComponent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComponent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComponent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComponent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComponent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IComponent_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Component,0xBA317DE7,0x1B03,0x45CC,0xB8,0x19,0xF5,0x51,0xF7,0xBA,0x8B,0xB9);

#ifdef __cplusplus

class DECLSPEC_UUID("BA317DE7-1B03-45CC-B819-F551F7BA8BB9")
Component;
#endif
#endif /* __DX_OptionSens_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


