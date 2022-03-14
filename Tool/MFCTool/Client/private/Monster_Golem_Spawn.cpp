#include "stdafx.h"
#include "Monster_Golem_Spawn.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Golem.h"

CMonster_Golem_Spawn::CMonster_Golem_Spawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Golem_Spawn::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Golem_Spawn::HandleInput()
{
	if (CGolem::ANIMATION_KEY::SPAWN == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex_Upper() &&
		m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() > 0.9f)
	{
		return CMonsterState::MOTION::IDLE;
	}

	if (true == m_pMonster->Get_isHurt())
	{
		goto Hurt;
	}

	return CMonsterState::MOTION::SPAWN;
Hurt:
	return CMonsterState::MOTION::HURT;
}

void CMonster_Golem_Spawn::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::SPAWN, CGolem::ANIMATION_KEY::SPAWN);
}

void CMonster_Golem_Spawn::Update(_float TimeDelta)
{
	//m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::SPAWN, CGolem::ANIMATION_KEY::SPAWN);
}

void CMonster_Golem_Spawn::Late_Update(_float TimeDelta)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));


	if (nullptr != pPlayerCollider &&
		m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollider))
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(false);
	}
	else if(m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() > 0.15f)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(true);
	}

	RELEASE_INSTANCE(CGameInstacne);

}

void CMonster_Golem_Spawn::Exit()
{
	m_pMonster->Get_MeshCom()->Set_AnimationStop(false);
}

CMonster_Golem_Spawn * CMonster_Golem_Spawn::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Golem_Spawn*	pInstance = new CMonster_Golem_Spawn(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Golem_Spawn::Free()
{
	__super::Free();
}
