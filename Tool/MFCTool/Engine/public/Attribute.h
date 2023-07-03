/*	
	데이터만 가질 class
	Render 할 필요 없다.
*/
#pragma once
#include "Base.h"

BEGIN(Engine)
class CAttribute : public CBase
{
public:
	enum STATE {ATTRIBUTE_DEAD = -1, ATTRIBUTE_ALIVE, ATTRIBUTE_END};
private:
	explicit CAttribute();
	explicit CAttribute(const CAttribute& rhs);
	virtual ~CAttribute() = default;
public:
	HRESULT	NativeConstruct_Prototype();
	HRESULT	NativeConstruct(void* pArg);
public:
	const PDESC*	GetInfo()
	{
		return &m_tInfo;
	}
public:
	void	SetInfo(PDESC tDesc)
	{
		m_tInfo = tDesc;
	}
public:
	_int	Reset();
	_int	Update(_float Time_Delta);
private:
	void	Move_Position();
	void	Fade_Color();
private:
	PDESC	m_tInfo;
public:
	static CAttribute*	Create();
	CAttribute*			Clone(void* pArg);
	virtual void		Free() override;
};
END

