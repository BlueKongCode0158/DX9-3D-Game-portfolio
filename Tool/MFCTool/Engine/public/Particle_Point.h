#pragma once
#include "Particle.h"

BEGIN(Engine)
class CShader;
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
public:
	_bool	Set_Index(_int iIndex);
private:
	CAttribute_Layer*		m_pPrototypeSystem = nullptr;
	list<CAttribute_Layer*>	m_pAttributeList;
private:
	_float	m_fTime			= 0.f;
	_bool	m_isLoop		= false;
	_uint	m_iShaderNumber = 0;
private:
	//	모든 파티클들은 하나의 쉐이더로 그린다.
	CShader*	m_pShaderCom = nullptr;
public:
	static	CComponent*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;
};
END
