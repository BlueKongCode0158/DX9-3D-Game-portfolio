#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain final: public CVIBuffer
{
private:
	explicit  CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit  CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual	  ~CVIBuffer_Terrain() = default;
public:
	D3DXPLANE	Get_Plane (_float3& vPosition)const;
public:
	HRESULT		Culling(class CFrustum* pFrustum, class CTransform* pTransform);
public:
	virtual	HRESULT NativeConstruct_Prototype(_uint iIndexX, _uint iIndexY, _float fInterval = 1.f);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pFilePath, _float fInterval = 1.f);
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
public:
	HRESULT	Update_Terrain_Height(_uint eType, LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, _float fBrushSize ,_float3 vPicking_Position, const _tchar* pFilePath);
	HRESULT Update_Terrain_Aver(_uint eType, LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, _float fBrushSize, _float3 vPicking_Position, const _tchar* pFilePath);
private:
	void	Update_Terrain_Brush_Rect(LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, RECT& RangeRect, const _tchar* pFilePath);
	void	Update_Terrain_Brush_Circle(LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, RECT& RangeRect, _float fBrushSize, _float3 vPickingPosition, const _tchar* pFilePath);
	void	Update_Terrain_Flat_Circle(LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, RECT& RangeRect, _float fBrushSize, _float3 vPickingPosition, const _tchar* pFilePath);
	void	Update_Terrain_Flat_Rect(LPDIRECT3DTEXTURE9 pHeightMap, _float TimeDelta, RECT& RangeRect, _float fBrushSize, _float3 vPickingPosition, const _tchar* pFilePath);
	void	Create_Terrain_NormalVector();
public:
	HRESULT Update_TerrainFile_Height(const _tchar* pFilePath, LPDIRECT3DTEXTURE9 pTexture);
#pragma region PLANE
private:
	_uint	m_iNumVerticesX = 0;
	_uint	m_iNumVerticesZ = 0;
	_float	m_fInterval		= 0.f;
#pragma endregion
private:
	class CQuadTree*	m_pQuadTree = nullptr;
public:
	static CVIBuffer_Terrain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iIndexX, _uint iIndexZ, _float fInterval = 1.f);
	static CVIBuffer_Terrain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFIlePath, _float fIntetval = 1.f);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;

};
END
