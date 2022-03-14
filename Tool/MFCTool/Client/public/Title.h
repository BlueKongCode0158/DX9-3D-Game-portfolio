#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CTitle final : public CUI
{
private:
	explicit CTitle(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTitle(const CTitle& rhs);
	virtual  ~CTitle() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
private:
	HRESULT Add_Component();
private:
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
public:
	static	CTitle*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
