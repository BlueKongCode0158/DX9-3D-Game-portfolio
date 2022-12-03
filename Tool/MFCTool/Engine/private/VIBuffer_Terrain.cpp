#include "..\public\VIBuffer_Terrain.h"
#include "Frustum.h"
#include "Transform.h"
#include "QuadTree.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	:CVIBuffer(rhs)
	,m_iNumVerticesX(rhs.m_iNumVerticesX)
	,m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	,m_fInterval(rhs.m_fInterval)
	,m_pQuadTree(rhs.m_pQuadTree)
{
	Safe_AddRef(m_pQuadTree);
}

D3DXPLANE CVIBuffer_Terrain::Get_Plane(_float3 & vPosition) const
{
	_uint iIndex = static_cast<_int>(vPosition.x / m_fInterval) + static_cast<_int>(vPosition.z / m_fInterval)*m_iNumVerticesX;

	if (iIndex > m_iNumVerticesX*m_iNumVerticesZ)
	{
		return D3DXPLANE();
	}

	_uint vIndex[4] =
	{
		m_iNumVerticesX + iIndex , m_iNumVerticesX + iIndex + 1,
		iIndex + 1, iIndex
	};

	_float fIndexX = vPosition.x - m_pVerticesPos[vIndex[0]].x;
	_float fIndexZ = m_pVerticesPos[vIndex[0]].z - vPosition.z;

	D3DXPLANE pPlane;

	if (fIndexX > fIndexZ)
	{
		D3DXPlaneFromPoints(&pPlane, &m_pVerticesPos[vIndex[0]], &m_pVerticesPos[vIndex[1]], &m_pVerticesPos[vIndex[2]]);
	}
	else
	{
		D3DXPlaneFromPoints(&pPlane, &m_pVerticesPos[vIndex[0]], &m_pVerticesPos[vIndex[2]], &m_pVerticesPos[vIndex[3]]);
	}
	return pPlane;
}

