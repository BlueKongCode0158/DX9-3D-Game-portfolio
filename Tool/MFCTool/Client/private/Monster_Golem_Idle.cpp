#include "stdafx.h"
#include "Monster_Golem_Idle.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Golem.h"

CMonster_Golem_Idle::CMonster_Golem_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Golem_Idle::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Golem_Idle::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (nullptr != pPlayerCollider &&
		m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollider) && false == m_isRecog)
	{
		m_isRecog = true;
		goto Move;
	}

	if (true == m_pMonster->Get_isHurt())
	{
		goto Hurt;
	}

	if (true == m_isRecog  && m_fIdleTime <= 0.f)
	{
		goto Move;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Hurt:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::HURT;
}

void CMonster_Golem_Idle::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::IDLE_ADDITIVE, CGolem::ANIMATION_KEY::IDLE_ADDITIVE);
}

void CMonster_Golem_Idle::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.95f)
	{
		m_pMonster->Get_MeshCom()->Set_FrameAnimation_Lower(0.03f);
		m_pMonster->Get_MeshCom()->Set_FrameAnimation_Upper(0.03f);
	}
	m_fIdleTime -= TimeDelta;

	if (m_fIdleTime < 0.f)
	{
		m_fIdleTime = 0.f;
	}
}

void CMonster_Golem_Idle::Late_Update(_float TimeDelta)
{
}

void CMonster_Golem_Idle::Exit()
{
}

CMonster_Golem_Idle * CMonster_Golem_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Golem_Idle*	pInstance = new CMonster_Golem_Idle(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Golem_Idle::Free()
{
	__super::Free();
}
