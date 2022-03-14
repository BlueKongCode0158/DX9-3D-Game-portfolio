#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CLoadingBar final : public CUI
{
private:
	explicit CLoadingBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CLoadingBar(const CLoadingBar& rhs);
	virtual  ~CLoadingBar() = default;
public:
	void	Set_LoadingPer(_float fLoadingPer)
	{
		m_fFinishPer = fLoadingPer;
	}
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
private:
	HRESULT	Add_Components();
private:
	_float				m_fFinishPer = 0.f;
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
public:
	static CLoadingBar*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual	void			Free() override;
};
END
