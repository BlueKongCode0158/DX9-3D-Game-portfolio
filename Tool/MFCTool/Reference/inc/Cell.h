#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CCell final :public CBase
{
public:
	enum POINT	{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE	{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
public:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CCell() = default;
public:
	_float3	Get_Points(POINT ePoint)
	{
		return m_vPoint[ePoint];
	}
	_uint	Get_Index()
	{
		return m_iIndex;
	}
	void Set_Neighbor(LINE eLine, CCell* pNeighbor)
	{
		m_pNeighbor[eLine] = pNeighbor;
	}
public:
	HRESULT NativeConstruct(_float3 vPointA, _float3 vPointB, _float3 vPointC, _uint iIndex);
	_bool	Compare_Points(_float3 vSourPoint, _float3 vDestPoint);
	_bool	isIn(_float3 vPoint, CCell** ppNeighbor, _float3** vNormal);
#ifdef _DEBUG
public:
	HRESULT	Ready_Debug();
	HRESULT	Render(const _matrix* pWorldMatrix,D3DXCOLOR Color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
private:
	LPD3DXLINE			m_pLine = nullptr;
#endif // DEBUG
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	_float3				m_vPoint[POINT_END];
	_float3				m_vLine[LINE_END];
	_float3				m_vNormal[LINE_END];
	CCell*				m_pNeighbor[LINE_END];
	_uint				m_iIndex = 0;
public:
	static  CCell*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float3 vPointA, _float3 vPointB, _float3 vPointC, _uint iIndex);
	virtual void	Free() override;
};
END
	