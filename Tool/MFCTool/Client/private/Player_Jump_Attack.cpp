#include "stdafx.h"
#include "..\public\Player_Jump_Attack.h"
#include "PlayerState.h"
#include "GameInstacne.h"

CPlayer_Jump_Attack::CPlayer_Jump_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{
}

HRESULT CPlayer_Jump_Attack::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_Jump_Attack::HandleInput()
{
	CGameInstacne* pGameinsctance = GET_INSTANCE(CGameInstacne);

	if (CPlayer::ANIMATION_KEY::JUMP == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower() &&
		m_pObject->Get_MeshCom()->Play_Animation_Per() > 0.6f)
	{
		m_iJump_Handle = CPlayer::ANIMATION_KEY::AIR_UP;
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, m_iJump_Handle);
	}

	_float3 vPosition = m_pObject->Get_TransformCom()->Get_MatrixRow(CTransform::STATE_POSITION);

	if (vPosition.y < m_fJumpHeight)
	{
		m_iJump_Handle = CPlayer::ANIMATION_KEY::AIR_DOWN;
		m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iAttack_Handle, m_iJump_Handle);
	}


	if (pGameinsctance->Input_KeyBoard_Down(DIK_W) &&
		false == m_pObject->Get_TransformCom()->Get_JumpState())
	{
		goto Move;
	}
	if (pGameinsctance->Input_KeyBoard_Down(DIK_A) &&
		false == m_pObject->Get_TransformCom()->Get_JumpState())
	{
		goto Move;
	}
	if (pGameinsctance->Input_KeyBoard_Down(DIK_S) &&
		false == m_pObject->Get_TransformCom()->Get_JumpState())
	{
		goto Move;
	}
	if (pGameinsctance->Input_KeyBoard_Down(DIK_D) &&
		false == m_pObject->Get_TransformCom()->Get_JumpState())
	{
		goto Move;
	}
	if (false == m_pObject->Get_TransformCom()->Get_JumpState())
	{
		goto Idle;
	}


	if (pGameinsctance->Input_KeyMouse_Up(CInput_Device::DIM::DIM_LBUTTON))
	{
		goto Jump;
	}


	
	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::JUMP_ATTACK;
Move:
	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::MOVE;
Idle:
	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::IDLE;
Jump:
	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::JUMP;
}

void CPlayer_Jump_Attack::Enter()
{
	//m_pObject->Set_PlayerSpeed(2.f);
	m_pObject->Set_PlayerAnimationSpeed(2.f, 1.f);


	if (m_iJump_Handle != CPlayer::JUMP	  &&
		m_iJump_Handle != CPlayer::AIR_DOWN &&
		m_iJump_Handle != CPlayer::AIR_UP)
	{
		m_iJump_Handle = CPlayer::JUMP;
	}

	m_pObject->Set_PlayerAnimation(m_iAttack_Handle, m_iJump_Handle);
}

void CPlayer_Jump_Attack::Update(_float TimeDelta)
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

	_float3 fHeight = m_pObject->Get_TransformCom()->Get_MatrixRow(CTransform::STATE_POSITION);

	m_fJumpHeight = fHeight.y;

}

void CPlayer_Jump_Attack::Exit()
{
	m_iJump_Handle = m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower();
}

CPlayer_Jump_Attack * CPlayer_Jump_Attack::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pObject)
{
	CPlayer_Jump_Attack* pInstance = new CPlayer_Jump_Attack(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Jump_Attack");
	}
	return pInstance;
}

void CPlayer_Jump_Attack::Free()
{
	__super::Free();
}
