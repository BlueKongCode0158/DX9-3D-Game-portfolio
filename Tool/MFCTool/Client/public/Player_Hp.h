#pragma once
#include "UI.h"

BEGIN(Engine)
class CPlayer_Hp final : public CUI
{
private:
	explicit CPlayer_Hp(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer_Hp(const CPlayer_Hp& rhs);
	virtual ~CPlayer_Hp() =default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
private:
	HRESULT Add_Component();
private:
	_uint		m_iHp		= 0;
	_float*		m_pHpPer	= nullptr;
public:
	static	CPlayer_Hp*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
