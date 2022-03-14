#include "stdafx.h"
#include "..\public\SkyBox.h"
#include "GameInstacne.h"


CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	:CGameObject(rhs)
{
}

HRESULT CSkyBox::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CSkyBox::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (nullptr != pArg)
	{
		memcpy(&m_tSkyDesc, pArg, sizeof(SKYDESC));
	}

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CSkyBox::Tick(double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CSkyBox::Late_Tick(double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	_matrix viewMatrix, CamWorldMatrix;
	viewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&CamWorldMatrix, nullptr, &viewMatrix);

	m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE_POSITION, *reinterpret_cast<_float3*>(&CamWorldMatrix.m[3][0]));

	if (FAILED(m_pRendrerCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this)))
	{
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CSkyBox::Render()
{
	__super::Render();

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);


	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}
	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CSkyBox::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Trasform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendrerCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(m_tSkyDesc.iCurrentIndex, TEXT("Prototype_VIBuffer_Cube"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(m_tSkyDesc.iCurrentIndex, TEXT("Prototype_Texture_Cube"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(m_tSkyDesc.iCurrentIndex, TEXT("Prototype_Shader_SkyBox"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSkyBox::SetUp_ConstantTable()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(),sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom->Get_Texture(m_tSkyDesc.iSkyBoxNum));

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

CSkyBox * CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkyBox* pInstance = new CSkyBox(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To Creating CSkyBox_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkyBox::Clone(void * pArg)
{
	CGameObject* pInstance = new CSkyBox(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To Creating CGameObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pRendrerCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);

}
