#include "stdafx.h"
#include "Monster_Vagrant_Hurt.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Vagrant.h"

CMonster_Vagrant_Hurt::CMonster_Vagrant_Hurt(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Vagrant_Hurt::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Vagrant_Hurt::HandleInput()
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.95f && m_fSkil1 <= 0)
	{
		return CMonsterState::ATTACK;
	}
	else if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.95f)
	{
		return CMonsterState::MOTION::IDLE;
	}
	return CMonsterState::MOTION::HURT;
}

void CMonster_Vagrant_Hurt::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::FLINCH1);
}

void CMonster_Vagrant_Hurt::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);
}

void CMonster_Vagrant_Hurt::Late_Update(_float TimeDelta)
{
}

void CMonster_Vagrant_Hurt::Exit()
{
	m_pMonster->Set_isHurt(false);
}

CMonster_Vagrant_Hurt * CMonster_Vagrant_Hurt::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Vagrant_Hurt*	pInstance = new CMonster_Vagrant_Hurt(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Vagrant_Hurt");
	}
	return pInstance;
}

void CMonster_Vagrant_Hurt::Free()
{
	__super::Free();
}
