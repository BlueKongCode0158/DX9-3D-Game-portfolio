/*	
	데이터만 가질 class
	Render 할 필요 없다.
*/
#pragma once
#include "Base.h"

BEGIN(Engine)
class CAttribute : public CBase
{
private:
	explicit CAttribute();
	explicit CAttribute(const CAttribute& rhs);
	virtual ~CAttribute() = default;
public:
	HRESULT	NativeConstruct_Prototype();
	HRESULT	NativeConstruct();
public:
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

