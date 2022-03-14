#include "..\public\Dynamic_Mesh.h"
#include "HierarchyLoader.h"
#include "Animation.h"
#include "Shader.h"

CDynamic_Mesh::CDynamic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CMesh(pGraphic_Device)
{

}

CDynamic_Mesh::CDynamic_Mesh(const CDynamic_Mesh & rhs)
	:CMesh(rhs)
	,m_pRootFrame(rhs.m_pRootFrame)
	,m_pRootFrame2(rhs.m_pRootFrame2)
	,m_PivotMatrix(rhs.m_PivotMatrix)
	,m_ConnectFrame(rhs.m_ConnectFrame)
	,m_ConnectFrameMatrix(rhs.m_ConnectFrameMatrix)
	,m_vecMeshContainers(rhs.m_vecMeshContainers)
	,m_pAnimation_Upper(rhs.m_pAnimation_Upper->Clone())
	,m_pAnimation_Lower(rhs.m_pAnimation_Lower->Clone())
	,m_pLoader(rhs.m_pLoader)
{
	Safe_AddRef(m_pLoader);
	_uint iNumMeshContainer = m_vecMeshContainers.size();

	for (_uint i = 0; i < iNumMeshContainer; i++)
	{
		Safe_AddRef(m_vecMeshContainers[i]->MeshData.pMesh);
		Safe_AddRef(m_vecMeshContainers[i]->pOriginalMesh);

		for (_uint j = 0; j < m_vecMeshContainers[i]->NumMaterials; j++)
		{
			Safe_AddRef(m_vecMeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
			Safe_AddRef(m_vecMeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
			Safe_AddRef(m_vecMeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
		}
	}
}

const _matrix * CDynamic_Mesh::Get_BoneMatrixPointer(const char * pBoneName)
{
	const _matrix* pMatrix = &reinterpret_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(m_pRootFrame, pBoneName))->CombinedTransformationMatrix;
	if (nullptr != pMatrix)
	{
		return pMatrix;
	}
	MSGBOX(" Bone is Not Finding ");
	return nullptr;
}

const _matrix * CDynamic_Mesh::Get_BoneMatrixPointer_Upper(const char * pBoneName)
{
	const _matrix* pMatrix = &reinterpret_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(m_pRootFrame2, pBoneName))->CombinedTransformationMatrix;
	if (nullptr != pMatrix)
	{
		return pMatrix;
	}
	MSGBOX(" Bone is Not Finding ");
	return nullptr;
}

const _matrix * CDynamic_Mesh::Get_BoneMatrixPointer_Lower(const char * pBoneName)
{
	const _matrix* pMatrix = &reinterpret_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(m_pRootFrame, pBoneName))->CombinedTransformationMatrix;
	if (nullptr != pMatrix)
	{
		return pMatrix;
	}
	MSGBOX(" Bone is Not Finding ");

	return nullptr;
}

HRESULT CDynamic_Mesh::NativeConstruct_Prototype(const _tchar* pFilePath ,const _tchar* pFileName)
{
	_tchar szFullPath[MAX_PATH] = TEXT("");

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device,pFilePath);
	LPD3DXANIMATIONCONTROLLER pAnimationController		= nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device,
		m_pLoader, nullptr, &m_pRootFrame, &pAnimationController)))
	{
		/*ID3DXAnimationController 추가할 거임.*/
		return E_FAIL;

	}
	m_pAnimation_Lower = CAnimation::Create(m_pGraphic_Device, pAnimationController);
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}

	m_pAnimation_Upper = m_pAnimation_Lower->Clone();
	if (nullptr == m_pAnimation_Upper)
	{
		return E_FAIL;
	}
	
	D3DXMatrixRotationY(&m_PivotMatrix, D3DXToRadian(180.0f));

	SetUp_CombinedTransformationMatrices(m_pRootFrame, &m_PivotMatrix,"pelvis");
	SetUp_CombinedTransformationMatricesPointer(m_pRootFrame);

	return S_OK;
}

