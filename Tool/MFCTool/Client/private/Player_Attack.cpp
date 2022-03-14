#include "stdafx.h"
#include "..\public\Player_Attack.h"
#include "PlayerState.h"
#include "GameInstacne.h"

CPlayer_Attack::CPlayer_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{
}

HRESULT CPlayer_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_Attack::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE_ATTACK;

		//m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_FOWARD, CPlayer::ANIMATION_KEY::RUN_FOWARD);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE_ATTACK;

		//m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_LEFT, CPlayer::ANIMATION_KEY::RUN_LEFT);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE_ATTACK;

		//m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_BACKWARD, CPlayer::ANIMATION_KEY::RUN_BACKWARD);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D) &&
		pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE_ATTACK;

		//m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::RUN_RIGHT, CPlayer::ANIMATION_KEY::RUN_RIGHT);
	}

	if (pGameInstance->Input_KeyMouse_Up(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::IDLE;
	}

	if (pGameInstance->Input_KeyBoard_Up(DIK_SPACE))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::JUMP;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::ATTACK;
}

void CPlayer_Attack::Enter()
{
	m_pObject->Set_PlayerSpeed(2.f);
	m_pObject->Set_PlayerAnimationSpeed(2.f, 1.f);
	m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, CPlayer::ANIMATION_KEY::IDLE_LOOP);
}

void CPlayer_Attack::Update(_float TimeDelta)
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

void CPlayer_Attack::Exit()
{
}

CPlayer_Attack * CPlayer_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pGameObject)
{
	CPlayer_Attack* pInstance = new CPlayer_Attack(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pGameObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Attack");
	}
	pInstance->Enter();

	return pInstance;
}

void CPlayer_Attack::Free()
{
	__super::Free();
}
