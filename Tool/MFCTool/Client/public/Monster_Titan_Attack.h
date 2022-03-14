#pragma once
#include "Monster_Data.h"

BEGIN(Client)
class CMonster_Titan_Attack final : public CMonster_Data
{
private:
	explicit CMonster_Titan_Attack(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Titan_Attack() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
private:
	_bool			m_isAttack = false;
public:
	static CMonster_Titan_Attack*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pMonster);
	virtual void					Free() override;
};
END
