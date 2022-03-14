#include "stdafx.h"
#include "Monster_Wisp_Death.h"
#include "MonsterState.h"

CMonster_Wisp_Death::CMonster_Wisp_Death(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Wisp_Death::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Wisp_Death::HandleInput()
{
	return CMonsterState::MOTION::MOVE;
}

void CMonster_Wisp_Death::Enter()
{
}

void CMonster_Wisp_Death::Update(_float TimeDelta)
{
}

void CMonster_Wisp_Death::Late_Update(_float TimeDelta)
{
}

void CMonster_Wisp_Death::Exit()
{
}

CMonster_Wisp_Death * CMonster_Wisp_Death::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Wisp_Death*	pInstance = new CMonster_Wisp_Death(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Wisp_Death::Free()
{
	__super::Free();
}
