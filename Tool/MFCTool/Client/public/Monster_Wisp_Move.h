#pragma once
#include "Monster_Data.h"

BEGIN(Client)
class CMonster_Wisp_Move final : public CMonster_Data
{
public:
	enum RANDOM_MOVE
	{
		STATE0, STATE1, STATE2, STATE3, STATE_END
	};
private:
	explicit CMonster_Wisp_Move(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Wisp_Move() = default;
private:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
private:
	void Random_Move(RANDOM_MOVE eType, _float TimeDelta);
private:
	_float			m_fAccTime		= 0.f;
	RANDOM_MOVE		m_eMove			=STATE0;
private:
	_bool			m_isActive		= false;
public:
	static CMonster_Wisp_Move*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, CGameObject* pMonster);
	virtual void					Free() override;
};
END
