#include "stdafx.h"
#include "..\public\Monster_Beetle_Idle.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Beetle.h"

CMonster_Beetle_Idle::CMonster_Beetle_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Beetle_Idle::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Beetle_Idle::HandleInput()
{
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*		pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (nullptr != pPlayerCollider)
	{
		if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollider) && true == m_isRecog)
		{
			goto Move;
		}

		if (m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pPlayerCollider) && false == m_isRecog)
		{
			m_isRecog = true;
			goto Move;
		}
	}
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
}

void CMonster_Beetle_Idle::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CBeetle::ANIMATION_KEY::IDLEADDITIVE);
}

void CMonster_Beetle_Idle::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.95f)
	{
		m_pMonster->Get_MeshCom()->Set_FrameAnimation(0.01f);
	}
	m_fIdleTime -= TimeDelta;

	if (m_fIdleTime < 0.f)
	{
		m_fIdleTime = 0.f;
	}

}

void CMonster_Beetle_Idle::Late_Update(_float TimeDelta)
{
	__super::Late_Update(TimeDelta);
}

void CMonster_Beetle_Idle::Exit()
{
}

CMonster_Beetle_Idle * CMonster_Beetle_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Beetle_Idle*	pInstance = new CMonster_Beetle_Idle(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Beetle_Idle::Free()
{
	__super::Free();
}
