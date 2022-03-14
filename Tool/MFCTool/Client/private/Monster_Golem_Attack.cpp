#include "stdafx.h"
#include "Monster_Golem_Attack.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Golem.h"

CMonster_Golem_Attack::CMonster_Golem_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Golem_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Golem_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	
	if ((m_fSkil0 > 0.f && m_fSkil1 > 0.f))
	{
		m_fIdleTime = 5.f;
		goto Idle;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Death:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::DEATH;
Flinch:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::HURT;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
}

void CMonster_Golem_Attack::Enter()
{
	m_fIdleTime = 0.f;
}

void CMonster_Golem_Attack::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	if (nullptr == pTransform)
	{
		RELEASE_INSTANCE(CGameInstacne);
		return;
	}

	_float	fDistance = D3DXVec3Length(&(m_pMonster->Get_TransformCom()->Get_MatrixRow(CTransform::STATE::STATE_POSITION) - pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION)));

	if (fDistance < 20.f && m_fSkil0 <= 0.f && false == m_isFire )
	{
		m_fSkil0 = 5.f;
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::SMACK, CGolem::ANIMATION_KEY::SMACK);
		m_isSmash = true;
	}
	else if (fDistance >= 20.f && m_fSkil1 <= 0.f && false == m_isSmash)
	{
		m_fSkil1 = 9.f;
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::FIRELASER, CGolem::ANIMATION_KEY::FIRELASER);
		m_isFire = true;
	}


	if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() > 0.72f &&
		CGolem::ANIMATION_KEY::FIRELASER == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::IDLE_ADDITIVE, CGolem::ANIMATION_KEY::IDLE_ADDITIVE);
		m_isFire = false;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Lower() > 0.9f &&
		CGolem::ANIMATION_KEY::SMACK == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::IDLE_ADDITIVE, CGolem::ANIMATION_KEY::IDLE_ADDITIVE);
		m_isSmash = false;
	}


	RELEASE_INSTANCE(CGameInstacne);
}

void CMonster_Golem_Attack::Late_Update(_float TimeDelta)
{
}

void CMonster_Golem_Attack::Exit()
{
	m_isFire = false;
	m_isSmash = false;
}

CMonster_Golem_Attack * CMonster_Golem_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Golem_Attack*	pInstance = new CMonster_Golem_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Golem_Attack::Free()
{
	__super::Free();
}
