#pragma once
#include "PlayerData.h"

BEGIN(Client)
class CPlayer_Jump final : public CPlayerData
{
private:
	explicit CPlayer_Jump(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CPlayer_Jump() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject) override;
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Exit()							override;
private:
	_float	m_fJumpHeight = 0.f;
public:
	static CPlayer_Jump*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject);
	virtual void			Free() override;
};
END
