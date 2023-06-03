#pragma once
#include "Base.h"

BEGIN(Engine)
class CShader;
class CAttribute;
class CTransform;
class CVIBuffer_Point;
class CAttribute_Layer final : public CBase
{
public:
	enum STATE { PARTICLE_DEAD = -1, PARTICLE_ALIVE };
private:
	explicit CAttribute_Layer();
	explicit CAttribute_Layer(const CAttribute_Layer& rhs);
	virtual ~CAttribute_Layer() = default;
public:
	HRESULT NativeConstruct_Prototype();
	HRESULT NativeConstruct(void* pArg);
	_int	Tick(_float fTimeDelta);
	HRESULT Render(CShader* pShader);
public:

	
public:
	void	Reset();
private:
	HRESULT SetUp_ConstantTable();
private:
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Point*	m_pVIBufferCom = nullptr;
private:
	list<CAttribute*>	m_pAttributeList;
	PDESC				m_tInfo;
public:
	static CAttribute_Layer*	Create();
	CAttribute_Layer*			Clone(void* pArg = nullptr);
	virtual void				Free()		override;
};
END
 