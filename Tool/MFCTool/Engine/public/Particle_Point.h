#pragma once
#include "Particle.h"

BEGIN(Engine)
class CShader;
class CTransform;
class CVIBuffer_Point;
class CAttribute_Layer;
class ENGINE_DLL CParticle_Point final : public CParticle
{
public:
	enum STATE { PARTICLE_DEAD = -1, PARTICLE_ALIVE };
private:
	explicit CParticle_Point(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticle_Point(const CParticle_Point& rhs);
	virtual ~CParticle_Point() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;
public:
	_int	Tick(_float fTimeDelta);
	_int	Reset();
	_int	Set_Transform(_matrix tMatrix);
private:
	CAttribute*				m_pPrototypeSystem = nullptr;
	list<CAttribute_Layer*>	m_pAttributeList;
private:
	_float	m_fTime		= 0.f;
	_bool	m_isLoop	= false;
private:
	PSDESC	m_tDesc;
public:
	static	CComponent*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;
};
END
