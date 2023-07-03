#pragma once
#include "Particle.h"

/*------------------------
	Particle_Point
	�� ������ 
	Attribute_Layer.h
	������ �ʿ����δ�.
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
	// ����Ʈ �����ͽ����� RefCnt�� ī���� �� ���̴�.
	CAttribute*				m_pPrototypeSystem = nullptr;
	list<CAttribute*>		m_pAttributeList;
private:
	PDESC	m_tInfo;
	PSDESC	m_tParticleState;
private:
	_float	m_fAccTime		= 0.f;
	_int	m_iShaderNumber = 0;
private:
	//	��� ��ƼŬ���� �ϳ��� ���̴��� �׸���.
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransform = nullptr;
	CVIBuffer_Point*	m_pVIBuffer = nullptr;
public:
	static	CComponent*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;
};
END
