#pragma once
#include "Client_Defines.h"
#include "Monster.h"


BEGIN(Client)
class CBossHp_BackGround;
class CBoss_Hp;
class CVagrant final : public CMonster
{
public:
	enum ANIMATION_KEY
	{
		PULL_UP_FEET,
		SPAWN,
		FIRETRACKING_BOMB,
		CHARGETRACKING_BOMB,
		POST_NOVA,
		PRE_NOVA,
		FLINCH3,
		FLINCH2,
		FLINCH1,
		IDLE_LEGS_ADDITIVE,
		IDLE_HULL_ADDITIVE,
		IDLE_SINGLE
	};
private:
	explicit CVagrant(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVagrant(const CVagrant& rhs);
	virtual  ~CVagrant() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT	Render()					override;
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CBossHp_BackGround*	m_pHpBackGround = nullptr;;
	CBoss_Hp*			m_pHpBar = nullptr;
public:
	static	CVagrant*	 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)  override;
	virtual void		 Free()				override;
};
END
