#include "..\public\Static_Mesh.h"
#include "Shader.h"

CStatic_Mesh::CStatic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMesh(pGraphic_Device)
{

}

CStatic_Mesh::CStatic_Mesh(const CStatic_Mesh & rhs)
	:CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_vecMaterials(rhs.m_vecMaterials)
{
	for (auto& pMeshMaterialTexture : m_vecMaterials)
	{
		Safe_AddRef(pMeshMaterialTexture->pDiffuseMap);
		Safe_AddRef(pMeshMaterialTexture->pNormalMap);
		Safe_AddRef(pMeshMaterialTexture->pSpecularMap);
	}

	Safe_AddRef(m_pMesh);
}

HRESULT CStatic_Mesh::NativeConstruct_Prototype(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = TEXT("");

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, &m_pAdjacency, &m_pMaterials, nullptr, &m_dwNumMaterials,&m_pMesh)))
	{

		return E_FAIL;
	}

	for (_uint i = 0; i < m_dwNumMaterials; i++)
	{
		D3DXMATERIAL*	pMaterial = reinterpret_cast<D3DXMATERIAL*>(m_pMaterials->GetBufferPointer());
		MESHMATERIALTEXTURE* pMeshMaterial = new MESHMATERIALTEXTURE;
		ZeroMemory(pMeshMaterial, sizeof(MESHMATERIALTEXTURE));

		_tchar		szTextureFileName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, pMaterial->pTextureFilename, strlen(pMaterial->pTextureFilename), szTextureFileName, sizeof(_tchar)*MAX_PATH);

		_tchar szFileName[MAX_PATH] = TEXT("");

		wsprintf(szFileName, szTextureFileName, L"D");
		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshMaterial->pDiffuseMap);

		

		wsprintf(szFileName, szTextureFileName, L"N");
		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshMaterial->pNormalMap);
		
		

		wsprintf(szFileName, szTextureFileName, L"S");
		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshMaterial->pSpecularMap);
		

		m_vecMaterials.push_back(pMeshMaterial);
	}
	return S_OK;
}

HRESULT CStatic_Mesh::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CStatic_Mesh::SetUp_TextureOnShader(CShader * pShader, D3DXHANDLE hHandle, MESHMATERIALTEXTURE::TYPE eType, const _uint uiMaterialIndex)
{
	LPDIRECT3DTEXTURE9		pTexture = nullptr;

	switch (eType)
	{
	case Engine::MeshMaterialTexture::TYPE_DIFFUSE:
		pTexture = m_vecMaterials[uiMaterialIndex]->pDiffuseMap;
		break;
	case Engine::MeshMaterialTexture::TYPE_NORMAL:
		pTexture = m_vecMaterials[uiMaterialIndex]->pNormalMap;
		break;
	case Engine::MeshMaterialTexture::TYPE_SPECULAR:
		pTexture = m_vecMaterials[uiMaterialIndex]->pSpecularMap;
		break;
	case Engine::MeshMaterialTexture::TYPE_END:
		MSGBOX("Material is Over Number");
		return E_FAIL;
		break;
	default:
		break;
	}

	if (FAILED(pShader->SetUp_TextureConstantTable(hHandle, pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CStatic_Mesh::Render(_uint iMaterialIndex)
{
	m_pMesh->DrawSubset(iMaterialIndex);
	return S_OK;
}

LPDIRECT3DTEXTURE9 CStatic_Mesh::Get_Material_Diffuse(MESHMATERIALTEXTURE::TYPE eType, const _uint uiMaterialIndex)
{
	LPDIRECT3DTEXTURE9		pTexture = nullptr;

	pTexture = m_vecMaterials[uiMaterialIndex]->pDiffuseMap;
	return pTexture;
}

CStatic_Mesh * CStatic_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStatic_Mesh* pInstance = new CStatic_Mesh(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(pFilePath,pFileName)))
	{
		MSGBOX("Failed to Creating Staic_Mesh_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CStatic_Mesh::Clone(void * pArg)
{
	CComponent* pInstance = new CStatic_Mesh(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Static_Mesh_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatic_Mesh::Free()
{
	__super::Free();

	for (auto& pMeshMaterialTexture : m_vecMaterials)
	{
		Safe_Release(pMeshMaterialTexture->pDiffuseMap);
		Safe_Release(pMeshMaterialTexture->pNormalMap);
		Safe_Release(pMeshMaterialTexture->pSpecularMap);

		if (false == m_isClone)
		{
			Safe_Delete(pMeshMaterialTexture);
		}
	}
	m_vecMaterials.clear();

	Safe_Release(m_pMesh);
}
