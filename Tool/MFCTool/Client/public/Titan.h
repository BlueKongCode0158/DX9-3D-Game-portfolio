#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CDynamic_Mesh;
END

BEGIN(Client)
class CTitan final : public CMonster
{
public:
	enum ANIMATION_KEY
	{
		DEATH,
		SPAWN,
		RECHARGE_ROCKS,
		CROUCH,
		FLINCH3,
		FLINCH2,
		FLINCH1,
		RETRACT_FIST,
		PREP_FIST,
		WALKRIGHT,
		WALKBACKWARD,
		WALKLEFT,
		WALKFORWARD,
		FIRE_LASERLOOP,
		LANDING,
		IDLE_SINGLE,
		IDLE_LOOP,
		FIRE_LASER_END,
		AIRLOOPUP,
		AIRLOOPDOWN,
		AIMYAW,
		AIMPITCH,
	};
private:
	explicit CTitan(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTitan(const CTitan& rhs);
	virtual  ~CTitan() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT	Render()					override;
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();
public:
	static	CTitan*		 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)		override;
	virtual void		 Free()					override;
};
END
