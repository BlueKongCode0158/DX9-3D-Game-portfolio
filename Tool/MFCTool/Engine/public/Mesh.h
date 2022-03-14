#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CMesh abstract : public CComponent
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype()				override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr)	override;
public:
	virtual CComponent* Clone(void* pArg)PURE;
	virtual void Free() override;
};
END

