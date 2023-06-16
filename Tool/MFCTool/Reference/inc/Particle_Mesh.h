#pragma once
#include "Particle.h"

BEGIN(Engine)
class CShader;
class CTransform;
class CStatic_Mesh;
class ENGINE_DLL CParticle_Mesh final : public CParticle
{
private:
	explicit CParticle_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticle_Mesh(const CParticle_Mesh& rhs);
	virtual ~CParticle_Mesh() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;
public:
	_int	Tick(_float fTimeDelta);
private:
	CStatic_Mesh*	m_pMeshCom			= nullptr;
	CTransform*		m_pTransformCom		= nullptr;
	CShader*		m_pShaderCom		= nullptr;
private:
	PSDESC			m_tInfo;
public:
	static	CComponent*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void		Free() override;
};
END
