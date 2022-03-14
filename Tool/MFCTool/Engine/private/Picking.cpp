#include "..\public\Picking.h"
#include "PipeLine.h"
#include "VIBuffer.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{

}

HRESULT CPicking::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	
	Safe_AddRef(pGraphic_Device);
	return S_OK;
}

HRESULT CPicking::Compute_MouseCursorPosInWorld(HWND hWnd)
{
	_float4 vMousePos;
	POINT	ptMouse = { 0,0 };

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9	ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	//vMousePos.x = (2.f * ptMouse.x) / (ViewPort.Width) - 1.f;
	//vMousePos.y = (-2.f * ptMouse.y) / (ViewPort.Height) + 1.f;

	vMousePos.x = (ptMouse.x) / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = (ptMouse.y) / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix		ProjMatrix;
	ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrix);

	m_vMouseRay = _float3(vMousePos.x - 0.f, vMousePos.y - 0.f, vMousePos.z - 0.f);
	m_vMousePivot = _float3{ 0.f,0.f,0.f };

	_matrix		ViewMatrix;
	ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	D3DXVec3TransformNormal(&m_vMouseRay, &m_vMouseRay, &ViewMatrix);
	D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &ViewMatrix);

	RELEASE_INSTANCE(CPipeLine);

	/* 월드까진 해줬어 */

	return S_OK;
}

HRESULT CPicking::Compute_WindowCenterPosition()
{
	_float4 vMousePos;

	/* 화면의 중심에서 나가는 광선을 만들거니까 */
	vMousePos.x = 0.f;
	vMousePos.y = 0.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix		ProjMatrix;
	ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrix);

	m_vCenterPos = _float3(vMousePos.x - 0.f, vMousePos.y - 0.f, vMousePos.z - 0.f);
	m_vCenterPivot = _float3{ 0.f,0.f,0.f };

	_matrix		ViewMatrix;
	ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	D3DXVec3TransformNormal(&m_vCenterPos, &m_vCenterPos, &ViewMatrix);
	D3DXVec3TransformCoord(&m_vCenterPivot, &m_vCenterPivot, &ViewMatrix);

	RELEASE_INSTANCE(CPipeLine);

	/* 월드까진 해줬어 */

	return S_OK;
}

_float3 * CPicking::Compute_PickingPoint(CVIBuffer * pVIBuffer, _matrix WorldMatrix)
{
	/* VIBuffer는 Index얻어주려고 가져온건가 */
	_float3		vRay, vPivot;
	_matrix		WorldMatrixInverse;

	WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&vPivot, &m_vMousePivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vRay, &m_vMouseRay, &WorldMatrixInverse);

	_float3		vPoint[3];
	_float		fMinDist = 315.f;
	_bool		isAction = false;

	for (_uint i = 0; i < pVIBuffer->Get_NumPolygon(); i++)
	{
		pVIBuffer->Get_PolygonVertexPosition(i, vPoint);

		_float		fU, fV, fDist;
		if (true == D3DXIntersectTri(&vPoint[0], &vPoint[1], &vPoint[2], &vPivot, &vRay, &fU, &fV, &fDist))
		{
			isAction = true;
			/* 여기서 거리 저장하고 제일 가까운 녀석 리턴할 수도 있어. */
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
			}
		}
	}

	if (false == isAction)
	{
		return nullptr;
	}
	else
	{
		D3DXVec3Normalize(&vRay, &vRay);
		m_vResultPos = vPivot + fMinDist*vRay;

		D3DXVec3TransformCoord(&m_vResultPos, &m_vResultPos, &WorldMatrix);

		return &m_vResultPos;
	}

	return nullptr;
}

_float3 * CPicking::Compute_PickingPoint(LPD3DXMESH pMesh, _matrix WorldMatrix)
{
	/* VIBuffer는 Index얻어주려고 가져온건가 */
	_float3		vRay, vPivot;
	_matrix		WorldMatrixInverse;

	WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&vPivot, &m_vMousePivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vRay, &m_vMouseRay, &WorldMatrixInverse);

	_float		fMinDist = 315.f;
	_bool		isAction = false;

	_float			fU, fV, fDist;
	_ulong			ufaceIndex, uCountHit;
	LPD3DXBUFFER	pBuffer = nullptr;
	BOOL			bisHit;

	if (true == D3DXIntersect(pMesh, &vPivot, &vRay, &bisHit, &ufaceIndex, &fU, &fV, &fDist, &pBuffer, &uCountHit))
	{
		isAction = true;
		/* 여기서 거리 저장하고 제일 가까운 녀석 리턴할 수도 있어. */
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
		}
	}

	if (false == isAction)
	{
		return nullptr;
	}
	else
	{
		D3DXVec3Normalize(&vRay, &vRay);
		m_vResultPos = vPivot + fMinDist*vRay;

		D3DXVec3TransformCoord(&m_vResultPos, &m_vResultPos, &WorldMatrix);

		return &m_vResultPos;
	}

	return nullptr;
}


