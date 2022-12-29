#pragma once
#include "Base.h"

BEGIN(Engine)
class CAttribute : public CBase
{
public:
	typedef struct ParticleDesc 
	{
		_float3 m_vDir			= _float3(0.f, 0.f, 0.f);
		_float3 m_vDirStart		= _float3(-1.f, -1.f, -1.f);
		_float3 m_vDirEnd		= _float3(1.f, 1.f, 1.f);
		_float3	m_vPosition		= _float3(0.f, 0.f, 0.f);
		_float3	m_vColor		= _float3(0.f, 0.f, 0.f);

		_float	m_fAge			= 0.f;
		_float	m_fLifeTime		= 0.f;
		_float	m_fColorFade	= 0.f;
		_float	m_fVelocity		= 0.f;
		_float	m_fAcceleration = 0.f;
		_bool	m_isAlive		= true;

		_bool	operator == (const ParticleDesc& rDesc) const
		{
			if (m_vDir != rDesc.m_vDir)
			{
				return false;
			}
			if (m_vDirStart != rDesc.m_vDirStart)
			{
				return false;
			}
			if (m_vDirEnd != rDesc.m_vDirEnd)
			{
				return false;
			}
			if (m_vPosition != rDesc.m_vPosition)
			{
				return false;
			}
			if (m_vColor != rDesc.m_vColor)
			{
				return false;
			}
			if (m_fColorFade != rDesc.m_fColorFade)
			{
				return false;
			}
			if (m_fVelocity != rDesc.m_fVelocity)
			{
				return false;
			}
			if (m_fAcceleration != rDesc.m_fAcceleration)
			{
				return false;
			}
			if (m_fLifeTime != rDesc.m_fLifeTime)
			{
				return false;
			}
			if (m_fAge != rDesc.m_fAge)
			{
				return false;
			}
			if (m_isAlive != rDesc.m_isAlive)
			{
				return false;
			}
			return true;
		}
		_bool	operator != (const ParticleDesc& rDesc) const
		{
			return !(*this == rDesc);
		}
	}PDESC;
private:
	explicit CAttribute(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAttribute(const CAttribute& rhs);
	virtual ~CAttribute() = default;
public:
	HRESULT	NativeConstruct_Prototype();
	HRESULT	NativeConstruct(void* pArg);
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
	_int	Reset();
	_int	Update(_float Time_Delta);
private:
	PDESC	m_tCreateDesc;
	PDESC	m_tCurretnDesc;
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	static CAttribute*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void		Free() override;
};
END