HRESULT CDynamic_Mesh::NativeConstruct(void * pArg)
{
	m_vecCombinedTransformationMatrixPointers.resize(m_vecMeshContainers.size());

	D3DXFRAME_DERIVED*	pOriginalFrame = reinterpret_cast<D3DXFRAME_DERIVED*>(m_pRootFrame);
	if (FAILED(Clone_Frame(pOriginalFrame)))
	{
		return E_FAIL;
	}

	for (_uint i = 0; i < m_vecMeshContainers.size(); i++)
	{
		m_vecCombinedTransformationMatrixPointers[i] = new _matrix*[m_vecMeshContainers[i]->iNumBones];
	}

	_uint	iIndex = 0;

	if (nullptr != m_pRootFrame)
	{
		SetUp_CloneCombinedTransformationMatricesPointer(m_pRootFrame, iIndex);
	}

	SetUp_BoneFrame(m_pRootFrame, "pelvis");

	m_pAnimation_Lower->Bind_Frame(reinterpret_cast<D3DXFRAME_DERIVED*>(m_pRootFrame));
	if (nullptr != m_pRootFrame2)
	{
		m_pAnimation_Upper->Bind_Frame(reinterpret_cast<D3DXFRAME_DERIVED*>(m_pRootFrame2));
		ConnectFrame();
	}
	return S_OK;
}

HRESULT CDynamic_Mesh::SetUp_TextureOnShader(CShader * pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMeshContainerIndex, _uint iMaterialIndex)
{
	LPDIRECT3DTEXTURE9	pTexture = nullptr;

	switch (eType)
	{
	case Engine::MeshMaterialTexture::TYPE_DIFFUSE:
		pTexture = m_vecMeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pDiffuseMap;
		break;
	case Engine::MeshMaterialTexture::TYPE_NORMAL:
		pTexture = m_vecMeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pNormalMap;
		break;
	case Engine::MeshMaterialTexture::TYPE_SPECULAR:
		pTexture = m_vecMeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pSpecularMap;
		break;
	default:
		MSGBOX("Warring Number. This Number is Not Texture_Material");
		break;
	}
	if (FAILED(pShader->SetUp_TextureConstantTable(hParameter, pTexture)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CDynamic_Mesh::Update_SkinnedMesh(_uint iMeshContainerIndex)
{
	for (_uint i = 0; i < m_vecMeshContainers[iMeshContainerIndex]->iNumBones; i++)
	{
		m_vecMeshContainers[iMeshContainerIndex]->pRenderingMatrices[i] = m_vecMeshContainers[iMeshContainerIndex]->pOffsetMatrices[i] * (*m_vecCombinedTransformationMatrixPointers[iMeshContainerIndex][i]);
	}

	void*	pSour = nullptr;
	void*	pDest = nullptr;

	m_vecMeshContainers[iMeshContainerIndex]->pOriginalMesh->LockVertexBuffer(0, &pSour);
	m_vecMeshContainers[iMeshContainerIndex]->MeshData.pMesh->LockVertexBuffer(0, &pDest);
	
	m_vecMeshContainers[iMeshContainerIndex]->pSkinInfo->UpdateSkinnedMesh(m_vecMeshContainers[iMeshContainerIndex]->pRenderingMatrices, nullptr, pSour, pDest);
	
	m_vecMeshContainers[iMeshContainerIndex]->pOriginalMesh->UnlockIndexBuffer();
	m_vecMeshContainers[iMeshContainerIndex]->MeshData.pMesh->UnlockIndexBuffer();
	return S_OK;
}

HRESULT CDynamic_Mesh::Set_AnimationIndex(_uint iAnimIndex)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	if (FAILED(m_pAnimation_Lower->Set_AnimationIndex(iAnimIndex)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CDynamic_Mesh::Set_AnimationIndex(_uint iAnimIndex_Upper, _uint iAnimIndex_Lower)
{
	if (nullptr == m_pAnimation_Upper &&
		nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	if (FAILED(m_pAnimation_Upper->Set_AnimationIndex(iAnimIndex_Upper)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pAnimation_Lower->Set_AnimationIndex(iAnimIndex_Lower)))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CDynamic_Mesh::Play_Animation(_double TimeDelta)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	if (FAILED(m_pAnimation_Lower->Play_Animation(TimeDelta)))
	{
		return E_FAIL;
	}

	if (nullptr != m_pAnimation_Upper)
	{
		if (FAILED(m_pAnimation_Upper->Play_Animation(TimeDelta)))
		{
			return E_FAIL;
		}
	}

	Update_CombinedTransformMatrices(m_pRootFrame, m_PivotMatrix);

	return S_OK;
}

HRESULT CDynamic_Mesh::Play_Animation(_double TimeDelta, _float fUpperAnimSpeed, _float fLowerSpeed)
{
	if (nullptr == m_pAnimation_Lower &&
		nullptr == m_pAnimation_Upper)
	{
		return E_FAIL;
	}
	if (FAILED(m_pAnimation_Lower->Play_Animation(fLowerSpeed * TimeDelta)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pAnimation_Upper->Play_Animation(fUpperAnimSpeed * TimeDelta)))
	{
		return E_FAIL;
	}

	Update_CombinedTransformMatrices(m_pRootFrame, m_PivotMatrix);

	return S_OK;
}

HRESULT CDynamic_Mesh::Set_FrameAnimation(_double AnimationPosition)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	return m_pAnimation_Lower->Set_FrameAnimation(AnimationPosition);
}

HRESULT CDynamic_Mesh::Set_FrameAnimation_Lower(_double AnimationPosition)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	return m_pAnimation_Lower->Set_FrameAnimation(AnimationPosition);
}

HRESULT CDynamic_Mesh::Set_FrameAnimation_Upper(_double AnimationPosition)
{
	if (nullptr == m_pAnimation_Upper)
	{
		return E_FAIL;
	}
	return m_pAnimation_Upper->Set_FrameAnimation(AnimationPosition);
}

_bool CDynamic_Mesh::Get_AnimationStop()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return false;
	}
	return m_pAnimation_Lower->Get_AnimationStop();
}

_bool CDynamic_Mesh::Get_AnimationStop_Upper()
{
	if (nullptr == m_pAnimation_Upper)
	{
		return false;
	}

	return m_pAnimation_Upper->Get_AnimationStop();
}

_bool CDynamic_Mesh::Get_AnimationStop_Lower()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return false;
	}

	return m_pAnimation_Lower->Get_AnimationStop();
}

