#include "stdafx.h"
#include "..\public\Player_Move.h"
#include "GameInstacne.h"
#include "PlayerState.h"

CPlayer_Move::CPlayer_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{
}

HRESULT CPlayer_Move::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_Move::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (true == m_isRoll)
	{
		if (pGameInstance->Input_KeyBoard_Down(DIK_LSHIFT) &&
			pGameInstance->Input_KeyBoard_Pressing(DIK_W))
		{
			m_isWalk = false;
			m_isRoll = false;

			m_pObject->Set_PlayerSpeed(4.f);
			m_pObject->Set_PlayerAnimationSpeed(2.f, 2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::ROLL_FORWARD, CPlayer::ANIMATION_KEY::ROLL_FORWARD);
		}
		if (pGameInstance->Input_KeyBoard_Down(DIK_LSHIFT) &&
			pGameInstance->Input_KeyBoard_Pressing(DIK_A))
		{
			m_isWalk = false;
			m_isRoll = false;

			m_pObject->Set_PlayerSpeed(4.f);
			m_pObject->Set_PlayerAnimationSpeed(2.f, 2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::ROLL_LEFT, CPlayer::ANIMATION_KEY::ROLL_LEFT);
		}
		if (pGameInstance->Input_KeyBoard_Down(DIK_LSHIFT) &&
			pGameInstance->Input_KeyBoard_Pressing(DIK_S))
		{
			m_isWalk = false;
			m_isRoll = false;

			m_pObject->Set_PlayerSpeed(4.f);
			m_pObject->Set_PlayerAnimationSpeed(2.f, 2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::ROLL_BACKWARD, CPlayer::ANIMATION_KEY::ROLL_BACKWARD);
		}
		if (pGameInstance->Input_KeyBoard_Down(DIK_LSHIFT) &&
			pGameInstance->Input_KeyBoard_Pressing(DIK_D))
		{
			m_isWalk = false;
			m_isRoll = false;

			m_pObject->Set_PlayerSpeed(4.f);
			m_pObject->Set_PlayerAnimationSpeed(2.f, 2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::ROLL_RIGHT, CPlayer::ANIMATION_KEY::ROLL_RIGHT);
		}
	}

	if (true == m_isWalk)
	{
		if (pGameInstance->Input_KeyBoard_Down(DIK_W))
		{
			m_pObject->Set_PlayerSpeed(2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_FOWARD, CPlayer::ANIMATION_KEY::RUN_FOWARD);
		}
		if (pGameInstance->Input_KeyBoard_Down(DIK_A))
		{
			m_pObject->Set_PlayerSpeed(2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_LEFT, CPlayer::ANIMATION_KEY::RUN_LEFT);
		}
		if (pGameInstance->Input_KeyBoard_Down(DIK_S))
		{
			m_pObject->Set_PlayerSpeed(2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_BACKWARD, CPlayer::ANIMATION_KEY::RUN_BACKWARD);
		}
		if (pGameInstance->Input_KeyBoard_Down(DIK_D))
		{
			m_pObject->Set_PlayerSpeed(2.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_RIGHT, CPlayer::ANIMATION_KEY::RUN_RIGHT);
		}

		if (pGameInstance->Input_KeyBoard_Down(DIK_LCONTROL) && CPlayer::ANIMATION_KEY::RUN_FOWARD  == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex())
		{
			m_pObject->Set_PlayerSpeed(4.f);
			m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::SPRINT_FORWARD, CPlayer::ANIMATION_KEY::SPRINT_FORWARD);
		}
	}

	if (pGameInstance->Input_KeyBoard_Up(DIK_W) || 
		pGameInstance->Input_KeyBoard_Up(DIK_A)	||
		pGameInstance->Input_KeyBoard_Up(DIK_S) ||
		pGameInstance->Input_KeyBoard_Up(DIK_D))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::IDLE;
	}

	if (pGameInstance->Input_KeyMouse_Down(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE_ATTACK;
	}

	if ((m_pObject->Get_MeshCom()->Play_Animation_Per() > 0.8f &&
		CPlayer::ANIMATION_KEY::ROLL_BACKWARD == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower()) ||
		(m_pObject->Get_MeshCom()->Play_Animation_Per() > 0.8f &&
		CPlayer::ANIMATION_KEY::ROLL_FORWARD == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower()) ||
		(m_pObject->Get_MeshCom()->Play_Animation_Per() > 0.8f &&
		CPlayer::ANIMATION_KEY::ROLL_RIGHT == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower()) ||
		(m_pObject->Get_MeshCom()->Play_Animation_Per() > 0.8f &&
		CPlayer::ANIMATION_KEY::ROLL_LEFT == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower())
		)
	{
		m_isRoll = false;
		m_isWalk = true;
		m_fTimeAcc = 0.f;
		m_pObject->Set_PlayerSpeed(2.f);

		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::IDLE;
	}

	if (pGameInstance->Input_KeyBoard_Down(DIK_SPACE))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::JUMP;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::MOVE;
}

void CPlayer_Move::Enter()
{
	m_isRoll = true;
	m_pObject->Set_PlayerSpeed(2.f);
	m_pObject->Set_PlayerAnimationSpeed(1.f, 1.f);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_FOWARD, CPlayer::ANIMATION_KEY::RUN_FOWARD);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_LEFT, CPlayer::ANIMATION_KEY::RUN_LEFT);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_BACKWARD, CPlayer::ANIMATION_KEY::RUN_BACKWARD);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_RIGHT, CPlayer::ANIMATION_KEY::RUN_RIGHT);
	}
	RELEASE_INSTANCE(CGameInstacne)
}

void CPlayer_Move::Update(_float TimeDelta)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (false == m_isRoll)
	{
		m_fTimeAcc += TimeDelta;

		if (m_fTimeAcc >= m_fRollTimeAcc)
		{
			m_isRoll = true;
		}
	}

	RELEASE_INSTANCE(CGameInstacne);
	return;
}

void CPlayer_Move::Exit()
{
	m_isRoll = true;
	m_isWalk = true;

	m_iJump_Handle = m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower();
}

CPlayer_Move * CPlayer_Move::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pGameObject)
{
	CPlayer_Move* pInstance = new CPlayer_Move(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pGameObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Move");
	}
	pInstance->Enter();

	return pInstance;
}

void CPlayer_Move::Free()
{
	__super::Free();

}
