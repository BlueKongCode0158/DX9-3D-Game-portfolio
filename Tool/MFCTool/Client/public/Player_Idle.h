#pragma once
#include "PlayerData.h"

BEGIN(Client)
class CPlayer_Idle final : public CPlayerData
{
protected:
	explicit CPlayer_Idle(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CPlayer_Idle() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject) override;
public:
	virtual _uint	HandleInput()	override;
	virtual void	Enter()			override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Exit()			override;
public:
	static CPlayer_Idle*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject);
	virtual void			Free() override;
};
END