HRESULT CDynamic_Mesh::Reset_FrameAnimation()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}

	return m_pAnimation_Lower->Reset_FrameAnimation();
}

HRESULT CDynamic_Mesh::Reset_FrameAnimation_Upper()
{
	if (nullptr == m_pAnimation_Upper)
	{
		return E_FAIL;
	}

	return m_pAnimation_Upper->Reset_FrameAnimation();
}

HRESULT CDynamic_Mesh::Reset_FrameAnimation_Lower()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}

	return m_pAnimation_Lower->Reset_FrameAnimation();
}

void CDynamic_Mesh::Set_AnimationStop(_bool isStop)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return;
	}
	m_pAnimation_Lower->Set_AnimationStop(isStop);

	if (nullptr != m_pAnimation_Upper)
	{
		m_pAnimation_Upper->Set_AnimationStop(isStop);
	}

	return;
}

_float CDynamic_Mesh::Play_Animation_Per()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	return m_pAnimation_Lower->Play_Animation_Per();
}

_float CDynamic_Mesh::Play_Animation_Per_Upper()
{
	if (nullptr == m_pAnimation_Upper)
	{
		return E_FAIL;
	}
	return m_pAnimation_Upper->Play_Animation_Per();
}

_float CDynamic_Mesh::Play_Animation_Per_Lower()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return E_FAIL;
	}
	return m_pAnimation_Lower->Play_Animation_Per();
}

_uint CDynamic_Mesh::Get_CurrentAnimationIndex()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return 0;
	}
	return m_pAnimation_Lower->Get_CurrentAnimationIndex();
}

_uint CDynamic_Mesh::Get_CurrentAnimationIndex_Upper()
{
	if (nullptr == m_pAnimation_Upper)
	{
		return 0;
	}
	return m_pAnimation_Upper->Get_CurrentAnimationIndex();
}

_uint CDynamic_Mesh::Get_CurrentAnimationIndex_Lower()
{
	if (nullptr == m_pAnimation_Lower)
	{
		return 0;
	}
	return m_pAnimation_Lower->Get_CurrentAnimationIndex();
}

HRESULT CDynamic_Mesh::ConnectFrame()
{
	m_isConnect = true;

	LPD3DXFRAME pFrame = D3DXFrameFind(m_pRootFrame, "pelvis");
	if (m_pRootFrame->pFrameSibling == nullptr)
	{
		pFrame->pFrameSibling = m_pRootFrame2;
		return S_OK;
	}
	return S_OK;
}

HRESULT CDynamic_Mesh::BreakFrame()
{
	m_isConnect = false;

	LPD3DXFRAME pFrame = D3DXFrameFind(m_pRootFrame, "pelvis");
	if (pFrame->pFrameSibling != nullptr)
	{
		pFrame->pFrameSibling = NULL;
		return S_OK;
	}
	return S_OK;
}

_bool CDynamic_Mesh::Animation_State_End(_uint iAnimIndex)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return false;
	}
	return m_pAnimation_Lower->Animation_State_End(iAnimIndex);
}

