#pragma once
#include "PlayerData.h"

BEGIN(Client)
class CPlayer_Attack final : public CPlayerData
{
private:
	explicit CPlayer_Attack(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	 ~CPlayer_Attack() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject) override;
public:
	virtual _uint	HandleInput()	override;
	virtual void	Enter()			override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Exit()			override;
public:
	static CPlayer_Attack*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject);
	virtual void			Free() override;
};
END
