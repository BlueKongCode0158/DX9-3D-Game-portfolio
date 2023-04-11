#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CParticle : public CComponent
{
protected:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
private:
	_int				m_iIndex = 0;
public:
	virtual CComponent* Clone(void * pArg = nullptr) PURE;
	virtual void		Free() override;
};
END
