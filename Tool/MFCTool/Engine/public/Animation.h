#pragma once
#include "Base.h"

BEGIN(Engine)
class CAnimation final : public CBase
{
private:
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;
public:
	HRESULT NativeConstruct_Prototype();
	HRESULT	NativeConstruct();
	HRESULT Set_AnimationIndex(_uint iAnimIndex);
	HRESULT Play_Animation(_double TimeDelta);
	HRESULT	Bind_Frame(D3DXFRAME_DERIVED* pFrame);
public:
	HRESULT	Set_FrameAnimation(_double AnimationPosition);
	HRESULT	Reset_FrameAnimation();
	_bool	Get_AnimationStop();
public:
	void	Set_AnimationStop(_bool isStop);
public:
	_bool	Animation_State_End(_uint iAnimIndex);
	_uint	Get_CurrentAnimationIndex() const
	{
		return m_iCurrentAnimationIndex;
	}
	_float	Play_Animation_Per();
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device		= nullptr;
	LPD3DXANIMATIONCONTROLLER	m_pAnimationControlor	= nullptr;
private:
	_uint	m_iCurrentAnimationIndex	= 0;
	_uint	m_iCurrentTrack				= 0;
	_uint	m_iNewTrack					= 1;
	_double m_TimeAcc					= 0.0;
private:
	_bool	m_isStop					= false;
public:
	static CAnimation*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController);
	virtual CAnimation* Clone();
	virtual void		Free() override;
};
END