_bool CDynamic_Mesh::Animation_State_End_Upper(_uint iAnimIndex)
{
	if (nullptr == m_pAnimation_Upper)
	{
		return false;
	}
	return m_pAnimation_Upper->Animation_State_End(iAnimIndex);
}

_bool CDynamic_Mesh::Animation_State_End_Lower(_uint iAnimIndex)
{
	if (nullptr == m_pAnimation_Lower)
	{
		return false;
	}
	return m_pAnimation_Lower->Animation_State_End(iAnimIndex);
}

void CDynamic_Mesh::Update_CombinedTransformMatrices(LPD3DXFRAME pFrame, _matrix ParentMatrix)
{
	D3DXFRAME_DERIVED* pFrame_Derived = reinterpret_cast<D3DXFRAME_DERIVED*>(pFrame);
	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * ParentMatrix;

	if (nullptr != pFrame_Derived->pFrameFirstChild)
	{
		/* 내 자식들이므로 나의 matrix를 */
		Update_CombinedTransformMatrices(pFrame_Derived->pFrameFirstChild, pFrame_Derived->CombinedTransformationMatrix);
	}
	if (nullptr != pFrame_Derived->pFrameSibling)
	{
		/* 같은 부모를 가지고 있는 형제*/
		Update_CombinedTransformMatrices(pFrame_Derived->pFrameSibling, ParentMatrix);
	}
}

void CDynamic_Mesh::SetUp_BoneFrame(LPD3DXFRAME pFrame, const char * BodyName)
{
	if (nullptr != pFrame->Name && nullptr != BodyName)
	{
		if (!strcmp(pFrame->Name, BodyName))
		{
			m_pRootFrame2 = pFrame->pFrameSibling;
			pFrame->pFrameSibling = NULL;
		}
	}
	if (nullptr != pFrame->pFrameSibling)
	{
		SetUp_BoneFrame(pFrame->pFrameSibling, BodyName);
	}
	if (nullptr != pFrame->pFrameFirstChild)
	{
		SetUp_BoneFrame(pFrame->pFrameFirstChild, BodyName);
	}
}

void CDynamic_Mesh::SetUp_CombinedTransformationMatrices(LPD3DXFRAME pFrame, _matrix* ParentMatrix, const char* BodyName)
{
	D3DXFRAME_DERIVED* pFrame_Derived = reinterpret_cast<D3DXFRAME_DERIVED*>(pFrame);
	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *ParentMatrix;

	if (nullptr != pFrame->Name && nullptr != BodyName)
	{
		if (!strcmp(pFrame->Name, BodyName))
		{
			m_ConnectFrameMatrix = ParentMatrix;
			m_ConnectFrame = pFrame;
		}
	}

	if (nullptr != pFrame_Derived->pFrameFirstChild)
	{
		/* 내 자식들이므로 나의 matrix를 */
		SetUp_CombinedTransformationMatrices(pFrame_Derived->pFrameFirstChild, &pFrame_Derived->CombinedTransformationMatrix, BodyName);
	}
	if (nullptr != pFrame_Derived->pFrameSibling)
	{
		/* 같은 부모를 가지고 있는 형제*/
		SetUp_CombinedTransformationMatrices(pFrame_Derived->pFrameSibling, ParentMatrix, BodyName);
	}
}

void CDynamic_Mesh::SetUp_CombinedTransformationMatricesPointer(LPD3DXFRAME pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer_Derived = reinterpret_cast<D3DXMESHCONTAINER_DERIVED*>(pFrame->pMeshContainer);
		m_vecMeshContainers.push_back(pMeshContainer_Derived);

		for (_uint i = 0; i < pMeshContainer_Derived->iNumBones; i++)
		{
			const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*		pFindFrame = reinterpret_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(m_pRootFrame, pBoneName));
			pMeshContainer_Derived->ppCombinedTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
		}

	}

	if (nullptr != pFrame->pFrameFirstChild)
	{
		SetUp_CombinedTransformationMatricesPointer(pFrame->pFrameFirstChild);
	}
	if (nullptr != pFrame->pFrameSibling)
	{
		SetUp_CombinedTransformationMatricesPointer(pFrame->pFrameSibling);
	}
}

