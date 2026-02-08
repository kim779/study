///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.03.12
//
//	용법	:	1. Contructing시에 이름을 설정하여 Loading하고, Destructing으로 제거하는 방법.
//				2. 기본 생성후 LodDll()로 DLL을 Loading하고, Destructing으로 제거하는 방법.
//				3. SetDllName()과 Initialize()로 Loading하여 Uninitialize()로 제거하는 방법.
//					특히 COM과 같이, 공유에 의한 Reference Counter 개념을 이용할 경우 필수.
//
//	수정	:	(2003.03.12) 자체 제작한 Module의 Loading이므로 함수 Address를 CallBack Pointer로
//					정의되지 않도록 Macro를 조정한다.
//				(2003.12.28) 속도를 측정하는 TimeMeter DLL의 동적 Loading을 위하여,
//					Default Parameter와 No Return Value를 적용시킨다.
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Class:  PDll																				  //
//  Authors: MicHael Galkovsky                                                                    //
//  Date:    April 14, 1998                                                                       //
//  Company:  Pervasive Software                                                                  //
//  Purpose:    Base class to wrap dynamic use of dll											  //
////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined (_PDLL_H_)
#define _PDLL_H_

#include <windows.h>
#include <winbase.h>

#define FUNC_LOADED 3456

//function declarations according to the number of parameters
//define the type
//declare a variable of that type
//declare a member function by the same name as the dll function
//check for dll handle
//if this is the first call to the function then try to load it
//if not then if the function was loaded successfully make a call to it
//otherwise return a NULL cast to the return parameter.

#define DECLARE_FUNCTION0( FuncName, Result, DefaultResult) \
	typedef  Result (* TYPE_##FuncName)( void); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	Result FuncName( void) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				 return m_##FuncName(); \
		} \
		return DefaultResult;	\
	}   	

#define DECLARE_FUNCTION1( FuncName, Result, DefaultResult, Param1) \
	typedef  Result (* TYPE_##FuncName)( Param1 p1); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	Result FuncName( Param1 p1) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				 return m_##FuncName( p1); \
		} \
		return DefaultResult;	\
	}   	

#define DECLARE_FUNCTION2( FuncName, Result, DefaultResult, Param1, Param2) \
	typedef  Result (* TYPE_##FuncName)( Param1 p1, Param2 p2); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	Result FuncName( Param1 p1, Param2 p2) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				 return m_##FuncName( p1, p2); \
		} \
		return DefaultResult;	\
	}   	

#define DECLARE_FUNCTION3( FuncName, Result, DefaultResult, Param1, Param2, Param3) \
	typedef  Result (* TYPE_##FuncName)( Param1 p1, Param2 p2, Param3 p3); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	Result FuncName( Param1 p1, Param2 p2, Param3 p3) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				 return m_##FuncName( p1, p2, p3); \
		} \
		return DefaultResult;	\
	}   	

#define DECLARE_FUNCTION4( FuncName, Result, DefaultResult, Param1, Param2, Param3, Param4) \
	typedef  Result (* TYPE_##FuncName)( Param1 p1, Param2 p2, Param3 p3, Param4 p4); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	Result FuncName( Param1 p1, Param2 p2, Param3 p3, Param4 p4) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				 return m_##FuncName( p1, p2, p3, p4); \
		} \
		return DefaultResult;	\
	}   	