_float3 * CPicking::Compute_CenterPoint(CVIBuffer * pVIBuffer, _matrix WorldMatrix)
{
	_float3		vRay, vPivot;
	_matrix		WorldMatrixInverse;

	WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&vPivot, &m_vCenterPivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vRay, &m_vCenterPos, &WorldMatrixInverse);

	_float3		vPoint[3];
	_float		fMinDist = 315.f;
	_bool		isAction = false;

	for (_uint i = 0; i < pVIBuffer->Get_NumPolygon(); i++)
	{
		pVIBuffer->Get_PolygonVertexPosition(i, vPoint);

		_float		fU, fV, fDist;
		if (true == D3DXIntersectTri(&vPoint[0], &vPoint[1], &vPoint[2], &vPivot, &vRay, &fU, &fV, &fDist))
		{
			isAction = true;
			/* 여기서 거리 저장하고 제일 가까운 녀석 리턴할 수도 있어. */
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
			}
		}
	}

	if (false == isAction)
	{
		return nullptr;
	}
	else
	{
		D3DXVec3Normalize(&vRay, &vRay);
		m_vCenterResultPos = vPivot + fMinDist*vRay;

		D3DXVec3TransformCoord(&m_vCenterResultPos, &m_vCenterResultPos, &WorldMatrix);

		return &m_vCenterResultPos;
	}

	return nullptr;
}

_float3 * CPicking::Compute_CenterPoint(CCollider * pCollider, _matrix WorldMatrix)
{
	_float3		vRay, vPivot;
	_matrix		WorldMatrixInverse;
	
	WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&vPivot, &m_vCenterPivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vRay, &m_vCenterPos, &WorldMatrixInverse);

	_float3		vPoint[3];
	_float		fMinDist = 315.f;
	_bool		isAction = false;

	for (_uint i = 0; i < 12; i++)
	{
		pCollider->Get_PointPolygonIndex(i, vPoint);

		_float		fU, fV, fDist;
		if (true == D3DXIntersectTri(&vPoint[0], &vPoint[1],&vPoint[2], &vPivot, &vRay, &fU, &fV, &fDist))
		{
			isAction = true;
			/* 여기서 거리 저장하고 제일 가까운 녀석 리턴할 수도 있어. */
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
			}
		}
	}

	if (false == isAction)
	{
		return nullptr;
	}
	else
	{
		D3DXVec3Normalize(&vRay, &vRay);
		m_vCenterResultPos = vPivot + fMinDist*vRay;

		D3DXVec3TransformCoord(&m_vCenterResultPos, &m_vCenterResultPos, &WorldMatrix);

		return &m_vCenterResultPos;
	}

	return nullptr;
}

_float3 * CPicking::Compute_CenterPoint(LPD3DXMESH pMesh, _matrix WorldMatrix)
{

	_float3		vRay, vPivot;
	_matrix		WorldMatrixInverse;

	WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&vPivot, &m_vCenterPivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vRay, &m_vCenterPos, &WorldMatrixInverse);

	_float		fMinDist = 315.f;
	_bool		isAction = false;

	_float			fU, fV, fDist;
	_ulong			ufaceIndex, uCountHit;
	LPD3DXBUFFER	pBuffer = nullptr;
	BOOL			bisHit;
	if (true == D3DXIntersect(pMesh, &vPivot, &vRay,&bisHit,&ufaceIndex,&fU,&fV,&fDist,&pBuffer,&uCountHit))
	{
		isAction = true;
		/* 여기서 거리 저장하고 제일 가까운 녀석 리턴할 수도 있어. */
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
		}
	}
	

	if (false == isAction)
	{
		return nullptr;
	}
	else
	{
		D3DXVec3Normalize(&vRay, &vRay);
		m_vCenterResultPos = vPivot + fMinDist*vRay;

		D3DXVec3TransformCoord(&m_vCenterResultPos, &m_vCenterResultPos, &WorldMatrix);

		return &m_vCenterResultPos;
	}

	return nullptr;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}

