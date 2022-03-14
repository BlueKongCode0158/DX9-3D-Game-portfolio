#include "stdafx.h"
#include "Monster_Vagrant_Spawn.h"
#include "GameInstacne.h"
#include "MonsterState.h"
#include "Monster.h"
#include "Vagrant.h"

CMonster_Vagrant_Spawn::CMonster_Vagrant_Spawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Vagrant_Spawn::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Vagrant_Spawn::HandleInput()
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		return CMonsterState::MOTION::IDLE;
	}

	return CMonsterState::MOTION::SPAWN;
}

void CMonster_Vagrant_Spawn::Enter()
{
	if (nullptr == m_pMonster)
	{
		return;
	}
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CVagrant::ANIMATION_KEY::SPAWN);

}

void CMonster_Vagrant_Spawn::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

}

void CMonster_Vagrant_Spawn::Late_Update(_float TimeDelta)
{

}

void CMonster_Vagrant_Spawn::Exit()
{

}

CMonster_Vagrant_Spawn * CMonster_Vagrant_Spawn::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Vagrant_Spawn*	pInstance = new CMonster_Vagrant_Spawn(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Vagrant_Spawn");
	}
	return pInstance;
}

void CMonster_Vagrant_Spawn::Free()
{
	__super::Free();
}
