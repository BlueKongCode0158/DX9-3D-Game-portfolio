#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class CAttribute;
class ENGINE_DLL CVIBuffer_Point final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Point(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Point(const CVIBuffer_Point& rhs);
	virtual ~CVIBuffer_Point() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype(_uint iNumInstance);
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual HRESULT Render_VIBuffer()			override;
	HRESULT			Render_VIBuffer(_uint iNumInstance);
public:
	_int	Update(list<CAttribute*>& rList);
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBInstance			= nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration	= nullptr;
	_uint							m_iNumInstance			= 0;
public:
	static CVIBuffer_Point* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg)	override;
	virtual void		Free()				override;
};
END
