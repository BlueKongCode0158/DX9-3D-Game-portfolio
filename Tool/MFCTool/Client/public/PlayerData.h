#pragma once
#include "Client_Defines.h"
#include "Player.h"
#include "State.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)
class CPlayerData : public CState
{
protected:
	explicit CPlayerData(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	 ~CPlayerData() = default;
protected:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	_uint	Get_AttackHandle_Motion();
	_uint	Get_JumpHandle_Motion();
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Exit()							override;
public:
	virtual void	Set_AttackHandle(_uint iAttack_Handle);
	virtual void	Set_JumpHandle(_uint iJump_Handle);
protected:
	CPlayer*	m_pObject = nullptr;
	_uint		m_iAttack_Handle	= CPlayer::ANIMATION_KEY::FIRE_PISTOL_LEFT;
	_uint		m_iJump_Handle		= CPlayer::ANIMATION_KEY::ANIMATIONKEY_END;
public:
	virtual void		Free() override;
};
END
