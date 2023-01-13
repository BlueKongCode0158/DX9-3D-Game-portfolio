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
	virtual ~CAttribute_Layer() = default;
public:
	HRESULT NativeConstruct();
	_int	Tick(_float fTimeDelta);
	HRESULT Render();
private:
	HRESULT SetUp_ConstantTable();
private:
	list<CAttribute*>	m_pAttributeList;
private:
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Point*	m_pVIBufferCom = nullptr;
public:
	static CAttribute_Layer*	Create();
	virtual void				Free()		override;

};
END
 