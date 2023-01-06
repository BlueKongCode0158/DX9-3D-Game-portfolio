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

	typedef struct tagSingleItem
	{
		unsigned int		iAddHp;
		unsigned int		iAddAtt;
		unsigned int		iAdd;
		unsigned int		iAddJump;
		float				iAddSpeed;
	}SINGLEITEM;

	typedef struct PSystemDesc
	{
		_int	m_iMaxParticle = 0;
		_int	m_iMinParticle = 0;
		_bool	m_isEmission = true;
		_float	m_fEmissionTime = 0.f;
		_float3	m_vSystemPosition;
	}PSDESC;

	typedef struct ParticleDesc
	{
		_float3 m_vDir = _float3(0.f, 0.f, 0.f);
		_float3 m_vDirStart = _float3(-1.f, -1.f, -1.f);
		_float3 m_vDirEnd = _float3(1.f, 1.f, 1.f);
		_float3	m_vPosition = _float3(0.f, 0.f, 0.f);
		_float3	m_vColor = _float3(0.f, 0.f, 0.f);

		_float	m_fAge = 0.f;
		_float	m_fLifeTime = 0.f;
		_float	m_fColorFade = 0.f;
		_float	m_fVelocity = 0.f;
		_float	m_fAcceleration = 0.f;
		_bool	m_isAlive = true;

		_bool	operator == (const ParticleDesc& rDesc) const
		{
			if (m_vDir != rDesc.m_vDir)
			{
				return false;
			}
			if (m_vDirStart != rDesc.m_vDirStart)
			{
				return false;
			}
			if (m_vDirEnd != rDesc.m_vDirEnd)
			{
				return false;
			}
			if (m_vPosition != rDesc.m_vPosition)
			{
				return false;
			}
			if (m_vColor != rDesc.m_vColor)
			{
				return false;
			}
			if (m_fColorFade != rDesc.m_fColorFade)
			{
				return false;
			}
			if (m_fVelocity != rDesc.m_fVelocity)
			{
				return false;
			}
			if (m_fAcceleration != rDesc.m_fAcceleration)
			{
				return false;
			}
			if (m_fLifeTime != rDesc.m_fLifeTime)
			{
				return false;
			}
			if (m_fAge != rDesc.m_fAge)
			{
				return false;
			}
			if (m_isAlive != rDesc.m_isAlive)
			{
				return false;
			}
			return true;
		}
		_bool	operator != (const ParticleDesc& rDesc) const
		{
			return !(*this == rDesc);
		}
	}PDESC;

	typedef struct ParticleInfo
	{
		PDESC			m_ParticleDesc;
		PSDESC			m_SystemDesc;
	}PINFO;
}
