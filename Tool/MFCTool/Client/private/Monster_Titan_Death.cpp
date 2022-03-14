#include "stdafx.h"
#include "Monster_Titan_Death.h"
#include "GameInstacne.h"
#include "MonsterState.h"
#include "Titan.h"

CMonster_Titan_Death::CMonster_Titan_Death(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Titan_Death::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Titan_Death::HandleInput()
{
	return CMonsterState::MOTION::DEATH;
}

void CMonster_Titan_Death::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CTitan::ANIMATION_KEY::DEATH);
}

void CMonster_Titan_Death::Update(_float TimeDelta)
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(true);
	}

}

void CMonster_Titan_Death::Late_Update(_float TimeDelta)
{
}

void CMonster_Titan_Death::Exit()
{
}

CMonster_Titan_Death * CMonster_Titan_Death::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Titan_Death*	pInstance = new CMonster_Titan_Death(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Titan_Death::Free()
{
	__super::Free();
}