HRESULT CVIBuffer_Terrain::Culling(CFrustum * pFrustum, CTransform * pTransform)
{
	_ulong dwIndex = 0;

	m_pQuadTree->Culling(pFrustum, m_pVerticesPos, reinterpret_cast<_ulong*>(m_pIndeces), dwIndex);
	
	_ulong* pIndices = nullptr;
	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	memcpy(pIndices, m_pIndeces, sizeof(_ulong) * dwIndex);
	m_pIB->Unlock();

	m_iNumVerticesPrimitive = dwIndex / 3;
	
	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iIndexX,_uint iIndexZ, _float fInterval)
{

	m_iNumVerticesX		 = iIndexX;
	m_iNumVerticesZ		 = iIndexZ;
	m_fInterval			 = fInterval;

	if (FAILED(__super::NativeConstruct_VertexBuffer(sizeof(VTXNORMAL), m_iNumVerticesX*m_iNumVerticesZ, 0, (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1), D3DPT_TRIANGLELIST, (m_iNumVerticesX - 1 )*(m_iNumVerticesZ - 1) *2)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct_IndexBuffer(4, 3, 0)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	VTXNORMAL*		pVertices	= nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i*iIndexX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j*fInterval , 0.f,i*fInterval);
			pVertices[iIndex].vNormal	= _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV	= _float2(static_cast<_float>(i) / (iIndexX - 1) , static_cast<_float>(j) / (iIndexZ - 1));
		}
	}
	m_pVB->Unlock();

	_uint		uindexVerticesNum = 0;
	_uint*		pIndex = nullptr;

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0);

	for (int i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (int j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + m_iNumVerticesX;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + m_iNumVerticesX + 1;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + 1;

			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + m_iNumVerticesX;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + 1;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * pFilePath, _float fInterval)
{
	_uint iIndexX = 0;
	_uint iIndexZ = 0;

	_ulong* pPixel = nullptr;

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (INVALID_HANDLE_VALUE != hFile)
	{

#pragma region READFILE
		_ulong dwByte;

		BITMAPFILEHEADER	fh;
		ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

		BITMAPINFOHEADER	ih;
		ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

		iIndexX = ih.biWidth;
		iIndexZ = ih.biHeight;

		pPixel = new _ulong[iIndexX*iIndexZ];
		ReadFile(hFile, pPixel, sizeof(_ulong)*iIndexX*iIndexZ, &dwByte, nullptr);

		CloseHandle(hFile);
#pragma endregion

	}
	else
	{
		return E_FAIL;
	}

	m_iNumVerticesX = iIndexX;
	m_iNumVerticesZ = iIndexZ;
	m_fInterval = fInterval;

	if (FAILED(__super::NativeConstruct_VertexBuffer(sizeof(VTXNORMAL), m_iNumVerticesX*m_iNumVerticesZ, 0, (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1), D3DPT_TRIANGLELIST, (m_iNumVerticesX - 1)*(m_iNumVerticesZ - 1) * 2)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct_IndexBuffer(4, 3, 0)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	VTXNORMAL*		pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i*iIndexX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j*fInterval, (pPixel[iIndex] & 0x000000ff) , i*fInterval);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			D3DXVec3Normalize(&pVertices[iIndex].vNormal, &pVertices[iIndex].vNormal);
			pVertices[iIndex].vTexUV = _float2(static_cast<_float>(i) / (iIndexX - 1), static_cast<_float>(j) / (iIndexZ - 1));
		}
	}

	_uint		uindexVerticesNum = 0;
	_uint*		pIndex = nullptr;

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0);

	for (int i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (int j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + m_iNumVerticesX;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + m_iNumVerticesX + 1;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + 1;

			_float3	vDest, vSour, vNormal;

			vDest = pVertices[iIndex + m_iNumVerticesX + 1].vPosition - pVertices[iIndex + m_iNumVerticesX].vPosition;
			vSour = pVertices[iIndex + 1].vPosition - pVertices[iIndex + m_iNumVerticesX + 1].vPosition;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[iIndex + m_iNumVerticesX].vNormal		+= vNormal;
			pVertices[iIndex + m_iNumVerticesX + 1].vNormal += vNormal;
			pVertices[iIndex + 1].vNormal					+= vNormal;

			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + m_iNumVerticesX;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex + 1;
			pIndex[uindexVerticesNum++] = reinterpret_cast<_uint*>(m_pIndeces)[uindexVerticesNum] = iIndex;

			vDest = pVertices[iIndex + 1].vPosition - pVertices[iIndex + m_iNumVerticesX].vPosition;
			vSour = pVertices[iIndex].vPosition - pVertices[iIndex + 1].vPosition;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[iIndex + m_iNumVerticesX].vNormal += vNormal;
			pVertices[iIndex + 1].vNormal				+= vNormal;
			pVertices[iIndex].vNormal					+= vNormal;
		}
	}

	for (_uint i = 0; i < m_iNumVerticesX*m_iNumVerticesZ; i++)
	{
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
	}

	m_pIB->Unlock();
	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);

	m_pQuadTree = CQuadTree::Create(m_iNumVerticesX*m_iNumVerticesZ - m_iNumVerticesX, m_iNumVerticesX*m_iNumVerticesZ - 1, m_iNumVerticesZ - 1, 0);
	if (nullptr == m_pQuadTree)
	{
		return E_FAIL;
	}
	if (FAILED(m_pQuadTree->SetUp_Neighbor()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);
	return S_OK;
}