void CDynamic_Mesh::SetUp_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint & iIndex)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pMeshContainer_Derived = reinterpret_cast<D3DXMESHCONTAINER_DERIVED*>(pFrame->pMeshContainer);

		for (_uint i = 0; i < pMeshContainer_Derived->iNumBones; i++)
		{
			const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*	pFindFrame = reinterpret_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(m_pRootFrame, pBoneName));

			m_vecCombinedTransformationMatrixPointers[iIndex][i] = &pFindFrame->CombinedTransformationMatrix;
		}
		++iIndex;
	}

	if (nullptr != pFrame->pFrameFirstChild)
	{
		SetUp_CloneCombinedTransformationMatricesPointer(pFrame->pFrameFirstChild, iIndex);
	}
	if (nullptr != pFrame->pFrameSibling)
	{
		SetUp_CloneCombinedTransformationMatricesPointer(pFrame->pFrameSibling, iIndex);
	}
}

void CDynamic_Mesh::Update_Pivot()
{
	if (m_pRootFrame == nullptr)
	{
		return;
	}

	Update_CombinedTransformMatrices(m_pRootFrame, m_PivotMatrix);
}

HRESULT CDynamic_Mesh::Render(_uint iMeshContainerIndex, _uint iMaterialIndex)
{
	_uint		iNumMeshContainer = m_vecMeshContainers.size();

	m_vecMeshContainers[iMeshContainerIndex]->MeshData.pMesh->DrawSubset(iMaterialIndex);
	return S_OK;
}

HRESULT CDynamic_Mesh::Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame)
{
	m_pRootFrame = new D3DXFRAME_DERIVED;
	memcpy(m_pRootFrame, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

	reinterpret_cast<D3DXFRAME_DERIVED*>(m_pRootFrame)->isCloned = true;

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		m_pRootFrame->pFrameFirstChild = new D3DXFRAME_DERIVED;
		Clone_Frame(reinterpret_cast<D3DXFRAME_DERIVED*>(pOriginalFrame->pFrameFirstChild), reinterpret_cast<D3DXFRAME_DERIVED*>(m_pRootFrame->pFrameFirstChild));
	}
	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		m_pRootFrame->pFrameSibling = new D3DXFRAME_DERIVED;
		Clone_Frame(reinterpret_cast<D3DXFRAME_DERIVED*>(pOriginalFrame->pFrameSibling), reinterpret_cast<D3DXFRAME_DERIVED*>(m_pRootFrame->pFrameSibling));
	}
	return S_OK;
}

void CDynamic_Mesh::Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame, D3DXFRAME_DERIVED * pOut)
{
	memcpy(pOut, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));
	pOut->isCloned = true;

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		pOut->pFrameFirstChild = new D3DXFRAME_DERIVED;
		Clone_Frame(reinterpret_cast<D3DXFRAME_DERIVED*>(pOriginalFrame->pFrameFirstChild), reinterpret_cast<D3DXFRAME_DERIVED*>(pOut->pFrameFirstChild));
	}
	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		pOut->pFrameSibling = new D3DXFRAME_DERIVED;
		Clone_Frame(reinterpret_cast<D3DXFRAME_DERIVED*>(pOriginalFrame->pFrameSibling), reinterpret_cast<D3DXFRAME_DERIVED*>(pOut->pFrameSibling));
	}
}

CDynamic_Mesh * CDynamic_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamic_Mesh* pInstance = new CDynamic_Mesh(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(pFilePath, pFileName)))
	{
		MSGBOX("Failed to Creating CDynamic_Mesh_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CDynamic_Mesh::Clone(void * pArg)
{
	CComponent* pInstance = new CDynamic_Mesh(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CDynamic_Mesh_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDynamic_Mesh::Free()
{
	__super::Free();

	_uint iNumMeshContainer = m_vecMeshContainers.size();

	for (_uint i = 0; i < iNumMeshContainer; i++)
	{
		Safe_Release(m_vecMeshContainers[i]->MeshData.pMesh);
		Safe_Release(m_vecMeshContainers[i]->pOriginalMesh);

		for (_uint j = 0; j < m_vecMeshContainers[i]->NumMaterials; j++)
		{
			Safe_Release(m_vecMeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
			Safe_Release(m_vecMeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
			Safe_Release(m_vecMeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
		}
	}
	m_vecMeshContainers.clear();
	m_pLoader->DestroyFrame(m_pRootFrame);
	if (nullptr != m_pRootFrame2 && false == m_isConnect)
	{
		m_pLoader->DestroyFrame(m_pRootFrame2);
	}

	for (auto& ppCombinedMatrixPointers : m_vecCombinedTransformationMatrixPointers)
	{
		Safe_Delete_Array(ppCombinedMatrixPointers);
	}
	m_vecCombinedTransformationMatrixPointers.clear();

	Safe_Release(m_pLoader);
	Safe_Release(m_pAnimation_Upper);
	Safe_Release(m_pAnimation_Lower);
}
