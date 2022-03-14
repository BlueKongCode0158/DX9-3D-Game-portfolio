#pragma once
#include "Monster_Data.h"

BEGIN(Client)
class CMonster;
class CMonster_Golem_Laser_Attack final : public CMonster_Data
{
private:
	explicit CMonster_Golem_Laser_Attack(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Golem_Laser_Attack() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
public:
	static CMonster_Golem_Laser_Attack*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pObject);
	virtual void				Free() override;
};
END
