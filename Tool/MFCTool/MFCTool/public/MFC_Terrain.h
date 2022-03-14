#pragma once
#include "GameObject.h"
#include "MFC_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(MFC_TOOL)
class CMFC_Terrain final : public CGameObject
{
private:
	explicit CMFC_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMFC_Terrain(const CMFC_Terrain& rhs);
	virtual  ~CMFC_Terrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _uint iIndexX, const _uint iIndexZ, const _tchar* TerrainComponent_Name);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT Render()					override;
public:
	HRESULT	Filter_Texture_Load(const _tchar* pFile);
public:
	const LPDIRECT3DTEXTURE9	Get_FilterTexture() const 
	{
		return	m_pFilterTexture;
	}
	const LPDIRECT3DTEXTURE9	Get_HightMapTexture() const
	{
		return	m_pHightMapTexture;
	}
private:
	HRESULT Add_Component(const _tchar* pPrototypeName);
private:
	void	UpAndDown_Brush(_float TimeDelta, _float fBrushSize, _float3 vPosition);
	void	Flat_Terrain(_float TimeDelta, _float fBrushSize, _float3 vPosition);
	void	Splatting_BrushType(_float TimeDelta, _float fBrushSize, _float3 vPosition);
private:
	CShader*			m_pShaderCom		= nullptr;
	CTexture*			m_pTextureCom		= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CTransform*			m_pTransformCom		= nullptr;
	CVIBuffer_Terrain*	m_pVIBufferCom		= nullptr;
private:
	_float3*			m_vBrushPoint = nullptr;
private:
	_uint					m_iIndexX;
	_uint					m_iIndexZ;
	const _tchar*			m_pTerrainPrototypeName;
public:
	LPDIRECT3DTEXTURE9	m_pFilterTexture	= nullptr;
	LPDIRECT3DTEXTURE9	m_pHightMapTexture	= nullptr;
public:
	static	CMFC_Terrain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device,const _uint iIndexX, const _uint iIndexZ, const _tchar* TerrainPrototype_ComponentName);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
END

