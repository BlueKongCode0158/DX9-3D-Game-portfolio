#pragma once
namespace Engine
{
	typedef struct D3DXFrame_Derived : public D3DXFRAME
	{
		bool			isCloned = false;
		D3DXMATRIX		CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct MeshMaterialTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_EMISSION, TYPE_END };
		LPDIRECT3DTEXTURE9		pDiffuseMap		= nullptr;
		LPDIRECT3DTEXTURE9		pNormalMap		= nullptr;
		LPDIRECT3DTEXTURE9		pSpecularMap	= nullptr;
		LPDIRECT3DTEXTURE9		pEmissionMap	= nullptr;
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
		D3DXVECTOR4		vColor;
		float			fSize;
	}VTXPOINT;

	typedef struct tagSingleItem
	{
		unsigned int		iAddHp;
		unsigned int		iAddAtt;
		unsigned int		iAdd;
		unsigned int		iAddJump;
		float				iAddSpeed;
	}SINGLEITEM;

	typedef struct tagMeshParticle
	{
		_float	m_fMinSize = 0.f;
		_float	m_fMaxSize = 0.f;

		_float	m_fAge		= 0.f;
		_float	m_fDuration = 0.f;

		_float	m_fTextureStart = 0.f;
		_float	m_fTextureEnd	= 0.f;
	}PDESC_MESH;

	// 파티클 시스템이 가지고 있는 Particle 클래스의 데이터 객체.
	typedef struct PSystemDesc
	{
	public:
		_int	m_iMaxParticle	= 0;
		_bool	m_isAlive		= true;
		_bool	m_isEmission	= true;		// 파티클이 방출될 수 있나?
		_bool	m_isLoop		= false;	// 파티클 반복 유무.
	public:
		_float	m_fEmissionTime	= 0.f;
		_int	m_iInitCount	= 0;
	}PSDESC;

	// 파티클의 Attribute 데이터 객체
	typedef struct PointDesc
	{
	public:
		_float3 m_vMinPos		= _float3(0.f, 0.f, 0.f);	// MIN POS
		_float3 m_vMaxPos		= _float3(0.f, 0.f, 0.f);	// MAX POS
		_float3	m_vPosition		= _float3(0.f, 0.f, 0.f);	// CUR POS
		_float3 m_vVelocity		= _float3(0.f, 0.f, 0.f);	// 속도
		_float3	m_vDir			= _float3(0.f, 0.f, 0.f);	// 방향벡터.
	public:
		_float3 m_vColor		= _float3(0.f, 0.f, 0.f);
		_float3 m_vStartColor	= _float3(0.f, 0.f, 0.f);	// 시작 컬러
		_float3	m_vEndColor		= _float3(0.f, 0.f, 0.f);	// 마지막 컬러.
		_float	m_fColorFade	= 0.f;	// 색이 변하는 정도.
	public:
		_float	m_fDuration		= 0.f;	// 파티클의 수명 
		_float	m_fAge			= 0.f;	// 현재 나이.
		_float	m_fVelocity		= 0.f;	// 파티클 속도
	}PDESC;
}
