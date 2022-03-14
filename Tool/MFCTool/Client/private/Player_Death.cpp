#include "stdafx.h"
#include "..\public\Player_Death.h"
#include "PlayerState.h"
#include "Player.h"

CPlayer_Death::CPlayer_Death(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPlayerData(pGraphic_Device)
{

}

HRESULT CPlayer_Death::NativeConstruct(CGameObject * pObject)
{
	if (FAILED(__super::NativeConstruct(pObject)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CPlayer_Death::HandleInput()
{
	return CPlayerState::MOTION::DEATH;
}

void CPlayer_Death::Enter()
{
	if (nullptr == m_pObject)
	{
		return;
	}
	m_pObject->Set_PlayerAnimation(CPlayer::ANIMATION_KEY::SLIDE_FORWARD, CPlayer::ANIMATION_KEY::SLIDE_FORWARD);
	m_pObject->Set_PlayerSpeed(0.f);
	m_pObject->Set_PlayerDead(true);
}

void CPlayer_Death::Update(_float TimeDelta)
{
}

void CPlayer_Death::Exit()
{
	m_pObject->Set_PlayerDead(true);
}

CPlayer_Death * CPlayer_Death::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject * pObject)
{
	CPlayer_Death* pInstance = new CPlayer_Death(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct(pObject)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPlayer_Move");
	}
	pInstance->Enter();

	return pInstance;
}

void CPlayer_Death::Free()
{
	__super::Free();
}
