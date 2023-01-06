#pragma once
#include "Component.h"

BEGIN(Engine)
class CShader;
class CAttribute;
class CTransform;
class CVIBuffer_Point;
class CParticle final : public CComponent
{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;
public:
	_int	Tick(_float Time_Delta);
public:
	void	Set_Transform(_matrix ParentMatrix);
private:
	void	Reset();
private:
	CShader*			m_pShaderCom	= nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Point*	m_pVIBufferCom	= nullptr;
private:
	CAttribute*			m_pPrototypeSystem = nullptr;
	list<CAttribute*>	m_pAttributeList;
private:
	_int				m_iIndex = 0;
	_float				m_fTime = 0.f;
private:
	PSDESC				m_tDesc;
public:
	static	CComponent* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent* Clone(void * pArg = nullptr) override;
	virtual void		Free() override;
};
END
