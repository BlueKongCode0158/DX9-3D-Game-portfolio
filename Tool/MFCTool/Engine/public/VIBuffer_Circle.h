#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Circle final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Circle(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Circle(const CVIBuffer_Circle& rhs);
	virtual	 ~CVIBuffer_Circle() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype();
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual HRESULT Render_VIBuffer()			override;
	HRESULT			Render_VIBuffer(_uint iNumInstance);
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBInstance = nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration = nullptr;
	_uint							m_iNumInstance = 0;
public:
	static CVIBuffer_Circle*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance);
	virtual CComponent*			Clone(void* pArg)	override;
	virtual void				Free()				override;
};
END

