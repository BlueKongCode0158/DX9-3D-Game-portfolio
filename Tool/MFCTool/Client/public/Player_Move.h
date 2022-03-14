#pragma once
#include "PlayerData.h"

BEGIN(Client)
class CPlayer_Move final : public CPlayerData
{
private:
	explicit CPlayer_Move(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	 ~CPlayer_Move() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject) override;
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Exit()							override;
private:
	_bool	m_isRoll		= true;
	_bool	m_isWalk		= true;
	_float	m_fRollTimeAcc	= 5.f;
	_float	m_fTimeAcc		= 0.f;
public:
	static CPlayer_Move*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject);
	virtual void			Free() override;
};
END
