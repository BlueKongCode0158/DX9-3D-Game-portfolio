#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)
class CMonster;
class CMonster_Data;
class CMonsterState final : public CState
{
public:
	enum MONSTERTYPE
	{
		WISP,
		GOLEM,
		BEETLE,
		VAGRANT,
		TITAN,
		MONSTER_END
	};
public:
	enum MOTION
	{
		SPAWN,
		IDLE,
		MOVE,
		HURT,
		BUFF,
		ATTACK,
		AREA_ATTACK,
		SIGNATURE_ATTACK,
		DEATH,
		MOTION_END
	};
private:
	explicit CMonsterState(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonsterState() = default;
private:
	virtual HRESULT NativeConstruct(CMonsterState::MONSTERTYPE eType, CMonster* pObject);
public:
	virtual _uint	HandleInput();
	virtual void	Enter();
	virtual void	Update(_float TimeDelta);
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit();
private:
	void	Change_State();
private:
	HRESULT	Setting_MonsterType_State(CMonsterState::MONSTERTYPE eType);
private:
	HRESULT	isCreateWisp();
	HRESULT isCreateGolem();
	HRESULT isCreateBeetle();
	HRESULT isCreateVagrant();
	HRESULT isCreateTitan();
private:
	MOTION		m_iCurrentAnimIndex = MOTION::SPAWN;
	MOTION		m_iNewAnimIndex		= MOTION::SPAWN;
private:
	_bool		m_isDeath = false;
private:
	CMonster*	m_pMonster = nullptr;
private:
	map<CMonsterState::MOTION, CMonster_Data*>	m_mapMonsterState;
public:
	static	CMonsterState*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CMonsterState::MONSTERTYPE eType, CMonster* pObject);
	virtual void			Free() override;
};
END
