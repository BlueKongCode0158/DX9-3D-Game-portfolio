#pragma once
#include "Engine_Defines.h"
#include "ImGUI_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CFrustum;
END

BEGIN(IMGUITOOL)
class CMainApp final : public CBase
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

