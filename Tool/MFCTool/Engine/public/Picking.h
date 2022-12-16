#pragma once
#include "Base.h"

BEGIN(Engine)
class CPicking : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	explicit CPicking();
	virtual  ~CPicking() = default;
public:
	HRESULT		NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device);
public:
	HRESULT		Compute_MouseCursorPosInWorld(HWND hWnd);
	_float3*	Compute_PickingPoint(class CVIBuffer* pVIBuffer, _matrix WorldMatrix);
	_float3*	Compute_PickingPoint(LPD3DXMESH pMesh, _matrix WorldMatrix);
public:
	HRESULT		Compute_WindowCenterPosition();
	_float3*	Compute_CenterPoint(class CVIBuffer* pVIBuffer, _matrix WorldMatrix);
	_float3*	Compute_CenterPoint(class CCollider* pCollider, _matrix WorldMatrix);
	_float3*	Compute_CenterPoint(LPD3DXMESH pMesh, _matrix WorldMatrix);
private:
	_float3		m_vMouseRay;
	_float3		m_vMousePivot;
	_float3		m_vResultPos;
private:
	_float3		m_vCenterPivot;
	_float3		m_vCenterPos;
	_float3		m_vCenterResultPos;
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	virtual void Free() override;
};
END
