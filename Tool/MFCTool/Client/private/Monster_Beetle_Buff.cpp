#include "stdafx.h"
#include "Beetle.h"
#include "GameInstacne.h"
#include "MonsterState.h"
#include "..\public\Monster_Beetle_Buff.h"

CMonster_Beetle_Buff::CMonster_Beetle_Buff(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMonster_Data(pGraphic_Device)
{
}

HRESULT CMonster_Beetle_Buff::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Beetle_Buff::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));

	_float3	fDir = pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION) - m_pMonster->Get_TransformCom()->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	_float	fDistance = D3DXVec3Length(&fDir);


	if (m_pMonster->Get_MeshCom()->Play_Animation_Per() > 0.9f &&
		CBeetle::ANIMATION_KEY::BUFFSELF == m_pMonster->Get_MeshCom()->Get_CurrentAnimationIndex())
	{
		if (fDistance <= 10.f && m_fSkil1 <= 0.f)
		{
			goto Attack;
		}
		else if (m_fSkil1 <= 0.f)
		{
			goto Move;
		}
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::BUFF;
Attack:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::ATTACK;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CMonsterState::MOTION::MOVE;
}

void CMonster_Beetle_Buff::Enter()
{
	m_fSkil0 = 10.f;
	m_pMonster->Get_MeshCom()->Set_AnimationIndex(CBeetle::ANIMATION_KEY::BUFFSELF);
}

void CMonster_Beetle_Buff::Update(_float TimeDelta)
{
	__super::Update(TimeDelta);
}

void CMonster_Beetle_Buff::Late_Update(_float TimeDelta)
{
	__super::Late_Update(TimeDelta);
}

void CMonster_Beetle_Buff::Exit()
{
	m_pMonster->Set_isHurt(false);
}

CMonster_Beetle_Buff * CMonster_Beetle_Buff::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pMonster)
{
	CMonster_Beetle_Buff*	pInstance = new CMonster_Beetle_Buff(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pMonster)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CMonster_Beetle_Buff");
	}
	return pInstance;
}

void CMonster_Beetle_Buff::Free()
{
	__super::Free();
}
