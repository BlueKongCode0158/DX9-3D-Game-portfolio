#include "stdafx.h"
#include "..\public\Player_MoveAttack.h"
#include "PlayerState.h"
#include "GameInstacne.h"

CPlayer_MoveAttack::CPlayer_MoveAttack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{
}

HRESULT CPlayer_MoveAttack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_MoveAttack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, CPlayer::ANIMATION_KEY::RUN_FOWARD);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, CPlayer::ANIMATION_KEY::RUN_LEFT);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, CPlayer::ANIMATION_KEY::RUN_BACKWARD);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, CPlayer::ANIMATION_KEY::RUN_RIGHT);
	}

	if ((pGameInstance->Input_KeyBoard_Up(DIK_W) ||
		pGameInstance->Input_KeyBoard_Up(DIK_A)  ||
		pGameInstance->Input_KeyBoard_Up(DIK_S)  ||
		pGameInstance->Input_KeyBoard_Up(DIK_D))  &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RETURN(CPlayerState::MOTION::ATTACK);
	}


	if (pGameInstance->Input_KeyMouse_Up(CInput_Device::DIM::DIM_LBUTTON) &&
		!(pGameInstance->Input_KeyBoard_Pressing(DIK_W) ||
		pGameInstance->Input_KeyBoard_Pressing(DIK_A)	||
		pGameInstance->Input_KeyBoard_Pressing(DIK_S)	||
		pGameInstance->Input_KeyBoard_Pressing(DIK_D)))
	{
		RETURN(CPlayerState::MOTION::MOVE);
	}

	if (pGameInstance->Input_KeyMouse_Up(CInput_Device::DIM::DIM_LBUTTON))
	{
		RETURN(CPlayerState::MOTION::MOVE);
	}

	if (pGameInstance->Input_KeyBoard_Down(DIK_SPACE) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RETURN(CPlayerState::MOTION::JUMP_ATTACK);
	}

	if (pGameInstance->Input_KeyBoard_Down(DIK_SPACE))
	{
		RETURN(CPlayerState::MOTION::JUMP);
	}

	RETURN(CPlayerState::MOTION::MOVE_ATTACK);
}

void CPlayer_MoveAttack::Enter()
{
	m_pObject->Set_PlayerSpeed(2.f);
	m_pObject->Set_PlayerAnimationSpeed(2.f, 1.f);
}

void CPlayer_MoveAttack::Update(_float TimeDelta)
{
	if (m_pObject->Get_MeshCom()->Animation_State_End_Upper(CPlayer::ANIMATION_KEY::FIRE_PISTOL_LEFT))
	{
		_uint iState = m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower();
		m_iAttack_Handle = CPlayer::ANIMATION_KEY::FIRE_PISTOL_RIGHT;
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, iState);
	}
	else if (m_pObject->Get_MeshCom()->Animation_State_End_Upper(CPlayer::ANIMATION_KEY::FIRE_PISTOL_RIGHT))
	{
		_uint iState = m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower();
		m_iAttack_Handle = CPlayer::ANIMATION_KEY::FIRE_PISTOL_LEFT;
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, iState);
	}
}

void CPlayer_MoveAttack::Exit()
{
	m_iJump_Handle = m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower();
}

CPlayer_MoveAttack * CPlayer_MoveAttack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pObject)
{
	CPlayer_MoveAttack* pInstance = new CPlayer_MoveAttack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Move");
	}
	pInstance->Enter();

	return pInstance;
}

void CPlayer_MoveAttack::Free()
{
	__super::Free();
}
