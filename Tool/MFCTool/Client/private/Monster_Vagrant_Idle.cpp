#include "stdafx.h"
#include "Monster_Vagrant_Idle.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Vagrant.h"

CMonster_Vagrant_Idle::CMonster_Vagrant_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Vagrant_Idle::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Vagrant_Idle::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*	pPlayerCollsion = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider")));

	if (nullptr != pPlayerCollsion)
	{
		if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollsion) && m_fSkil0 <= 0)
		{
			goto AreaAttack;
		}
	}
	if (true == m_pMonster->Get_isHurt())
	{
		goto Hurt;
	}

	//goto Hurt;


	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
AreaAttack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::AREA_ATTACK;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Hurt:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::HURT;
}

void CMonster_Vagrant_Idle::Enter()
{
	m_pMonster->Get_ColliderArea_Com()->Reset_ColliderScalse();
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::IDLE_HULL_ADDITIVE);
}

void CMonster_Vagrant_Idle::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);
}

void CMonster_Vagrant_Idle::Late_Update(_float TimeDelta)
{
}

void CMonster_Vagrant_Idle::Exit()
{
}

CMonster_Vagrant_Idle * CMonster_Vagrant_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Vagrant_Idle*	pInstance = new CMonster_Vagrant_Idle(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Vagrant_Idle");
	}
	return pInstance;
}

void CMonster_Vagrant_Idle::Free()
{
	__super::Free();
}
