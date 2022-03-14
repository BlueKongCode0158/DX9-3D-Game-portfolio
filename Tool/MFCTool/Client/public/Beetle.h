#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CNormal_Monster_Hp;
class CBeetle final : public CMonster
{
public:
	enum ANIMATION_KEY
	{
		LIGHTIMPACT,
		AIRLOOPUP,
		AIRLOOPDOWN,
		JUMP,
		WALKFORWARD,
		DEATH,
		FIRESUNDER,
		BUFFSELF,
		SPAWN,
		AIMYAW,
		AIMPITCH,
		HURT,
		FLINCH3,
		FLINCH2,
		FLINCH1,
		GROUNDSMASH,
		RUNBACKWARD,
		RUNRIGHTFORWARD,
		RUNLEFTFORWARD,
		RUNFORWARD,
		IDLESINGLE,
		IDLEADDITIVE
	};
private:
	explicit CBeetle(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBeetle(const CBeetle& rhs);
	virtual  ~CBeetle() = default;
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
	CNormal_Monster_Hp* m_pHpBar = nullptr;
public:
	static  CBeetle*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};
END
