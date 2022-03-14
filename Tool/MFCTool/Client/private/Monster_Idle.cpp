#include "stdafx.h"
#include "..\public\Monster_Idle.h"
#include "MonsterState.h"

CMonster_Idle::CMonster_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Idle::NativeConstruct(CGameObject * pObject)
{
	return E_NOTIMPL;
}

_uint CMonster_Idle::HandleInput()
{
	return CMonsterState::MOTION::IDLE;
}

void CMonster_Idle::Enter()
{
}

void CMonster_Idle::Update(_float TimeDelta)
{
}

void CMonster_Idle::Late_Update(_float TimeDelta)
{
}

void CMonster_Idle::Exit()
{
}

CMonster_Idle * CMonster_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	return nullptr;
}

void CMonster_Idle::Free()
{
}
