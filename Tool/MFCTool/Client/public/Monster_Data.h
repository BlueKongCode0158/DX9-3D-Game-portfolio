#pragma once
#include "Client_Defines.h"
#include "Monster.h"
#include "State.h"

BEGIN(Client)
class CMonster_Data : public CState
{
protected:
	explicit CMonster_Data(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CMonster_Data() = default;
protected:
	virtual HRESULT	NativeConstruct(CGameObject* pObject);
public:
	virtual _uint	HandleInput()					override;
	virtual void	Enter()							override;
	virtual void	Update(_float TimeDelta)		override;
	virtual void	Late_Update(_float TimeDelta);
	virtual void	Exit()							override;
public:
	const _float	Get_IdleTime() const;
	void			Set_IdleTime(_float fTime);
public:
	const _float3	Get_SkilTime() const;
	void			Set_SkilTime(_float fSkil0, _float fSkil1, _float fSkil2);
public:
	const _bool		Get_Recog() const;
	void			Set_Recog(_bool isRecog);
protected:
	CMonster*		m_pMonster = nullptr;
	_bool			m_isRecog = false;
protected:
	_float			m_fSkil0		= 0.f;
	_float			m_fSkil1		= 0.f;
	_float			m_fSkil2		= 0.f;
protected:
	_float			m_fIdleTime		= 0.f;
public:
	virtual void	Free() override;
};
END
