#include "stdafx.h"
#include "..\public\Monster_Beetle_Spawn.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Beetle.h"


CMonster_Beetle_Spawn::CMonster_Beetle_Spawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Beetle_Spawn::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Beetle_Spawn::HandleInput()
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		return CMonsterState::MOTION::IDLE;
	}
	return CMonsterState::MOTION::SPAWN;
}

void CMonster_Beetle_Spawn::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CBeetle::ANIMATION_KEY::SPAWN);
}

void CMonster_Beetle_Spawn::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));


	if (nullptr != pPlayerCollider &&
		m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollider))
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(false);
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per_Upper() > 0.05f)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(true);
	}

	RELEASE_INSTANCE(CGameInstacne);

}

void CMonster_Beetle_Spawn::Late_Update(_float TimeDelta)
{
	__super::Late_Update(TimeDelta);
}

void CMonster_Beetle_Spawn::Exit()
{
}

CMonster_Beetle_Spawn * CMonster_Beetle_Spawn::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Beetle_Spawn*	pInstance = new CMonster_Beetle_Spawn(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Spawn");
	}
	return pInstance;
}

void CMonster_Beetle_Spawn::Free()
{
	__super::Free();
}