/*
#define DECLARE_FUNCTION0( FuncName) \
	typedef  void (* TYPE_##FuncName)(); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	void FuncName() \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				 m_##FuncName(); \
		} \
	}   	

#define DECLARE_FUNCTION1( FuncName, Param1) \
	typedef  void (* TYPE_##FuncName)(Param1); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName(Param1 p1) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1); \
		} \
	}

#define DECLARE_FUNCTION1_V1( FuncName, Param1, Value1) \
	typedef  void (* TYPE_##FuncName)(Param1); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName(Param1 p1 = Value1) \
	{ \
		if (m_dllHandle) \
		{ \
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1); \
		} \
	}

#define DECLARE_FUNCTION2( FuncName, Param1, Param2) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2); \
		} \
	}

#define DECLARE_FUNCTION2_V2( FuncName, Param1, Param2, Value2) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2 = Value2) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2); \
		} \
	}

#define DECLARE_FUNCTION3( FuncName, Param1, Param2, Param3) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED; \
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3);\
		} \
	}

#define DECLARE_FUNCTION3_V3( FuncName, Param1, Param2, Param3, Value3) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3 = Value3) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED; \
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3);\
		} \
	}

#define DECLARE_FUNCTION4( FuncName, Param1, Param2, Param3, Param4) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3, Param4); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4);\
		} \
	}

#define DECLARE_FUNCTION5( FuncName, Param1, Param2, Param3, Param4, Param5) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3, Param4, Param5); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4, p5);\
		} \
	}

#define DECLARE_FUNCTION6( FuncName, Param1, Param2, Param3, Param4, Param5, Param6) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3, Param4, Param5, Param6); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4, p5, p6);\
		} \
	}

#define DECLARE_FUNCTION7( FuncName, Param1, Param2, Param3, Param4, Param5, Param6, Param7) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3, Param4, Param5, Param6, Param7); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4, p5, p6, p7);\
		} \
	}

#define DECLARE_FUNCTION8( FuncName, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4, p5, p6, p7, p8);\
		}\
	}

#define DECLARE_FUNCTION9( FuncName, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9) \
	typedef  void (* TYPE_##FuncName)(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName; \
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_NAME != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4, p5, p6, p7, p8, p9);\
		}\
	}

#define DECLARE_FUNCTION10( FuncName, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10) \
	typedef  void (* TYPE_##FuncName)FuncName(Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10); \
	TYPE_##FuncName m_##FuncName; \
	short m_is##FuncName;\
	void FuncName (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10) \
	{\
		if (m_dllHandle)\
		{\
			if (FUNC_LOADED != m_is##FuncName) \
			{\
				m_##FuncName = NULL; \
				m_##FuncName = (TYPE_##FuncName)GetProcAddress(m_dllHandle, #FuncName); \
				m_is##FuncName = FUNC_LOADED;\
			}\
			if (NULL != m_##FuncName) \
				m_##FuncName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);\
		}\
	}
*/

//declare constructors and LoadFunctions
#define DECLARE_CLASS(ClassName) \
	public:	\
	ClassName (const char* name){LoadDll(name);} \
	ClassName () {PDLL();}

class PDLL
{

protected:
	HINSTANCE m_dllHandle;
	char* m_dllName;
	int m_refCount;

public:

	PDLL()
	{
		m_dllHandle = NULL;
		m_dllName = NULL;
		m_refCount = 0;
	}

	//A NULL here means the name has already been set
	void LoadDll(const char* name, short showMsg = 0)
	{
		if (name)
			SetDllName(name);

		//try to load
		m_dllHandle = LoadLibrary(m_dllName);

		if (m_dllHandle == NULL && showMsg)
		{
			//show warning here if needed
		}		
	}

	bool SetDllName(const char* newName)
	{
		bool bResult = false;

		//we allow name resets only if the current DLL handle is invalid
		//once they've hooked into a DLL, the  name cannot be changed
		if (!m_dllHandle)
		{
			if (m_dllName)
			{
				delete []m_dllName;
				m_dllName = NULL;
			}

			//They may be setting this null (e.g., uninitialize)
			if (newName)
			{
				m_dllName = new char[strlen(newName) + 1];
				//make sure memory was allocated
				if (m_dllName)
					strcpy(m_dllName, newName);
				else
					bResult = false;
			}
			bResult = true;
		}
		return bResult;
	}

	virtual bool Initialize(short showMsg = 0)
	{

		bool bResult = false;

		//Add one to our internal reference counter
		m_refCount++;

		if (m_refCount == 1 && m_dllName) //if this is first time, load the DLL
		{
			//we are assuming the name is already set
    		LoadDll(NULL, showMsg);
			bResult = (m_dllHandle != NULL);
		}
		return bResult;
	}

	virtual void Uninitialize(void)
	{
		//If we're already completely unintialized, early exit
		if (!m_refCount)
			return;

		//if this is the last time this instance has been unitialized, 
		//then do a full uninitialization
		m_refCount--;

		if( m_refCount < 1)
		{
			if( m_dllHandle)
    		{
	    		FreeLibrary(m_dllHandle);
    			m_dllHandle = NULL;
			}

			SetDllName(NULL); //clear out the name & free memory
		}
	}

	~PDLL()
	{
		//force this to be a true uninitialize
		m_refCount = 1; 
		Uninitialize();

		//free name
		if (m_dllName)
		{
			delete [] m_dllName;
			m_dllName = NULL;
		}
	}

};
#endif
