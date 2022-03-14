#include "stdafx.h"
#include "..\public\MonsterState.h"
#include "Monster_Data.h"
#pragma region BEETLE
#include "Monster_Beetle_Attack.h"
#include "Monster_Beetle_Buff.h"
#include "Monster_Beetle_Death.h"
#include "Monster_Beetle_Idle.h"
#include "Monster_Beetle_Move.h"
#include "Monster_Beetle_Spawn.h"
#pragma endregion
#pragma region GOLEM
#include "Monster_Golem_Attack.h"
#include "Monster_Golem_Death.h"
#include "Monster_Golem_Idle.h"
#include "Monster_Golem_Move.h"
#include "Monster_Golem_Spawn.h"
#include "Monster_Golem_Flinch.h"
#include "Monster_Golem_Laser_Attack.h"
#pragma endregion
#pragma region WISP
#include "Monster_Wisp_Attack.h"
#include "Monster_Wisp_Death.h"
#include "Monster_Wisp_Idle.h"
#include "Monster_Wisp_Move.h"
#include "Monster_Wisp_Spawn.h"
#pragma endregion
#pragma region TITAN
#include "Monster_Titan_Flinch.h"
#include "Monster_Titan_Spawn.h"
#include "Monster_Titan_Idle.h"
#include "Monster_Titan_Attack.h"
#include "Monster_Titan_Move.h"
#include "Monster_Titan_Death.h"
#pragma endregion
#pragma region VAGRANT
#include "Monster_Vagrant_Spawn.h"
#include "Monster_Vagrant_Idle.h"
#include "Monster_Vagrant_Hurt.h"
#include "Monster_Vagrant_Attack.h"
#include "Monster_Vagrant_Area_Attack.h"
#pragma endregion


