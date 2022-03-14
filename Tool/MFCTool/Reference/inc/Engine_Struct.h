#pragma once
namespace Engine
{
	typedef struct D3DXFrame_Derived : public D3DXFRAME
	{
		_bool			isCloned = false;
		D3DXMATRIX		CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct MeshMaterialTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_END };
		LPDIRECT3DTEXTURE9		pDiffuseMap = nullptr;
		LPDIRECT3DTEXTURE9		pNormalMap	= nullptr;
		LPDIRECT3DTEXTURE9		pSpecularMap= nullptr;
	}MESHMATERIALTEXTURE;

	typedef struct D3DXMeshContainerDerived : public D3DXMESHCONTAINER
	{
		MESHMATERIALTEXTURE**	ppMaterialTextures				 = nullptr;
		LPD3DXMESH				pOriginalMesh					 = nullptr;
		D3DXMATRIX*				pOffsetMatrices					 = nullptr;
		D3DXMATRIX*				pRenderingMatrices				 = nullptr;
		D3DXMATRIX**			ppCombinedTransformationMatrices = nullptr;
		unsigned int			iNumBones = 0;
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct tagVertexTexture_Viewport
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX_VP;

	typedef struct tagVertexNormal
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
	}VTXNORMAL;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;
	}VTXCUBE;

	typedef	struct tagVertexMatrix
	{
		D3DXVECTOR4		vRight;
		D3DXVECTOR4		vUp;
		D3DXVECTOR4		vLook;
		D3DXVECTOR4		vPosition;
	}VTXMATRIX;

	typedef	struct tagVertexPointTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
		float			fSize;
	}VTXPOINT;
}