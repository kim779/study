

/*
(1) 헤더에 다음 인터페이스를 추가
// {{ get/setAll Interface implementation
	// made by Choi Jin Won ( 2003. 02. 21 )
	class CAllPropGetterSetter : public IBaseAllGetterSetter
	{
	public:
		virtual void getAllCtrlProperty(CStringList* plist);
		virtual BOOL setAllCtrlProperty(CStringList* plist);
		virtual void setBaseDesk(long lKey);
		virtual void setRscMng(long lKey);
	} m_xAllGetterSetter;
	friend class CAllPropGetterSetter;
// }}



(2) cpp에 다음 구현부를 추가 (주의: classOrig에 대한 define문은 현재 컨트롤에 맞게 변경
/////////////////////////////////////////////////////////////////////////////
// CAllPropGetterSetter Implementation
#define classOrig	CHtcComboCtrl
void classOrig::CAllPropGetterSetter::getAllCtrlProperty(CStringList* plist)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)
	
	pThis->GetAllProperties2((long)plist);
}

BOOL classOrig::CAllPropGetterSetter::setAllCtrlProperty(CStringList* plist)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)

	return pThis->SetAllProperties2((long)plist);
}

void classOrig::CAllPropGetterSetter::setBaseDesk(long lKey)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)

	return pThis->SetBaseDesk(lKey);
}

void classOrig::CAllPropGetterSetter::setRscMng(long lKey)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)

	return pThis->SetRscMng(lKey);	
}
/////////////////////////////////////////////////////////////////////////////
*/

interface IBaseAllGetterSetter
{
public:
		virtual void getAllCtrlProperty(CStringList* plist) PURE;
		virtual BOOL setAllCtrlProperty(CStringList* plist) PURE;
		virtual void setBaseDesk(long lKey) PURE;
		virtual void setRscMng(long lKey) PURE;
};