#include "stdafx.h"
#include "Monster_Wisp_Spawn.h"
#include "MonsterState.h"

CMonster_Wisp_Spawn::CMonster_Wisp_Spawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Wisp_Spawn::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Wisp_Spawn::HandleInput()
{
	return CMonsterState::MOTION::IDLE;
}

void CMonster_Wisp_Spawn::Enter()
{
	m_pMonster->Set_isHurt(false);
}

void CMonster_Wisp_Spawn::Update(_float TimeDelta)
{
}

void CMonster_Wisp_Spawn::Late_Update(_float TimeDelta)
{
}

void CMonster_Wisp_Spawn::Exit()
{
}

CMonster_Wisp_Spawn * CMonster_Wisp_Spawn::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Wisp_Spawn*	pInstance = new CMonster_Wisp_Spawn(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Wisp_Spawn::Free()
{
	__super::Free();
}