CMonsterState::CMonsterState(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CState(pGraphic_Device)
{
}

HRESULT CMonsterState::NativeConstruct(CMonsterState::MONSTERTYPE eType, CMonster* pObject)
{
	if (nullptr == pObject)
	{
		return E_FAIL;
	}

	m_pMonster = dynamic_cast<CMonster*>(pObject);

	if (nullptr == m_pMonster)
	{
		return E_FAIL;
	}

	if (FAILED(Setting_MonsterType_State(eType)))
	{
		return E_FAIL;
	}
	m_mapMonsterState[m_iCurrentAnimIndex]->Enter();

	return S_OK;
}

_uint CMonsterState::HandleInput()
{
	if (nullptr == m_mapMonsterState[m_iCurrentAnimIndex])
	{
		return 0;
	}
	m_iNewAnimIndex = static_cast<MOTION>(m_mapMonsterState[m_iCurrentAnimIndex]->HandleInput());


	if (m_pMonster->Get_Hp() <= 0)
	{
		m_iNewAnimIndex = CMonsterState::MOTION::DEATH;
	}
	Change_State();

	return m_iNewAnimIndex;
}

void CMonsterState::Enter()
{
	m_iCurrentAnimIndex = CMonsterState::MOTION::SPAWN;
	return;
}

void CMonsterState::Update(_float TimeDelta)
{
	if (nullptr == m_mapMonsterState[m_iCurrentAnimIndex])
	{
		return;
	}
	m_mapMonsterState[m_iCurrentAnimIndex]->Update(TimeDelta);

	return;
}

void CMonsterState::Late_Update(_float TimeDelta)
{
	if (nullptr == m_mapMonsterState[m_iCurrentAnimIndex])
	{
		return;
	}
	m_mapMonsterState[m_iCurrentAnimIndex]->Late_Update(TimeDelta);

	return;
}

void CMonsterState::Exit()
{
	if (nullptr != m_mapMonsterState[m_iCurrentAnimIndex])
	{
		return;
	}
	m_mapMonsterState[m_iCurrentAnimIndex]->Exit();

	return;
}

void CMonsterState::Change_State()
{
	if (m_iNewAnimIndex != m_iCurrentAnimIndex)
	{
		auto&	pState1 = m_mapMonsterState[m_iCurrentAnimIndex];
		if (nullptr == pState1)
		{
			return;
		}
		pState1->Exit();
		_float3	fSkil_Time	= pState1->Get_SkilTime();
		_float	fIdleTime	= pState1->Get_IdleTime();
		_bool	isRcog		= pState1->Get_Recog();
		m_iCurrentAnimIndex = m_iNewAnimIndex;

		auto&	pState2 = m_mapMonsterState[m_iCurrentAnimIndex];
		if (nullptr == pState2)
		{
			return;
		}
		pState2->Set_SkilTime(fSkil_Time.x, fSkil_Time.y, fSkil_Time.z);
		pState2->Set_IdleTime(fIdleTime);
		pState2->Set_Recog(isRcog);
		pState2->Enter();
	}
}

HRESULT CMonsterState::Setting_MonsterType_State(CMonsterState::MONSTERTYPE eType)
{
	HRESULT hr;
	switch (eType)
	{
	case CMonsterState::MONSTERTYPE::WISP:
		hr = isCreateWisp();
		break;
	case CMonsterState::MONSTERTYPE::GOLEM:
		hr = isCreateGolem();
		break;
	case CMonsterState::MONSTERTYPE::BEETLE:
		hr = isCreateBeetle();
		break;
	case CMonsterState::MONSTERTYPE::VAGRANT:
		hr = isCreateVagrant();
		break;
	case CMonsterState::MONSTERTYPE::TITAN:
		hr = isCreateTitan();
		break;
	case CMonsterState::MONSTERTYPE::MONSTER_END:
		hr = E_FAIL;
		break;
	}
	return hr;
}

HRESULT CMonsterState::isCreateWisp()
{
	CMonster_Data* pMonsterIdle		= CMonster_Wisp_Idle::Create(m_pGraphic_Device,m_pMonster);
	CMonster_Data* pMonsterMove		= CMonster_Wisp_Move::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterAttack	= CMonster_Wisp_Attack::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterDeath	= CMonster_Wisp_Death::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterSpawn	= CMonster_Wisp_Spawn::Create(m_pGraphic_Device, m_pMonster);

	if (nullptr == pMonsterIdle		|| nullptr == pMonsterSpawn ||
		nullptr == pMonsterAttack	||
		nullptr == pMonsterDeath	|| nullptr == pMonsterMove)
	{
		return E_FAIL;
	}


	m_mapMonsterState.emplace(CMonsterState::IDLE, pMonsterIdle);
	m_mapMonsterState.emplace(CMonsterState::MOVE, pMonsterMove);
	m_mapMonsterState.emplace(CMonsterState::DEATH, pMonsterDeath);
	m_mapMonsterState.emplace(CMonsterState::SPAWN, pMonsterSpawn);
	m_mapMonsterState.emplace(CMonsterState::ATTACK, pMonsterAttack);

	return S_OK;
}

HRESULT CMonsterState::isCreateGolem()
{
	CMonster_Data* pMonsterIdle		= CMonster_Golem_Idle::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterMove		= CMonster_Golem_Move::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterAttack	= CMonster_Golem_Attack::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterDeath	= CMonster_Golem_Death::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterSpawn	= CMonster_Golem_Spawn::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterFlinch	= CMonster_Golem_Flinch::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterLaser	= CMonster_Golem_Laser_Attack::Create(m_pGraphic_Device, m_pMonster);
	
	if (nullptr == pMonsterIdle		|| nullptr == pMonsterSpawn		||
		nullptr == pMonsterAttack	|| nullptr == pMonsterFlinch	||
		nullptr == pMonsterDeath	|| nullptr == pMonsterMove		||
		nullptr == pMonsterLaser)
	{
		return E_FAIL;
	}

	m_mapMonsterState.emplace(CMonsterState::IDLE, pMonsterIdle);
	m_mapMonsterState.emplace(CMonsterState::MOVE, pMonsterMove);
	m_mapMonsterState.emplace(CMonsterState::DEATH, pMonsterDeath);
	m_mapMonsterState.emplace(CMonsterState::SPAWN, pMonsterSpawn);
	m_mapMonsterState.emplace(CMonsterState::ATTACK, pMonsterAttack);
	m_mapMonsterState.emplace(CMonsterState::HURT, pMonsterFlinch);
	m_mapMonsterState.emplace(CMonsterState::SIGNATURE_ATTACK, pMonsterLaser);
	return S_OK;
}

HRESULT CMonsterState::isCreateBeetle()
{
	CMonster_Data* pMonsterIdle		= CMonster_Beetle_Idle::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterMove		= CMonster_Beetle_Move::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterAttack	= CMonster_Beetle_Attack::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterDeath	= CMonster_Beetle_Death::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterSpawn	= CMonster_Beetle_Spawn::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterBuff		= CMonster_Beetle_Buff::Create(m_pGraphic_Device, m_pMonster);

	m_mapMonsterState.emplace(CMonsterState::IDLE, pMonsterIdle);
	m_mapMonsterState.emplace(CMonsterState::MOVE, pMonsterMove);
	m_mapMonsterState.emplace(CMonsterState::DEATH, pMonsterDeath);
	m_mapMonsterState.emplace(CMonsterState::SPAWN, pMonsterSpawn);
	m_mapMonsterState.emplace(CMonsterState::ATTACK, pMonsterAttack);
	m_mapMonsterState.emplace(CMonsterState::BUFF, pMonsterBuff);

	if (nullptr == pMonsterIdle		|| nullptr == pMonsterSpawn ||
		nullptr == pMonsterAttack	|| nullptr == pMonsterBuff  ||
		nullptr == pMonsterDeath	|| nullptr == pMonsterMove	)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonsterState::isCreateVagrant()
{
	CMonster_Data* pMonsterIdle			= CMonster_Vagrant_Idle::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterAttack		= CMonster_Vagrant_Attack::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterSpawn		= CMonster_Vagrant_Spawn::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterHurt			= CMonster_Vagrant_Hurt::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterAreaAttack	= CMonster_Vagrant_Area_Attack::Create(m_pGraphic_Device, m_pMonster);

	if (nullptr == pMonsterIdle		|| nullptr == pMonsterSpawn ||
		nullptr == pMonsterAttack	|| nullptr == pMonsterHurt	||
		nullptr == pMonsterAreaAttack)
	{
		return E_FAIL;
	}
	m_mapMonsterState.emplace(CMonsterState::IDLE, pMonsterIdle);
	m_mapMonsterState.emplace(CMonsterState::SPAWN, pMonsterSpawn);
	m_mapMonsterState.emplace(CMonsterState::HURT, pMonsterHurt);
	m_mapMonsterState.emplace(CMonsterState::ATTACK, pMonsterAttack);
	m_mapMonsterState.emplace(CMonsterState::AREA_ATTACK, pMonsterAreaAttack);
	return S_OK;
}

HRESULT CMonsterState::isCreateTitan()
{
	CMonster_Data* pMonsterIdle		= CMonster_Titan_Idle::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterAttack	= CMonster_Titan_Attack::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterSpawn	= CMonster_Titan_Spawn::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterMove		= CMonster_Titan_Move::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterDeath	= CMonster_Titan_Death::Create(m_pGraphic_Device, m_pMonster);
	CMonster_Data* pMonsterFlinch	= CMonster_Titan_Flinch::Create(m_pGraphic_Device, m_pMonster);

	if (nullptr == pMonsterIdle		|| nullptr == pMonsterSpawn ||
		nullptr == pMonsterAttack	|| nullptr == pMonsterMove	||
		nullptr == pMonsterDeath	|| nullptr == pMonsterFlinch)
	{
		return E_FAIL;
	}

	m_mapMonsterState.emplace(CMonsterState::IDLE	, pMonsterIdle);
	m_mapMonsterState.emplace(CMonsterState::SPAWN	, pMonsterSpawn);
	m_mapMonsterState.emplace(CMonsterState::MOVE	, pMonsterMove);
	m_mapMonsterState.emplace(CMonsterState::ATTACK	, pMonsterAttack);
	m_mapMonsterState.emplace(CMonsterState::DEATH	, pMonsterDeath);
	m_mapMonsterState.emplace(CMonsterState::HURT	, pMonsterFlinch);

	return S_OK;
}

CMonsterState * CMonsterState::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CMonsterState::MONSTERTYPE eType, CMonster* pObject)
{
	CMonsterState* pInstance = new CMonsterState(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(eType,pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonsterState");
	}
	return pInstance;
}

void CMonsterState::Free()
{
	__super::Free();

	for (auto& Pair : m_mapMonsterState)
	{
		Safe_Release(Pair.second);
	}
	m_mapMonsterState.clear();
}
