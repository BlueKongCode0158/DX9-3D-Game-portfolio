#pragma once
#include "Base.h"

BEGIN(Engine)
class CShader;
class CTransform;
class CAttribute;
class CVIBuffer_Point;
class ENGINE_DLL CParticle_System : public CBase
{
private:
	explicit CParticle_System(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticle_System(const CParticle_System& rhs);
	virtual  ~CParticle_System() = default;
public:
	HRESULT	NativeConstruct_Prototype();
	HRESULT	NativeConstruct(void* pArg);
public:
	HRESULT	Reset();
	_int	Tick(_float Time_Delta);
	_int	Late_Tick(_float Time_Delta);
	HRESULT	Render();
private:
	HRESULT	SetUp_ConstantTable();
private:
	CAttribute*			m_pPrototypeSystem = nullptr;
private:
	list<CAttribute*>	m_pAttributeList;
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransfomCom = nullptr;
	CVIBuffer_Point*	m_pVIBufferCom = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	static CParticle_System*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticle_System*			Clone(void* pArg);
	virtual void				Free() override;
};
END
