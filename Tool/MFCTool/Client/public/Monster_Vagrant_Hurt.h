#pragma once
#include "Monster_Data.h"

BEGIN(Client)
class CMonster_Vagrant_Hurt final : public CMonster_Data
{
private:
	explicit CMonster_Vagrant_Hurt(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Vagrant_Hurt() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
public:
	static CMonster_Vagrant_Hurt*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pMonster);
	virtual void					Free() override;
};
END
