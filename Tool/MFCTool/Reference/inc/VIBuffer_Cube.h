#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual  ~CVIBuffer_Cube() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
public:
	static CVIBuffer_Cube*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free() override;

};
END

