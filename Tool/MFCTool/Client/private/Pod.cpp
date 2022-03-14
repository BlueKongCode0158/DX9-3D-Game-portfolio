#include "stdafx.h"
#include "..\public\Pod.h"
#include "GameInstacne.h"

CPod::CPod(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPod::CPod(const CPod & rhs)
	:CGameObject(rhs)
{
}

HRESULT CPod::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPod::NativeConstruct(void * pArg)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	m_pTransformCom->Set_MatrixScale(0.04f, 0.04f, 0.04f);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, _float3(30.f, 20.f, 30.f));

	m_pMeshCom->Set_AnimationIndex(1,1);
	return S_OK;
}

_int CPod::Tick(_double TimeDelta)
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	CComponent*	pTerrainBuffer = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer_Terrain"), 0);
	CComponent* pTerrainTrasform = pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);

	m_pTransformCom->Stand_On_Plane(dynamic_cast<CTransform*>(pTerrainTrasform), dynamic_cast<CVIBuffer_Terrain*>(pTerrainBuffer));

	_float3	vPosition = m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, vPosition + _float3(0.f, 2.f, 0.f));

	if (m_pMeshCom->Play_Animation_Per() > 0.90f)
	{
		m_pMeshCom->Set_AnimationStop(true);

		if (m_pMeshCom->Get_CurrentAnimationIndex() != 0)
		{
			if (pGameInstance->Input_KeyBoard_Down(DIK_E))
			{
				m_isOpen = true;
				m_pMeshCom->Set_AnimationStop(false);
			}
		}
		if (m_pMeshCom->Get_CurrentAnimationIndex() == 0)
		{
			m_isFinish = true;
		}
	}

	if (true == m_isOpen)
	{
		m_pMeshCom->Set_AnimationIndex(0);
	}
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

_int CPod::Late_Tick(_double TimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
	{
		return _int();
	}
	m_pMeshCom->Play_Animation(TimeDelta * 2);

	return _int();
}

HRESULT CPod::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	_uint iShaderIndex = 3;


	m_pShaderCom->Begin_Shader(2);
	_uint iNumMeshContainers = m_pMeshCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; i++)
	{
		_uint iNumMaterial = m_pMeshCom->Get_NumMaterials(i);
		m_pMeshCom->Update_SkinnedMesh(i);

		for (_uint j = 0; j < iNumMaterial; j++)
		{
			/* Diffuse Map Setting */
			if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i, j)))
			{
				return E_FAIL;
			}
			/* Normal Map Setting */
			if (SUCCEEDED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_NormalTexture", MESHMATERIALTEXTURE::TYPE_NORMAL, i, j)))
			{
				m_pShaderCom->End_Shader();
				m_pShaderCom->Begin_Shader(iShaderIndex);
			}
			m_pShaderCom->Commit();
			m_pMeshCom->Render(i, j);
		}
	}
	m_pShaderCom->End_Shader();

	return S_OK;
}

_bool CPod::isOpen()
{
	return m_isOpen;
}

_bool CPod::isFinish()
{
	return m_isFinish;
}

HRESULT CPod::Add_Component()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fTransformSpec = 5.f;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Pod_Mesh"), TEXT("Com_Mesh"), reinterpret_cast<CComponent**>(&m_pMeshCom))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPod::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
	{
		return E_FAIL;
	}
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CPod * CPod::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPod* pInstance = new CPod(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPod_Prototype");
	}
	return pInstance;
}

CGameObject * CPod::Clone(void * pArg)
{
	CPod* pInstance = new CPod(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CPod_Clone");
	}
	return pInstance;
}

void CPod::Free()
{
	__super::Free();

	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
