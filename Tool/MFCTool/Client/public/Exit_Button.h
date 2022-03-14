#pragma once
#include "UI.h"

BEGIN(Client)
class CExit_Button final : public CUI
{
private:
	explicit CExit_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CExit_Button(const CExit_Button& rhs);
	virtual	 ~CExit_Button() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
private:
	HRESULT	SetUp_ConstantTable();
	HRESULT Add_Component();
private:
	LPDIRECT3DTEXTURE9	m_pTexture_NonSelect	= nullptr;
public:
	_bool	Get_Select()
	{
		return m_isSelect;
	}
private:
	_bool				m_isSelecting			= false;
	_bool				m_isSelect				= false;
public:
	static	CExit_Button*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
END
