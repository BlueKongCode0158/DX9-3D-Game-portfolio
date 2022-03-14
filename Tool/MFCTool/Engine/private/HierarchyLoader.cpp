#include "..\public\HierarchyLoader.h"

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(pGraphic_Device);
}

HRESULT CHierarchyLoader::NativeConstruct(const _tchar * pFilePath)
{
	m_pMeshFilePath = pFilePath;

	return S_OK;
}

void CHierarchyLoader::Allocate_Name(const _byte * pSourName, _byte ** ppDestName)
{
	_uint iLength = strlen(pSourName);
	*ppDestName = new char[iLength + 1];
	strcpy(*ppDestName, pSourName);
}

STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*		pFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pFrame, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != Name)
	{
		Allocate_Name(Name, &pFrame->Name);
	}

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	*ppNewFrame = pFrame;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pMeshConatiner = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pMeshConatiner, sizeof(D3DXMESHCONTAINER_DERIVED));

	if (nullptr != Name)
		Allocate_Name(Name, &pMeshConatiner->Name);

	D3DVERTEXELEMENT9			Element[] =
	{
		{0,0, D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 },
		{0,12,D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL , 0 },
		{0,24,D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD ,0 },
		{0,32,D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TANGENT , 0 },
		D3DDECL_END()
	}; // 해당 방법은 나중에 NORMAL 할 때 다시 알려주신다고 하셨음. ->알려주심

	//D3DVERTEXELEMENT9	Element[MAX_FVF_DECL_SIZE];
	//pMeshData->pMesh->GetDeclaration(Element);

	if (FAILED(pMeshData->pMesh->CloneMesh(pMeshData->pMesh->GetOptions(), Element, m_pGraphic_Device, &pMeshConatiner->MeshData.pMesh)))
	{
		return E_FAIL;
	}
	pMeshConatiner->MeshData.Type	= D3DXMESHTYPE_MESH;
	pMeshConatiner->NumMaterials	= NumMaterials;
	pMeshConatiner->ppMaterialTextures = new MESHMATERIALTEXTURE*[NumMaterials];
	
	for (_uint i = 0; i < NumMaterials; i++)
	{
		pMeshConatiner->ppMaterialTextures[i] = new MESHMATERIALTEXTURE;
	}

	for (_uint i = 0; i < pMeshConatiner->NumMaterials; i++)
	{
		_tchar		szFullPath[MAX_PATH] = TEXT("");
		lstrcpy(szFullPath, m_pMeshFilePath);

		_tchar		szTextureFileName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, pMaterials[i].pTextureFilename, strlen(pMaterials[i].pTextureFilename), szTextureFileName, MAX_PATH);

		lstrcat(szFullPath, szTextureFileName);

		_uint		iLength = 0;
		iLength = lstrlen(szFullPath);
		_uint uiIndex = 0;

		for (_uint i = iLength; i >= 0; i--)
		{
			if (szFullPath[i] == L'.')
			{
				uiIndex = i - 1;
				break;
			}
		}

		szFullPath[uiIndex] = L'D';
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshConatiner->ppMaterialTextures[i]->pDiffuseMap);

		szFullPath[uiIndex] = L'N';
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshConatiner->ppMaterialTextures[i]->pNormalMap);
		
		szFullPath[uiIndex] = L'S';
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshConatiner->ppMaterialTextures[i]->pNormalMap);

	}

	pMeshConatiner->pAdjacency = new _ulong[pMeshConatiner->MeshData.pMesh->GetNumFaces() * 3];
	memcpy(pMeshConatiner->pAdjacency, pAdjacency, sizeof(_ulong) * pMeshConatiner->MeshData.pMesh->GetNumFaces() * 3);

	if (FAILED(D3DXComputeTangent(pMeshConatiner->MeshData.pMesh, 0, 0, 0, 1, pMeshConatiner->pAdjacency)))
	{
		return E_FAIL;
	}


	pMeshConatiner->pSkinInfo = pSkinInfo;
	Safe_AddRef(pMeshConatiner->pSkinInfo);
	pMeshConatiner->pSkinInfo->SetDeclaration(Element);


	pMeshConatiner->iNumBones = pMeshConatiner->pSkinInfo->GetNumBones();
	pMeshConatiner->pOffsetMatrices = new _matrix[pMeshConatiner->iNumBones];

	_int iIndexSize = pMeshConatiner->iNumBones;
	for (_uint i = 0; i < iIndexSize; i++)
	{
		pMeshConatiner->pOffsetMatrices[i] = *pMeshConatiner->pSkinInfo->GetBoneOffsetMatrix(i);
	}

	pMeshConatiner->pRenderingMatrices = new _matrix[pMeshConatiner->iNumBones];
	pMeshConatiner->ppCombinedTransformationMatrices = new _matrix*[pMeshConatiner->iNumBones];

	pMeshConatiner->MeshData.pMesh->CloneMesh(pMeshConatiner->MeshData.pMesh->GetOptions(), Element, m_pGraphic_Device, &pMeshConatiner->pOriginalMesh);

	*ppNewMeshContainer = pMeshConatiner;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (false == reinterpret_cast<D3DXFRAME_DERIVED*>(pFrameToFree)->isCloned)
	{
		Safe_Delete_Array(pFrameToFree->Name);

		if (nullptr != pFrameToFree->pMeshContainer)
		{
			DestroyMeshContainer(pFrameToFree->pMeshContainer);
		}
	}

	if (nullptr != pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (nullptr != pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	Safe_Delete(pFrameToFree);
	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*	pMeshContainer = reinterpret_cast<D3DXMESHCONTAINER_DERIVED*>(pMeshContainerToFree);

	for (_uint i = 0; i < pMeshContainer->NumMaterials; i++)
	{
		Safe_Release(pMeshContainer->ppMaterialTextures[i]->pDiffuseMap);
		Safe_Release(pMeshContainer->ppMaterialTextures[i]->pNormalMap);
		Safe_Release(pMeshContainer->ppMaterialTextures[i]->pSpecularMap);

		Safe_Delete(pMeshContainer->ppMaterialTextures[i]);
	}

	Safe_Delete_Array(pMeshContainer->ppMaterialTextures);
	Safe_Release(pMeshContainer->pOriginalMesh);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrices);
	Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);
	
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete(pMeshContainer);
	return S_OK;
}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath)
{
	CHierarchyLoader* pInstance = new CHierarchyLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(pFilePath)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CHierarchyLoader");
	}
	return pInstance;
}

void CHierarchyLoader::Free()
{
	Safe_Release(m_pGraphic_Device);
}
