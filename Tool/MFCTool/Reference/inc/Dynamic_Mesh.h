#pragma once
#include "Mesh.h"

BEGIN(Engine)
class CShader;
class CAnimation;
class CHierarchyLoader;
class ENGINE_DLL CDynamic_Mesh final : public CMesh
{
private:
	explicit CDynamic_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDynamic_Mesh(const CDynamic_Mesh& rhs);
	virtual  ~CDynamic_Mesh() = default;
public:
	_uint	Get_NumMeshContainer() const
	{
		return m_vecMeshContainers.size();
	}
	_uint	Get_NumMaterials(_uint iMeshContainerIndex) const
	{
		return m_vecMeshContainers[iMeshContainerIndex]->NumMaterials;
	}
	LPD3DXMESH	Get_Mesh(_uint iMeshContainerIndex) const
	{
		return m_vecMeshContainers[iMeshContainerIndex]->pOriginalMesh;
	}
	const _matrix* Get_BoneMatrixPointer(const char* pBoneName);
	const _matrix* Get_BoneMatrixPointer_Upper(const char* pBoneName);
	const _matrix* Get_BoneMatrixPointer_Lower(const char* pBoneName);
public:
	virtual	HRESULT NativeConstruct_Prototype(const _tchar* pFilePath, const _tchar* pFileName);
	virtual HRESULT NativeConstruct(void* pArg = nullptr)				override;
public:
	HRESULT	SetUp_TextureOnShader(CShader*	pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMeshContainerIndex, _uint iMaterialIndex);
	HRESULT	Update_SkinnedMesh(_uint iMeshContainerIndex);
	HRESULT	Set_AnimationIndex(_uint iAnimIndex);
	HRESULT Set_AnimationIndex(_uint iAnimIndex_Upper,_uint iAnimIndex_Lower);
	HRESULT	Play_Animation(_double TimeDelta);
	HRESULT	Play_Animation(_double TimeDelta, _float fUpperAnimSpeed, _float fLowerSpeed);
public:
	HRESULT	Set_FrameAnimation(_double AnimationPosition);
	HRESULT	Set_FrameAnimation_Lower(_double AnimationPosition);
	HRESULT	Set_FrameAnimation_Upper(_double AnimationPosition);
public:
	_bool Get_AnimationStop();
	_bool Get_AnimationStop_Upper();
	_bool Get_AnimationStop_Lower();
public:
	HRESULT	Reset_FrameAnimation();
	HRESULT	Reset_FrameAnimation_Upper();
	HRESULT	Reset_FrameAnimation_Lower();
public:
	void	Set_PivotMatrix(_matrix tPivot)
	{
		m_PivotMatrix = tPivot;
	}
public:
	void	Set_AnimationStop(_bool isStop);
public:
	_float	Play_Animation_Per();
	_float	Play_Animation_Per_Upper();
	_float	Play_Animation_Per_Lower();
public:
	_uint	Get_CurrentAnimationIndex();
	_uint	Get_CurrentAnimationIndex_Upper();
	_uint	Get_CurrentAnimationIndex_Lower();
public:
	HRESULT	ConnectFrame();
	HRESULT	BreakFrame();
public:
	_bool	Animation_State_End(_uint iAnimIndex);
	_bool	Animation_State_End_Upper(_uint iAnimIndex);
	_bool	Animation_State_End_Lower(_uint iAnimIndex);
public:
	void Update_CombinedTransformMatrices(LPD3DXFRAME pFrame, _matrix ParentMatrix);
	void SetUp_BoneFrame(LPD3DXFRAME pFrame, const char* BodyName);
	void SetUp_CombinedTransformationMatrices(LPD3DXFRAME pFrame, _matrix *ParentMatrix, const char* BodyName);
	void SetUp_CombinedTransformationMatricesPointer(LPD3DXFRAME pFrame);
	void SetUp_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint& iIndex);
public:
	void Update_Pivot();
public:
	virtual HRESULT Render(_uint iMeshContainerIndex, _uint iMaterialIndex);
private:
	LPD3DXFRAME			m_pRootFrame			= nullptr;
	LPD3DXFRAME			m_pRootFrame2			= nullptr;
private:
	_bool				m_isConnect				= false;
private:
	_matrix*			m_ConnectFrameMatrix	= nullptr;
	LPD3DXFRAME			m_ConnectFrame			= nullptr;
private:
	_matrix				m_PivotMatrix;
	CHierarchyLoader*	m_pLoader			= nullptr;
	CAnimation*			m_pAnimation_Upper	= nullptr;
	CAnimation*			m_pAnimation_Lower	= nullptr;
private:
	vector<_matrix**>					m_vecCombinedTransformationMatrixPointers;
	vector<D3DXMESHCONTAINER_DERIVED*>	m_vecMeshContainers;
private:
	HRESULT	Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame);
	void	Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame, D3DXFRAME_DERIVED* pOut);
public:
	static  CDynamic_Mesh*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void* pArg)							override;
	virtual void			Free()										override;
};
END

