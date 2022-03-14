#include "stdafx.h"
#include "..\public\PlayerData.h"
#include "GameInstacne.h"
#include "Player_Idle.h"
#include "Player_Move.h"
#include "Player_Death.h"
#include "Player_Attack.h"
#include "Player_MoveAttack.h"


CPlayerData::CPlayerData(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CState(pGraphic_Device)
{
}

HRESULT CPlayerData::NativeConstruct(CGameObject * pObject)
{
	if (nullptr == pObject)
	{
		return E_FAIL;
	}

	m_pObject = dynamic_cast<CPlayer*>(pObject);
	if (nullptr == m_pObject)
	{
		return E_FAIL;
	}
	//Safe_AddRef(m_pObject);

	/* 터졌던 이유 : Player 의 Free가 호출되려면 그 객체의 레퍼런스 카운트가 0이 되었어야 했는데
					 그게 불가능했네. */
	
	return S_OK;
}

_uint CPlayerData::Get_AttackHandle_Motion()
{
	return	m_iAttack_Handle;
}

_uint CPlayerData::Get_JumpHandle_Motion()
{
	return m_iJump_Handle;
}

_uint CPlayerData::HandleInput()
{
	return _uint();
}

void CPlayerData::Enter()
{
}

void CPlayerData::Update(_float TimeDelta)
{
}

void CPlayerData::Exit()
{
}

void CPlayerData::Set_AttackHandle(_uint iAttack_Handle)
{
	m_iAttack_Handle = iAttack_Handle;
}

void CPlayerData::Set_JumpHandle(_uint iJump_Handle)
{
	m_iJump_Handle = iJump_Handle;
}

void CPlayerData::Free()
{
	__super::Free();
}
