#include "stdafx.h"
#include "..\public\Player_Jump.h"
#include "PlayerState.h"
#include "GameInstacne.h"

CPlayer_Jump::CPlayer_Jump(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{
}

HRESULT CPlayer_Jump::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_Jump::HandleInput()
{
	if (CPlayer::ANIMATION_KEY::JUMP == m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex() &&
		m_pObject->Get_MeshCom()->Play_Animation_Per() > 0.6f)
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::AIR_UP, CPlayer::ANIMATION_KEY::AIR_UP);
	}

	_float3 vPosition = m_pObject->Get_TransformCom()->Get_MatrixRow(CTransform::STATE_POSITION);

	if (vPosition.y < m_fJumpHeight)
	{
		m_pObject->Get_MeshCom()->Set_AnimationIndex(CPlayer::ANIMATION_KEY::AIR_DOWN, CPlayer::ANIMATION_KEY::AIR_DOWN);
	}

	if (false == m_pObject->Get_TransformCom()->Get_JumpState())
	{
		return CPlayerState::MOTION::IDLE;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (pGameInstance->Input_KeyMouse_Down(CInput_Device::DIM::DIM_LBUTTON))
	{
		goto JumpAttack;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::JUMP;

JumpAttack:
	RELEASE_INSTANCE(CGameInstacne);
	return CPlayerState::MOTION::JUMP_ATTACK;
}

void CPlayer_Jump::Enter()
{
	if (m_iJump_Handle != CPlayer::JUMP	  &&
		m_iJump_Handle != CPlayer::AIR_DOWN &&
		m_iJump_Handle != CPlayer::AIR_UP)
	{
		m_iJump_Handle = CPlayer::JUMP;
	}


	m_pObject->Get_MeshCom()->Set_AnimationIndex(m_iJump_Handle, m_iJump_Handle);
}

void CPlayer_Jump::Update(_float TimeDelta)
{
	_float3 fHeight = m_pObject->Get_TransformCom()->Get_MatrixRow(CTransform::STATE_POSITION);

	m_fJumpHeight = fHeight.y;
}

void CPlayer_Jump::Exit()
{
	m_iJump_Handle = m_pObject->Get_MeshCom()->Get_CurrentAnimationIndex_Lower();
}

CPlayer_Jump * CPlayer_Jump::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pObject)
{
	CPlayer_Jump* pInstance = new CPlayer_Jump(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Move");
	}
	pInstance->Enter();

	return pInstance;
}

void CPlayer_Jump::Free()
{
	__super::Free();
}
