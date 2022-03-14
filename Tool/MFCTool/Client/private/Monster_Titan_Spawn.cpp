#include "stdafx.h"
#include "Monster_Titan_Spawn.h"
#include "MonsterState.h"
#include "Monster_Titan_Spawn.h"
#include "GameInstacne.h"
#include "Titan.h"

CMonster_Titan_Spawn::CMonster_Titan_Spawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Titan_Spawn::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Titan_Spawn::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		goto Idle;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::SPAWN;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
}

void CMonster_Titan_Spawn::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CTitan::ANIMATION_KEY::SPAWN);
	m_pMonster->Get_MeshCom()->Set_AnimationStop(false);
}

void CMonster_Titan_Spawn::Update(_float TimeDelta)
{
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*		pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.1f &&
		m_pMonster->Get_MeshCom()->Play_Animation_Per() < 0.105f)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(true);
	}


	if (nullptr != pCollider)
	{
		if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pCollider) && 
			pGameInstance->Input_KeyBoard_Down(DIK_E))
		{
			m_pMonster->Get_MeshCom()->Set_AnimationStop(false);
		}
	}
	RELEASE_INSTANCE(CGameInstacne);
}

void CMonster_Titan_Spawn::Late_Update(_float TimeDelta)
{
}

void CMonster_Titan_Spawn::Exit()
{
}

CMonster_Titan_Spawn * CMonster_Titan_Spawn::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Titan_Spawn*	pInstance = new CMonster_Titan_Spawn(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Titan_Spawn");
	}
	return pInstance;
}

void CMonster_Titan_Spawn::Free()
{
	__super::Free();
}
