#include "stdafx.h"
#include "Player.h"
#include "GameInstacne.h"
#include "..\public\PlayerState.h"
#include "PlayerData.h"
#include "Player_Jump.h"
#include "Player_Move.h"
#include "Player_Idle.h"
#include "Player_Death.h"
#include "Player_Attack.h"
#include "Player_MoveAttack.h"
#include "Player_Jump_Attack.h"

CPlayerState::CPlayerState(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CState(pGraphic_Device)
{

}

HRESULT CPlayerState::NativeConstruct(CGameObject* pObject)
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

	CPlayerData* pJump = CPlayer_Jump::Create(m_pGraphic_Device, m_pObject);
	CPlayerData* pMove = CPlayer_Move::Create(m_pGraphic_Device, m_pObject);
	CPlayerData* pIdle = CPlayer_Idle::Create(m_pGraphic_Device, m_pObject);
	CPlayerData* pDeath = CPlayer_Death::Create(m_pGraphic_Device, m_pObject);
	CPlayerData* pAttack = CPlayer_Attack::Create(m_pGraphic_Device, m_pObject);
	CPlayerData* pMoveAttack = CPlayer_MoveAttack::Create(m_pGraphic_Device, m_pObject);
	CPlayerData* pJumpAttack = CPlayer_Jump_Attack::Create(m_pGraphic_Device, m_pObject);

	m_mapPlayerState.emplace(CPlayerState::MOTION::IDLE,			pIdle);
	m_mapPlayerState.emplace(CPlayerState::MOTION::DEATH,			pDeath);
	m_mapPlayerState.emplace(CPlayerState::MOTION::ATTACK,			pAttack);
	m_mapPlayerState.emplace(CPlayerState::MOTION::MOVE,			pMove);
	m_mapPlayerState.emplace(CPlayerState::MOTION::MOVE_ATTACK,		pMoveAttack);
	m_mapPlayerState.emplace(CPlayerState::MOTION::JUMP, 			pJump);
	m_mapPlayerState.emplace(CPlayerState::MOTION::JUMP_ATTACK,		pJumpAttack);

	return S_OK;
}

void CPlayerState::ChangeState()
{
	if (m_iNewAnim != m_iCurrentAnim)
	{
		/* 현재 애니메이션에서 모션 Handle 을 가져와준다.*/
		auto& pState = m_mapPlayerState[m_iCurrentAnim];
		if (nullptr == pState)
		{
			return;
		}
		m_iAttackHandle = pState->Get_AttackHandle_Motion();
		pState->Exit();

		m_iJumpHandle = pState->Get_JumpHandle_Motion();

		m_iCurrentAnim = m_iNewAnim;


		/* 바뀐 애니메이션에다가 모션 Handle을 부여해준다.*/
		auto& pState2 = m_mapPlayerState[m_iCurrentAnim];
		if (nullptr == pState)
		{
			return;
		}
		pState2->Set_AttackHandle(m_iAttackHandle);
		pState2->Set_JumpHandle(m_iJumpHandle);

		pState2->Enter();
	}
}


_uint CPlayerState::HandleInput()
{
	if (nullptr == m_mapPlayerState[m_iCurrentAnim])
	{
		return 0;
	}
	m_iNewAnim = static_cast<MOTION>(m_mapPlayerState[m_iCurrentAnim]->HandleInput());

	ChangeState();

	return 0;
}
void CPlayerState::Enter()
{
	m_iCurrentAnim = CPlayerState::MOTION::IDLE;

	return;
}

void CPlayerState::Update(_float TimeDelta)
{
	if (nullptr == m_mapPlayerState[m_iCurrentAnim])
	{
		return;
	}

	m_mapPlayerState[m_iCurrentAnim]->Update(TimeDelta);

	return;
}

void CPlayerState::Exit()
{
	if (nullptr == m_mapPlayerState[m_iCurrentAnim])
	{
		return;
	}
	m_mapPlayerState[m_iCurrentAnim]->Exit();
	return;
}

CPlayerState * CPlayerState::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject)
{
	CPlayerState* pInstance = new CPlayerState(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayerState");
	}
	pInstance->Enter();
	return pInstance;
}

void CPlayerState::Free()
{
	__super::Free();

	for (auto& Pair : m_mapPlayerState)
	{
		Safe_Release(Pair.second);
	}
	m_mapPlayerState.clear();

}
