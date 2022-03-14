#include "stdafx.h"
#include "..\public\Player_Idle.h"
#include "PlayerState.h"
#include "GameInstacne.h"

CPlayer_Idle::CPlayer_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{
}

HRESULT CPlayer_Idle::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_Idle::HandleInput()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE;
	}

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE;
	}

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE;
	}

	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::MOVE;
	}
	
	if (pGameInstance->Input_KeyMouse_Pressing(CInput_Device::DIM::DIM_LBUTTON))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::ATTACK;
	}
	if (pGameInstance->Input_KeyBoard_Down(DIK_SPACE))
	{
		RELEASE_INSTANCE(CGameInstacne);
		return CPlayerState::MOTION::JUMP;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::IDLE;
}

void CPlayer_Idle::Enter()
{
	m_pObject->Set_PlayerSpeed(0.f);
	m_pObject->Set_PlayerAnimationSpeed(2.f, 2.f);
	m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::IDLE_LOOP, CPlayer::ANIMATION_KEY::IDLE_LOOP);
}

void CPlayer_Idle::Update(_float TimeDelta)
{
	if (m_pObject->Get_MeshCom()->Animation_State_End(CPlayer::ANIMATION_KEY::IDLE_IN))
	{
		m_pObject->Set_PlayerAnimationSpeed(1.f, 1.f);
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::IDLE_LOOP, CPlayer::ANIMATION_KEY::IDLE_LOOP);
	}
}

void CPlayer_Idle::Exit()
{

}

CPlayer_Idle * CPlayer_Idle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject)
{
	CPlayer_Idle* pInstance = new CPlayer_Idle(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Idle");
	}
	pInstance->Enter();

	return pInstance;
}

void CPlayer_Idle::Free()
{
	__super::Free();
}
