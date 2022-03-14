#include "stdafx.h"
#include "Monster_Vagrant_Attack.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Vagrant.h"

CMonster_Vagrant_Attack::CMonster_Vagrant_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Vagrant_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Vagrant_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*	pPlayerCollsion = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider")));

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f && CVagrant::ANIMATION_KEY::CHARGETRACKING_BOMB == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		m_fSkil1 = 10.f;
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::FIRETRACKING_BOMB);
	}
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f && CVagrant::ANIMATION_KEY::FIRETRACKING_BOMB == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CMonsterState::MOTION::IDLE;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
}

void CMonster_Vagrant_Attack::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::CHARGETRACKING_BOMB);
}

void CMonster_Vagrant_Attack::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

}

void CMonster_Vagrant_Attack::Late_Update(_float TimeDelta)
{
}

void CMonster_Vagrant_Attack::Exit()
{
}

CMonster_Vagrant_Attack * CMonster_Vagrant_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Vagrant_Attack*	pInstance = new CMonster_Vagrant_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Vagrant_Attack");
	}
	return pInstance;
}

void CMonster_Vagrant_Attack::Free()
{
	__super::Free();
}
