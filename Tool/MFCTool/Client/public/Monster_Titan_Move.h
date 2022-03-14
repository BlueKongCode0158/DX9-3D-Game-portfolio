#pragma once
#include "Monster_Data.h"

BEGIN(Client)
class CMonster_Titan_Move final : public CMonster_Data
{
private:
	explicit CMonster_Titan_Move(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Titan_Move() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
private:
	float			m_fSpeed = 0.f;
public:
	static CMonster_Titan_Move*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pMonster);
	virtual void					Free() override;
};
END
