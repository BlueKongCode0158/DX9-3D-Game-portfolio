#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class CAttribute;
class ENGINE_DLL CVIBuffer_Gizmo final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Gizmo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Gizmo(const CVIBuffer_Gizmo& rhs);
	virtual ~CVIBuffer_Gizmo() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype();
	virtual HRESULT	NativeConstruct(void* pArg) override;
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBInstance = nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration = nullptr;
	_uint							m_iNumInstance = 0;
public:
	static CVIBuffer_Gizmo* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance);
	virtual CComponent*		Clone(void* pArg)	override;
	virtual void			Free()				override;
};
END
