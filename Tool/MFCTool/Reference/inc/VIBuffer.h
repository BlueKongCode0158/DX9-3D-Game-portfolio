#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer : public CComponent 
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual  ~CVIBuffer() = default;
public:
	HRESULT	NativeConstruct_VertexBuffer(_uint iStride
		,_uint iNumVertices, _ulong m_dwVBUsage, _ulong m_dwFVF
		,D3DPRIMITIVETYPE ePrimitiveType, _uint iNumPrimitive);
	HRESULT	NativeConstruct_IndexBuffer(_uint iNumIndexSize,
		_uint iNumVerticesPrimitive, _ulong	dwIBUsage);
public:
	_uint	Get_NumPolygon()
	{
		return m_iNumPrimitive;
	}
	void	Get_PolygonVertexPosition(_uint uiPolygonIndex, _float3* pPoints) const;
public:																			 
	virtual	HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual HRESULT	Render_VIBuffer();
protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
#pragma region VERTEX
protected:
	_uint		m_iStride		= 0;
	_uint		m_iNumVertices	= 0;
	_ulong		m_dwVBUsage		= 0;
	_ulong		m_dwFVF			= 0;
protected:
	D3DPRIMITIVETYPE		m_ePrimitiveType = D3DPRIMITIVETYPE(0);
	_uint					m_iNumPrimitive = 0;
#pragma endregion
#pragma region INDEX
protected:
	_uint	m_iOneIndexSize			= 0;
	_uint	m_iNumVerticesPrimitive = 0;
	_ulong	m_dwIBUsage				= 0;
#pragma endregion
protected:
	_float3*	m_pVerticesPos = nullptr;
	void*		m_pIndeces = nullptr;
public:
	virtual CComponent* Clone(void* pArg = nullptr)PURE;
	virtual void		Free() override;
};
END

