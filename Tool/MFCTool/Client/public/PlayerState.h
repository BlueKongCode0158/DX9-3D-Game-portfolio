#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CPlayer;
class CPlayerData;
class CPlayerState final : public CState
{
public:
	enum MOTION
	{
		ATTACK,
		MOVE_ATTACK,
		JUMP_ATTACK,
		MOVE,
		JUMP,
		IDLE,
		DEATH,
		MOTION_END
	};
protected:
	explicit CPlayerState(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CPlayerState() = default;
protected:
	HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual void			ChangeState();
public:
	virtual _uint			HandleInput()	override;
	virtual void			Enter()			override;
	virtual void			Update(_float TimeDelta)		override;
	virtual void			Exit()			override;
protected:
	CPlayer*				m_pObject			= nullptr;
private:
	map<CPlayerState::MOTION, CPlayerData*>	m_mapPlayerState;
private:
	MOTION			m_iCurrentAnim	= IDLE;
	MOTION			m_iNewAnim		= IDLE;
private:
	_uint			m_iAttackHandle = 0;
	_uint			m_iJumpHandle	= 0;
private:
	_bool			m_isJumping		= false;
public:
	static	CPlayerState*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject);
	virtual void			Free()				override;
};
END
