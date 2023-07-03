#pragma once
#include "Particle.h"

/*------------------------
	Particle_Point
	가 있으면 
	Attribute_Layer.h
	파일은 필요없어보인다.
-------------------------*/

BEGIN(Engine)
class CShader;
class CAttribute;
class CTransform;
class CVIBuffer_Point;
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
private:
	HRESULT	SetUp_ConstantTable();
	void	Create_Attribute();
	void	Update_List(_float fTimeDelta);
public:
	_int	Tick(_float fTimeDelta);
public:
	void	Set_Position(_float3 vPosition);
	_bool	Set_Index(_int iIndex);
	_bool	Set_ParticleInfo(PDESC* tPInfo);
private:
	// 스마트 포인터식으로 RefCnt로 카운팅 할 것이다.
	CAttribute*				m_pPrototypeSystem = nullptr;
	list<CAttribute*>		m_pAttributeList;
private:
	PDESC	m_tInfo;
	PSDESC	m_tParticleState;
private:
	_float	m_fAccTime		= 0.f;
	_int	m_iShaderNumber = 0;
private:
	//	모든 파티클들은 하나의 쉐이더로 그린다.
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransform = nullptr;
	CVIBuffer_Point*	m_pVIBuffer = nullptr;
public:
	static	CComponent*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;
};
END
