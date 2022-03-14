#include "stdafx.h"
#include "Monster_Vagrant_Area_Attack.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Vagrant.h"

CMonster_Vagrant_Area_Attack::CMonster_Vagrant_Area_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Vagrant_Area_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Vagrant_Area_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*	pPlayerCollsion = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider")));

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.95f && CVagrant::ANIMATION_KEY::PRE_NOVA == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		m_fSkil0 = 20.f;
		m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::POST_NOVA);
	}
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f && CVagrant::ANIMATION_KEY::POST_NOVA == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CMonsterState::MOTION::IDLE;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::AREA_ATTACK;
}

void CMonster_Vagrant_Area_Attack::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::PRE_NOVA);
}

void CMonster_Vagrant_Area_Attack::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	_float fRadius = (m_pMonster->Get_MeshCom()->Play_Animation_Per() * 150.f) * 2.f;

	if (CVagrant::ANIMATION_KEY::POST_NOVA == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex()
		&& fRadius != 0.f)
	{
		m_pMonster->Get_ColliderArea_Com()->Update_ColliderScale(_float3(fRadius, fRadius, fRadius));
	}
}

void CMonster_Vagrant_Area_Attack::Late_Update(_float TimeDelta)
{

}

void CMonster_Vagrant_Area_Attack::Exit()
{
}

CMonster_Vagrant_Area_Attack * CMonster_Vagrant_Area_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Vagrant_Area_Attack*	pInstance = new CMonster_Vagrant_Area_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Vagrant_Area_Attack");
	}
	return pInstance;
}

void CMonster_Vagrant_Area_Attack::Free()
{
	__super::Free();
}
