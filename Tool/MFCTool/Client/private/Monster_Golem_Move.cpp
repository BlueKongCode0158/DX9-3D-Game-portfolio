#include "stdafx.h"
#include "Monster_Golem_Move.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Golem.h"

CMonster_Golem_Move::CMonster_Golem_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Golem_Move::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Golem_Move::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pPlayerCollider))
	{
		goto Attack;
	}

	if(true == m_pMonster->Get_isHurt())
	{
		goto Hurt;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Death:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::DEATH;
Hurt:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::HURT;
}

void CMonster_Golem_Move::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::RUNFORWARD_RM, CGolem::ANIMATION_KEY::RUNFORWARD_RM);
}

void CMonster_Golem_Move::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = reinterpret_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() <= 0.4f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() >= 0.f &&
		m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() <= 0.4f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() >= 0.f)
	{
		m_fSpeed = 1.f;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() <= 0.5f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() >= 0.4f &&
			 m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() <= 0.5f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() >= 0.4f)
	{
		if (m_fSpeed != 0.f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::MONSTER);
			CSound_Manager::Get_Instance()->PlaySound(L"golem_step_01.ogg", CSound_Manager::CHANNELID::MONSTER);
			CSound_Manager::Get_Instance()->SetVolum(CSound_Manager::CHANNELID::MONSTER, 0.3f);
			m_fSpeed = 0.f;
		}
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() <= 0.9f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() >= 0.5f &&
			 m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() <= 0.9f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() >= 0.5f)
	{
		m_fSpeed = 1.f;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() <= 1.f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() >= 0.9f &&
			 m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() <= 1.f && m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() >= 0.9f)
	{
		if (m_fSpeed != 0.f)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::MONSTER);
			CSound_Manager::Get_Instance()->PlaySound(L"golem_step_01.ogg", CSound_Manager::CHANNELID::MONSTER);
			CSound_Manager::Get_Instance()->SetVolum(CSound_Manager::CHANNELID::MONSTER, 0.3f);
			m_fSpeed = 0.f;
		}
	}

	if (m_pMonster->Get_MeshCom()->Animation_State_End_Lower(CGolem::ANIMATION_KEY::RUNFORWARD_RM))
	{
		m_pMonster->Get_MeshCom()->Reset_FrameAnimation_Lower();
		m_pMonster->Get_MeshCom()->Reset_FrameAnimation_Upper();
	}



	m_pMonster->Get_TransformCom()->TargetToMove(m_fSpeed*TimeDelta, pTransform);
	m_pMonster->Get_TransformCom()->Walk_Look(m_fSpeed * TimeDelta);

	RELEASE_INSTANCE(CGameInstacne);
}

void CMonster_Golem_Move::Late_Update(_float TimeDelta)
{

}

void CMonster_Golem_Move::Exit()
{
}

CMonster_Golem_Move * CMonster_Golem_Move::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Golem_Move*	pInstance = new CMonster_Golem_Move(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Golem_Move::Free()
{
	__super::Free();
}
