#include "stdafx.h"
#include "Monster_Titan_Attack.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Titan.h"

CMonster_Titan_Attack::CMonster_Titan_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Titan_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Titan_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*		pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));


	if (!(m_fSkil0 <= 0))
	{
		goto Move;
	}

	if (nullptr == pCollider)
	{
		goto Attack;
	}
	if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pCollider) && false == m_isAttack)
	{
		goto Move;
	}
	if (false == m_isAttack)
	{
		goto Idle;
	}


	if (m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pCollider))
	{
		goto Attack;
	}
	

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

void CMonster_Titan_Attack::Enter()
{
	m_isAttack = true;
	if (m_fSkil0 <= 0)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CTitan::ANIMATION_KEY::PREP_FIST);
	}
}

void CMonster_Titan_Attack::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f &&
		CTitan::ANIMATION_KEY::PREP_FIST == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex() &&
		m_fSkil0 <= 0.f)
	{
		m_fSkil0 = 10.f;
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CTitan::ANIMATION_KEY::RETRACT_FIST);
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f &&
		CTitan::ANIMATION_KEY::RETRACT_FIST == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		m_isAttack = false;
	}
	return;
}

void CMonster_Titan_Attack::Late_Update(_float TimeDelta)
{
}

void CMonster_Titan_Attack::Exit()
{
}

CMonster_Titan_Attack * CMonster_Titan_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Titan_Attack*	pInstance = new CMonster_Titan_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Titan_Attack::Free()
{
	__super::Free();
}
