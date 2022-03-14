#pragma once
#include "Mesh.h"

BEGIN(Engine)
class CShader;

class ENGINE_DLL CStatic_Mesh final: public CMesh
{
private:
	explicit CStatic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatic_Mesh(const CStatic_Mesh& rhs);
	virtual  ~CStatic_Mesh() = default;
public:
	_ulong	 Get_NumMaterials() const
	{
		return m_dwNumMaterials;
	}
	LPD3DXMESH Get_Mesh() const
	{
		return m_pMesh;
	}
public:
	virtual	HRESULT	 NativeConstruct_Prototype(const _tchar* pFilePath , const _tchar* pFileName);
	virtual HRESULT	 NativeConstruct(void* pArg = nullptr)	override;
public:
	HRESULT				 SetUp_TextureOnShader(CShader* pShader, D3DXHANDLE hHandle, MESHMATERIALTEXTURE::TYPE eType, const _uint uiMaterialIndex);
	HRESULT				 Render(_uint iMaterialIndex);
	LPDIRECT3DTEXTURE9 Get_Material_Diffuse(MESHMATERIALTEXTURE::TYPE eType, const _uint uiMaterialIndex);
private:
	LPD3DXBUFFER		m_pAdjacency	= nullptr;
	LPD3DXBUFFER		m_pMaterials	= nullptr;
	LPD3DXMESH			m_pMesh			= nullptr;
	_ulong				m_dwNumMaterials= 0;
public:
	vector<MESHMATERIALTEXTURE*>	m_vecMaterials;
public:
	static  CStatic_Mesh*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void* pArg)							override;
	virtual void			Free()										override;

};
END

