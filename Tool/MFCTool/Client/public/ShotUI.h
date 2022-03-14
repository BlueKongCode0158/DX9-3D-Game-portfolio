#pragma once
#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CShotUI final : public CUI
{
private:
	explicit CShotUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CShotUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
public:
	_float3*	Get_TargetPosition() const
	{
		return m_pPosition;
	}
private:
	HRESULT	Add_Components();
private:
	CTexture*			m_pTextureCom		= nullptr;
private:
	_bool				m_isActive = false;
	_float3*			m_pPosition = nullptr;
public:
	static CShotUI*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual	void			Free() override;

};
END
