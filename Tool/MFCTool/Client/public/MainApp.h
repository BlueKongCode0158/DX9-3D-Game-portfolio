#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CFrustum;
END

BEGIN(Client)
class CMainApp final : public CBase
	/* 오류 났던 이유 : Engine에 있는 CBase라고 정확하게 표기해주지 않았기때문에*/
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT NativeConstruct();
	_int	Tick(_double TimeDelta);
	HRESULT Render();
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device	= nullptr;
	CRenderer*			m_pRenderer			= nullptr;
	CFrustum*			m_pFrustum			= nullptr;
private:
	HRESULT	Add_Prototype_GameObject();
	HRESULT Add_Prototype_Component();
	HRESULT Add_Light_Desc();
public:
	static CMainApp* Create();
public:
	virtual void Free() override;
};
END

