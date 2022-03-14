#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CState abstract : public CBase
{
protected:
	 explicit CState(LPDIRECT3DDEVICE9 pGraphic_Device);
	 virtual  ~CState() = default;
public:
	virtual _uint	HandleInput()					PURE;
	virtual void	Enter()							PURE;
	virtual void	Update(_float TimeDelta)		PURE;
	virtual void	Exit()							PURE;
protected:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
public:
	virtual void	Free() override;
};
END
