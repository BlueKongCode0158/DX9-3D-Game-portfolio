#include "stdafx.h"
#include "Monster_Wisp_Attack.h"
#include "MonsterState.h"
#include "GameInstacne.h"

CMonster_Wisp_Attack::CMonster_Wisp_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Wisp_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Wisp_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
	
	if (false == m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pPlayerCollider))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CMonsterState::MOTION::MOVE;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
}

void CMonster_Wisp_Attack::Enter()
{
	m_pMonster->Set_isHurt(false);
}

void CMonster_Wisp_Attack::Update(_float TimeDelta)
{

}

void CMonster_Wisp_Attack::Late_Update(_float TimeDelta)
{

}

void CMonster_Wisp_Attack::Exit()
{

}

CMonster_Wisp_Attack * CMonster_Wisp_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Wisp_Attack*	pInstance = new CMonster_Wisp_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Wisp_Attack::Free()
{
	__super::Free();
}
