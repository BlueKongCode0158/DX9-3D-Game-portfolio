#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual  ~CVIBuffer_Rect() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
public:
	static	CVIBuffer_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free() override;
};
END
