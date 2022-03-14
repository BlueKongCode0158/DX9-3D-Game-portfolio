#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CLevel : public CBase
{
protected:
	/* 자식클래스에서 생성자로 만들어줘야하기때문에 protected로 만들어준다. */
	explicit CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int	Tick(_double TimeDelta);
	virtual HRESULT	Render();
protected:
	LPDIRECT3DDEVICE9	m_pGraphic_Deviec = nullptr;
public:
	virtual void Free() override;
};
END

