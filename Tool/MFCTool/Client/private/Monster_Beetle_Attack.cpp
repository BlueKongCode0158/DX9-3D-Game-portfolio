#include "stdafx.h"
#include "MonsterState.h"
#include "..\public\Monster_Beetle_Attack.h"
#include "GameInstacne.h"
#include "Beetle.h"

CMonster_Beetle_Attack::CMonster_Beetle_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Beetle_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Beetle_Attack::HandleInput()
{
	CGameInstacne*	pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider*		pCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));

	if (m_fSkil0 > 0.f && false == m_isAttack)
	{
		goto Idle;
	}


	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::IDLE;
}

void CMonster_Beetle_Attack::Enter()
{
	m_fSkil0 = 5.f;
	m_isAttack = true;
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CBeetle::ANIMATION_KEY::GROUNDSMASH);
}

void CMonster_Beetle_Attack::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);

	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f)
	{
		m_isAttack = false;
	}

}

void CMonster_Beetle_Attack::Late_Update(_float TimeDelta)
{
	__super::Late_Update(TimeDelta);
}

void CMonster_Beetle_Attack::Exit()
{
	m_pMonster->Set_isHurt(false);
}

CMonster_Beetle_Attack * CMonster_Beetle_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Beetle_Attack*	pInstance = new CMonster_Beetle_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Beetle_Attack::Free()
{
	__super::Free();
}
