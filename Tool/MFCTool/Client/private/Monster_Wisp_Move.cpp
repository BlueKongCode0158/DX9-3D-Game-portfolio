#include "stdafx.h"
#include "Monster_Wisp_Move.h"
#include "MonsterState.h"
#include "GameInstacne.h"

CMonster_Wisp_Move::CMonster_Wisp_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{

}

HRESULT CMonster_Wisp_Move::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Wisp_Move::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
	
	if (nullptr != pPlayerCollider)
	{
		if (m_pMonster->Get_RecognitionCom()->Collision_SPHERE(pPlayerCollider) && (false == m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pPlayerCollider)))
		{
			m_isActive = true;
			RELEASE_INSTANCE(CGameInstacne);
			return CMonsterState::MOTION::MOVE;
		}
		
		if (m_pMonster->Get_AttackColliderCom()->Collision_SPHERE(pPlayerCollider))
		{
			m_isActive = false;
			RELEASE_INSTANCE(CGameInstacne);
			return CMonsterState::MOTION::ATTACK;
		}
		else
		{
			m_isActive = false;
		}
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
}

void CMonster_Wisp_Move::Enter()
{
	m_pMonster->Set_isHurt(false);
}

void CMonster_Wisp_Move::Update(_float TimeDelta)
{


	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	if (nullptr != pPlayerTransform && true == m_isActive)
	{
		m_pMonster->Get_TransformCom()->TargetToMove(TimeDelta*2.f, pPlayerTransform);
	}
	else if (false == m_isActive)
	{
		m_fAccTime += TimeDelta;

		if (m_fAccTime >= 4.f)
		{
			_uint iRand = rand() & STATE_END;
			m_eMove = static_cast<RANDOM_MOVE>(iRand);
			m_fAccTime = 0.f;
		}

		Random_Move(m_eMove, TimeDelta);
	}



	RELEASE_INSTANCE(CGameInstacne);
}

void CMonster_Wisp_Move::Late_Update(_float TimeDelta)
{
}

void CMonster_Wisp_Move::Exit()
{
}

void CMonster_Wisp_Move::Random_Move(RANDOM_MOVE eType, _float TimeDelta)
{
	if (eType == STATE0)
	{
		m_pMonster->Get_TransformCom()->Walk_Look(TimeDelta);
	}
	else if (eType == STATE1)
	{
		m_pMonster->Get_TransformCom()->Walk_Look(TimeDelta);
		m_pMonster->Get_TransformCom()->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta);
	}
	else if (eType == STATE2)
	{
		m_pMonster->Get_TransformCom()->Walk_Look(TimeDelta);
		m_pMonster->Get_TransformCom()->Rotation_Axis(_float3(0.f, 1.f, 0.f), -TimeDelta);
	}
	else if (eType == STATE3)
	{
		m_pMonster->Get_TransformCom()->Walk_Look(-TimeDelta);
		m_pMonster->Get_TransformCom()->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta);
	}
}

CMonster_Wisp_Move * CMonster_Wisp_Move::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Wisp_Move*	pInstance = new CMonster_Wisp_Move(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Wisp_Move::Free()
{
	__super::Free();
}