HRESULT CVIBuffer_Terrain::Update_Terrain_Height(_uint eType, LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, _float Brush_Size, _float3 vPicking_Position, const _tchar* pFilePath)
{
	RECT tRect = {};
	tRect.bottom = static_cast<LONG>(vPicking_Position.z) - Brush_Size;
	tRect.top = static_cast<LONG>(vPicking_Position.z) + Brush_Size;
	tRect.left = static_cast<LONG>(vPicking_Position.x)	- Brush_Size;
	tRect.right = static_cast<LONG>(vPicking_Position.x) + Brush_Size;

	switch (eType)
	{
	case 0:
		Update_Terrain_Brush_Rect(pHeightMap, TimeDelta, tRect, pFilePath);
		break;
	case 1:
		Update_Terrain_Brush_Circle(pHeightMap, TimeDelta, tRect, Brush_Size ,vPicking_Position ,pFilePath);
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CVIBuffer_Terrain::Update_Terrain_Aver(_uint eType, LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, _float fBrushSize, _float3 vPicking_Position, const _tchar * pFilePath)
{
	RECT tRect = {};
	tRect.bottom = static_cast<LONG>(vPicking_Position.z) - fBrushSize;
	tRect.top = static_cast<LONG>(vPicking_Position.z) + fBrushSize;
	tRect.left = static_cast<LONG>(vPicking_Position.x) - fBrushSize;
	tRect.right = static_cast<LONG>(vPicking_Position.x) + fBrushSize;

	switch (eType)
	{
	case 0:
		Update_Terrain_Flat_Rect(pHeightMap, TimeDelta, tRect, fBrushSize,vPicking_Position, pFilePath);
		break;
	case 1:
		Update_Terrain_Flat_Circle(pHeightMap, TimeDelta, tRect, fBrushSize, vPicking_Position, pFilePath);
		break;
	default:
		break;
	}

	return S_OK;
}

void CVIBuffer_Terrain::Update_Terrain_Brush_Rect(LPDIRECT3DTEXTURE9 pHightMap, _float TimeDelta, RECT& RangeRect, const _tchar * pFilePath)
{
	
	if (RangeRect.left < 0)
	{
		RangeRect.left = 0;
	}
	if (RangeRect.right > m_iNumVerticesZ*m_fInterval)
	{
		RangeRect.right = m_iNumVerticesZ*m_fInterval;
	}
	if (RangeRect.bottom < 0)
	{
		RangeRect.bottom = 0;
	}
	if (RangeRect.top > m_iNumVerticesX*m_fInterval)
	{
		RangeRect.top = m_iNumVerticesX*m_fInterval;
	}


	VTXNORMAL*			pVertices = nullptr;
	D3DLOCKED_RECT		lockedRect;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	pHightMap->LockRect(0, &lockedRect, 0, 0);

	for (_uint i = RangeRect.bottom; i < RangeRect.top; i++)
	{
		for (_uint j = RangeRect.left; j < RangeRect.right; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			if (pVertices[iIndex].vPosition.y <= 255.f)
			{
				pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = m_pVerticesPos[iIndex].y + TimeDelta;
			}
			else
			{
				pVertices[iIndex].vPosition.y = 255.f;
			}

			_float fHeight_texture = pVertices[iIndex].vPosition.y / 255.f;

			((_ulong*)lockedRect.pBits)[iIndex] = D3DXCOLOR(
				fHeight_texture,
				fHeight_texture,
				fHeight_texture,
				fHeight_texture);
		}
	}

	pHightMap->UnlockRect(0);
	m_pVB->Unlock();

	D3DXSaveTextureToFile(pFilePath, D3DXIFF_BMP, pHightMap, nullptr);

	Create_Terrain_NormalVector();

}

void CVIBuffer_Terrain::Update_Terrain_Brush_Circle(LPDIRECT3DTEXTURE9 pHightMap, _float TimeDelta, RECT& RangeRect, _float fBrushSize, _float3 vPickingPosition, const _tchar * pFilePath)
{

	if (RangeRect.left < 0)
	{
		RangeRect.left = 0;
	}
	if (RangeRect.right > m_iNumVerticesZ*m_fInterval)
	{
		RangeRect.right = m_iNumVerticesZ*m_fInterval;
	}
	if (RangeRect.bottom < 0)
	{
		RangeRect.bottom = 0;
	}
	if (RangeRect.top > m_iNumVerticesX*m_fInterval)
	{
		RangeRect.top = m_iNumVerticesX*m_fInterval;
	}

	VTXNORMAL*			pVertices = nullptr;
	D3DLOCKED_RECT		lockedRect;

	_float	fDistance = 0.f;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	pHightMap->LockRect(0, &lockedRect, 0, 0);

	for (_uint i = RangeRect.bottom; i < RangeRect.top; i++)
	{
		for (_uint j = RangeRect.left; j < RangeRect.right; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			_float fDistanceX = j - vPickingPosition.x;
			_float fDistanceZ = i - vPickingPosition.z;

			fDistance = sqrt(fDistanceX* fDistanceX + fDistanceZ*fDistanceZ);

			if (fabsf(fDistance) > fBrushSize)
			{
				continue;
			}

			_float fHeight = pow(fBrushSize, 2) - (fDistance*fDistance);


			if (pVertices[iIndex].vPosition.y <= 255.f)
			{
				pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = m_pVerticesPos[iIndex].y + fHeight*TimeDelta;
			}
			else
			{
				pVertices[iIndex].vPosition.y = 255.f;
			}

			_float fHeight_texture = pVertices[iIndex].vPosition.y / 255.f;

			((_ulong*)lockedRect.pBits)[iIndex] = D3DXCOLOR(
				fHeight_texture,
				fHeight_texture,
				fHeight_texture,
				fHeight_texture);
		}
	}
	pHightMap->UnlockRect(0);
	m_pVB->Unlock();

	D3DXSaveTextureToFile(pFilePath, D3DXIFF_BMP, pHightMap, nullptr);

	Create_Terrain_NormalVector();
}

void CVIBuffer_Terrain::Update_Terrain_Flat_Circle(LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, RECT & RangeRect, _float fBrushSize, _float3 vPickingPosition, const _tchar * pFilePath)
{
	if (RangeRect.left < 0)
	{
		RangeRect.left = 0;
	}
	if (RangeRect.right > m_iNumVerticesZ*m_fInterval)
	{
		RangeRect.right = m_iNumVerticesZ*m_fInterval;
	}
	if (RangeRect.bottom < 0)
	{
		RangeRect.bottom = 0;
	}
	if (RangeRect.top > m_iNumVerticesX*m_fInterval)
	{
		RangeRect.top = m_iNumVerticesX*m_fInterval;
	}

	VTXNORMAL*			pVertices = nullptr;
	D3DLOCKED_RECT		lockedRect;

	_float	fDistance = 0.f;

	_double	dHeight = 0.0;
	_uint	iIndexAcc = 0;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	pHeightMap->LockRect(0, &lockedRect, 0, 0);

	for (_uint i = RangeRect.bottom; i < RangeRect.top; i++)
	{
		for (_uint j = RangeRect.left; j < RangeRect.right; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			dHeight += pVertices[iIndex].vPosition.y;
			iIndexAcc++;
			//pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = m_pVerticesPos[iIndex].y + fHeight*TimeDelta;

		}
	}
	dHeight = dHeight / iIndexAcc;

	for (_uint i = RangeRect.bottom; i < RangeRect.top; i++)
	{
		for (_uint j = RangeRect.left; j < RangeRect.right; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			_float fDistanceX = j - vPickingPosition.x;
			_float fDistanceZ = i - vPickingPosition.z;

			fDistance = sqrt(fDistanceX* fDistanceX + fDistanceZ*fDistanceZ);

			if (fabsf(fDistance) > fBrushSize)
			{
				continue;
			}

			_float fHeight = pow(fBrushSize, 2) - (fDistance*fDistance);

			if (pVertices[iIndex].vPosition.y <= 255.f)
			{
				if (pVertices[iIndex].vPosition.y > dHeight)
				{
					pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = m_pVerticesPos[iIndex].y - TimeDelta;
				}
				else if (pVertices[iIndex].vPosition.y < dHeight)
				{
					pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = m_pVerticesPos[iIndex].y + TimeDelta;
				}
				
			}
			else
			{
				pVertices[iIndex].vPosition.y = 255.f;
			}

			_float fHeight_texture = pVertices[iIndex].vPosition.y / 255.f;

			((_ulong*)lockedRect.pBits)[iIndex] = D3DXCOLOR(
				fHeight_texture,
				fHeight_texture,
				fHeight_texture,
				fHeight_texture);
		}
	}
	pHeightMap->UnlockRect(0);
	m_pVB->Unlock();

	D3DXSaveTextureToFile(pFilePath, D3DXIFF_BMP, pHeightMap, nullptr);

	Create_Terrain_NormalVector();
}

void CVIBuffer_Terrain::Update_Terrain_Flat_Rect(LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, RECT & RangeRect, _float fBrushSize, _float3 vPickingPosition, const _tchar * pFilePath)
{
	if (RangeRect.left < 0)
	{
		RangeRect.left = 0;
	}
	if (RangeRect.right > m_iNumVerticesZ*m_fInterval)
	{
		RangeRect.right = m_iNumVerticesZ*m_fInterval;
	}
	if (RangeRect.bottom < 0)
	{
		RangeRect.bottom = 0;
	}
	if (RangeRect.top > m_iNumVerticesX*m_fInterval)
	{
		RangeRect.top = m_iNumVerticesX*m_fInterval;
	}

	VTXNORMAL*			pVertices = nullptr;
	D3DLOCKED_RECT		lockedRect;

	_float	fDistance = 0.f;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	pHeightMap->LockRect(0, &lockedRect, 0, 0);
	
	_double	dHeight = 0.0;
	_uint	iIndexAcc = 0;
	for (_uint i = RangeRect.bottom; i < RangeRect.top; i++)
	{
		for (_uint j = RangeRect.left; j < RangeRect.right; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			dHeight += pVertices[iIndex].vPosition.y;
			iIndexAcc++;
			//pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = m_pVerticesPos[iIndex].y + fHeight*TimeDelta;

		}
	}
	dHeight = dHeight / iIndexAcc;

	for (_uint i = RangeRect.bottom; i < RangeRect.top; i++)
	{
		for (_uint j = RangeRect.left; j < RangeRect.right; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = dHeight;

			if (pVertices[iIndex].vPosition.y <= 255.f)
			{
				pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = dHeight;
			}
			else
			{
				pVertices[iIndex].vPosition.y = m_pVerticesPos[iIndex].y = 255.f;
			}

			_float fHeight_texture = pVertices[iIndex].vPosition.y / 255.f;

			((_ulong*)lockedRect.pBits)[iIndex] = D3DXCOLOR(
				fHeight_texture,
				fHeight_texture,
				fHeight_texture,
				fHeight_texture);

		}
	}
	pHeightMap->UnlockRect(0);
	m_pVB->Unlock();

	D3DXSaveTextureToFile(pFilePath, D3DXIFF_BMP, pHeightMap, nullptr);

	Create_Terrain_NormalVector();

}

void CVIBuffer_Terrain::Create_Terrain_NormalVector()
{
	VTXNORMAL*		pVertices = nullptr;

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	for (int i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (int j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;
			_float3	vDest, vSour, vNormal;

			vDest = pVertices[iIndex + m_iNumVerticesX + 1].vPosition - pVertices[iIndex + m_iNumVerticesX].vPosition;
			vSour = pVertices[iIndex + 1].vPosition - pVertices[iIndex + m_iNumVerticesX + 1].vPosition;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[iIndex + m_iNumVerticesX].vNormal += vNormal;
			pVertices[iIndex + m_iNumVerticesX + 1].vNormal += vNormal;
			pVertices[iIndex + 1].vNormal += vNormal;

			vDest = pVertices[iIndex + 1].vPosition - pVertices[iIndex + m_iNumVerticesX].vPosition;
			vSour = pVertices[iIndex].vPosition - pVertices[iIndex + 1].vPosition;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[iIndex + m_iNumVerticesX].vNormal += vNormal;
			pVertices[iIndex + 1].vNormal += vNormal;
			pVertices[iIndex].vNormal += vNormal;
		}
	}

	for (_uint i = 0; i < m_iNumVerticesX*m_iNumVerticesZ; i++)
	{
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
	}

	m_pVB->Unlock();

}

HRESULT CVIBuffer_Terrain::Update_TerrainFile_Height(const _tchar * pFilePath, LPDIRECT3DTEXTURE9 pTexture)
{
	_ulong* pPixel = nullptr;

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE != hFile)
	{

#pragma region READFILE
		_ulong dwByte;

		BITMAPFILEHEADER	fh;
		ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

		BITMAPINFOHEADER	ih;
		ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

		pPixel = new _ulong[m_iNumVerticesX*m_iNumVerticesZ];
		ReadFile(hFile, pPixel, sizeof(_ulong)*m_iNumVerticesX*m_iNumVerticesZ, &dwByte, nullptr);

		CloseHandle(hFile);
#pragma endregion

	}
	else
	{
		return E_FAIL;
	}

	VTXNORMAL*		pVertices = nullptr;
	D3DLOCKED_RECT		lockedRect;

	pTexture->LockRect(0, &lockedRect, 0, 0);

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i*m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j*m_fInterval, (pPixel[iIndex] & 0x000000ff), i*m_fInterval);

			_float fHeight_texture = pVertices[iIndex].vPosition.y / 255.f;

			((_ulong*)lockedRect.pBits)[iIndex] = D3DXCOLOR(
				fHeight_texture,
				fHeight_texture,
				fHeight_texture,
				fHeight_texture);
		}
	}
	m_pVB->Unlock();
	pTexture->UnlockRect(0);

	Safe_Delete_Array(pPixel);

	Create_Terrain_NormalVector();
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device,_uint iIndexX,_uint iIndexZ, _float fInterval)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(iIndexX, iIndexZ, fInterval)))
	{
		MSGBOX("Failed To Creating CVIBuffer_Terrrain_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFIlePath, _float fIntetval)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(pFIlePath, fIntetval)))
	{
		MSGBOX("Failed To Creating CVIBuffer_Terrain_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CComponent*		pInstance = new CVIBuffer_Terrain(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pQuadTree);
}
