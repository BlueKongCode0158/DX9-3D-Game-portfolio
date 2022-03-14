#include "stdafx.h"
#include "Monster_Golem_Death.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Golem.h"

CMonster_Golem_Death::CMonster_Golem_Death(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Golem_Death::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Golem_Death::HandleInput()
{
	return CMonsterState::MOTION::DEATH;
}

void CMonster_Golem_Death::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::DEATH, CGolem::ANIMATION_KEY::DEATH);
}

void CMonster_Golem_Death::Update(_float TimeDelta)
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(true);
	}
}

void CMonster_Golem_Death::Late_Update(_float TimeDelta)
{
}

void CMonster_Golem_Death::Exit()
{
}

CMonster_Golem_Death * CMonster_Golem_Death::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Golem_Death*	pInstance = new CMonster_Golem_Death(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Golem_Death::Free()
{
	__super::Free();
}
