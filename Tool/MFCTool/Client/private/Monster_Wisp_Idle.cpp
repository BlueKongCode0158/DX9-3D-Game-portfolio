#include "stdafx.h"
#include "Monster_Wisp_Idle.h"
#include "MonsterState.h"
#include "GameInstacne.h"

CMonster_Wisp_Idle::CMonster_Wisp_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Wisp_Idle::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Wisp_Idle::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
	
	if (nullptr != pPlayerCollider)
	{
		if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollider))
		{
			RELEASE_INSTANCE(CGameInstacne);
			return CMonsterState::MOTION::MOVE;
		}

	}
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
}

void CMonster_Wisp_Idle::Enter()
{
	m_pMonster->Set_isHurt(false);
}

void CMonster_Wisp_Idle::Update(_float TimeDelta)
{
}

void CMonster_Wisp_Idle::Late_Update(_float TimeDelta)
{
}

void CMonster_Wisp_Idle::Exit()
{
}

CMonster_Wisp_Idle * CMonster_Wisp_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Wisp_Idle*	pInstance = new CMonster_Wisp_Idle(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Wisp_Idle::Free()
{
	__super::Free();
}
