#pragma once
#include "Monster_Data.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CMonster;
class CMonster_Beetle_Idle final : public CMonster_Data
{
private:
	explicit CMonster_Beetle_Idle(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Beetle_Idle() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
public:
	static CMonster_Beetle_Idle*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pMonster);
	virtual void			Free() override;
};
END
