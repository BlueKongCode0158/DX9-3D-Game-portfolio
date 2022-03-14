#pragma once
#include "UI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CMouse_Cursor final : public CUI
{
private:
	explicit CMouse_Cursor(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMouse_Cursor() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
public:
	void	Set_isActive(_bool isActive)
	{
		m_isActive = isActive;
	}
	_bool	Get_isActive()
	{
		return m_isActive;
	}
private:
	HRESULT	Add_Components();
private:
	CTexture*			m_pTextureCom	= nullptr;
private:
	_bool				m_isActive		= false;
public:
	static CMouse_Cursor*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual	void			Free() override;

};
END
