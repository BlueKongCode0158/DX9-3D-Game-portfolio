#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CNormal_Monster_Hp;

class CGolem final : public CMonster
{
public:
	enum ANIMATION_KEY
	{
		AIRLOOPDOWN,
		AIRLOOPUP,
		DEATH,
		FIRELASER,
		FLINCH1,
		FLINCH2,
		HURT,
		IDLE_ADDITIVE,
		IDLE_SINGLE,
		JUMP,
		LOOK_HORIZONTAL,
		LOOK_VERTICAL,
		RUNBACKWARD_RM,
		RUNFORWARD_RM,
		RUNLEFT_RM,
		RUNRIGHT_RM,
		RUNTOIDLELEFT,
		RUNTOIDLERIGHT,
		SMACK,
		SPAWN
	};
private:
	explicit CGolem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGolem(const CGolem& rhs);
	virtual  ~CGolem() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT	Render()					override;
private:
	HRESULT Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CNormal_Monster_Hp*		m_pHpBar = nullptr;
public:
	static  CGolem*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};
END
