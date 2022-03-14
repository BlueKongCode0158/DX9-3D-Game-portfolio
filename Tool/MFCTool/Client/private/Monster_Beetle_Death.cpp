#include "stdafx.h"
#include "..\public\Monster_Beetle_Death.h"
#include "MonsterState.h"
#include "GameInstacne.h"
#include "Beetle.h"


CMonster_Beetle_Death::CMonster_Beetle_Death(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Beetle_Death::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Beetle_Death::HandleInput()
{
	return CMonsterState::MOTION::DEATH;
}

void CMonster_Beetle_Death::Enter()
{
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CBeetle::ANIMATION_KEY::DEATH);
}

void CMonster_Beetle_Death::Update(_float TimeDelta)
{
	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		m_pMonster->Get_MeshCom()->Set_AnimationStop(true);
	}
}

void CMonster_Beetle_Death::Late_Update(_float TimeDelta)
{
}

void CMonster_Beetle_Death::Exit()
{
}

CMonster_Beetle_Death * CMonster_Beetle_Death::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Beetle_Death*	pInstance = new CMonster_Beetle_Death(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Death");
	}
	return pInstance;
}

void CMonster_Beetle_Death::Free()
{
	__super::Free();
}
