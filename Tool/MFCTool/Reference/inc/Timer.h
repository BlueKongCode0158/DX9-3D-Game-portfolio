#pragma once
#include "Base.h"

BEGIN(Engine)
class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual  ~CTimer() = default;
public:
	HRESULT		Ready_Time();
	void		Update_Time();
public:
	const _float		Get_Time()
	{
		return m_fTimeDelta;
	}
private:
	float			m_fTimeDelta;
	LARGE_INTEGER	m_tCupTick;
	LARGE_INTEGER	m_tBegin;
	LARGE_INTEGER	m_tEnd;
	LARGE_INTEGER	m_tInvalidate;
public:
	static	CTimer*	Create();
	virtual void	Free() override;
};
END

