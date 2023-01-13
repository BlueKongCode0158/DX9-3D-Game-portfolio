#pragma once
#include "Base.h"

BEGIN(Engine)
class CAttribute : public CBase
{
private:
	explicit CAttribute();
	explicit CAttribute();
	virtual ~CAttribute() = default;
public:
	HRESULT	NativeConstruct_Prototype();
	HRESULT	NativeConstruct();
public:
	_bool	GetAlive()
	{
		return m_tCurretnDesc.m_isAlive;
	}
	const PDESC*	GetInfo()
	{
		return &m_tCurretnDesc;
	}
public:
	void	SetInfo(PDESC tDesc)
	{
		m_tCreateDesc = tDesc;
	}
public:
	_int	Reset();
	_int	Update(_float Time_Delta);
private:
	PDESC	m_tCreateDesc;
	PDESC	m_tCurretnDesc;
public:
	static CAttribute*	Create();
	CAttribute*			Clone();
	virtual void		Free() override;
};
END

