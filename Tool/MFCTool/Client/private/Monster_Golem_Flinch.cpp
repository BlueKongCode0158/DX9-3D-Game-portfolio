#include "stdafx.h"
#include "Monster_Golem_Flinch.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Golem.h"

CMonster_Golem_Flinch::CMonster_Golem_Flinch(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Golem_Flinch::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Golem_Flinch::HandleInput()
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		m_isRecog = true;
		return CMonsterState::MOTION::MOVE;
	}

	return CMonsterState::MOTION::HURT;
}

void CMonster_Golem_Flinch::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CGolem::ANIMATION_KEY::FLINCH1, CGolem::ANIMATION_KEY::FLINCH1);
}

void CMonster_Golem_Flinch::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);
}

void CMonster_Golem_Flinch::Late_Update(_float TimeDelta)
{
}

void CMonster_Golem_Flinch::Exit()
{
	m_pMonster->Set_isHurt(false);
}

CMonster_Golem_Flinch * CMonster_Golem_Flinch::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Golem_Flinch*	pInstance = new CMonster_Golem_Flinch(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Golem_Flinch");
	}
	return pInstance;
}

void CMonster_Golem_Flinch::Free()
{
	__super::Free();
}
