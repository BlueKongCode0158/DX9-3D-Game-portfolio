#pragma once
#include "Base.h"

/*
오브젝트가 컴포넌트를 가지고 있도록 만듦.
필요한 기능이 있을 때 해당 기능의 컴포넌트를 오브젝트에 집어넣어 작동 되도록 만든다.
*/

BEGIN(Engine)
class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	// 프로토타입 설정함수.
	virtual	HRESULT NativeConstruct_Prototype();
	// Clone 설정함수.
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	// 오브젝트 Render ( 그릴 때 ) 돌아가는 기능
	virtual HRESULT Render();
protected:
	LPDIRECT3DDEVICE9	m_pGraphic_Device	= nullptr;
	_bool				m_isClone			= false;
public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;
	virtual void		Free() override;
};
END
