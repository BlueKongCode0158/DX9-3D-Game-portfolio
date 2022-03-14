#include "stdafx.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "..\public\Monster_Titan_Laser_Attack.h"


CMonster_Titan_Laser_Attack::CMonster_Titan_Laser_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Titan_Laser_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_uint CMonster_Titan_Laser_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


Laser:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::SIGNATURE_ATTACK;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
}

void CMonster_Titan_Laser_Attack::Enter()
{
}

void CMonster_Titan_Laser_Attack::Update(_float TimeDelta)
{
}

void CMonster_Titan_Laser_Attack::Late_Update(_float TimeDelta)
{
}

void CMonster_Titan_Laser_Attack::Exit()
{
}

CMonster_Titan_Laser_Attack * CMonster_Titan_Laser_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Titan_Laser_Attack*	pInstance = new CMonster_Titan_Laser_Attack(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Titan_Laser_Attack");
	}
	return pInstance;
}

void CMonster_Titan_Laser_Attack::Free()
{
	__super::Free();
}
