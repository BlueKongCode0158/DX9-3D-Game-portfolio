#pragma once
#include "Monster_Data.h"

BEGIN(Client)
class CMonster_Golem_Flinch final : public CMonster_Data
{
private:
	explicit CMonster_Golem_Flinch(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Golem_Flinch() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
public:
	static CMonster_Golem_Flinch*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pMonster);
	virtual void					Free() override;
};
END
