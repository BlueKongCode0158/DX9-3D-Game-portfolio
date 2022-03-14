#include "..\public\Shader.h"
#include "Texture.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CShader::CShader(const CShader & rhs)
	:CComponent(rhs)
	,m_pEffect(rhs.m_pEffect)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::NativeConstruct_Prototype(const _tchar* pFilePath)
{
	LPD3DXBUFFER		pErrorBuffer = nullptr;

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphic_Device, pFilePath, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErrorBuffer)))
	{
		return E_FAIL;
	}

	Safe_Release(pErrorBuffer);
	
	return S_OK;
}

HRESULT CShader::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CShader::SetUp_ConstantTable(D3DXHANDLE hHandle, const void* pData, _uint iLengthByte)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}
	if (FAILED(m_pEffect->SetValue(hHandle, pData, iLengthByte)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CShader::SetUp_TextureConstantTable(D3DXHANDLE hHandle, CTexture* pTexture, _uint uiTextureIndex)
{
	if (pTexture->Get_vecTexture_Size() <= uiTextureIndex ||
		nullptr == m_pEffect)
	{
		return E_FAIL;
	}
	if (FAILED(m_pEffect->SetTexture(hHandle, pTexture->Get_Texture(uiTextureIndex))))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CShader::SetUp_TextureConstantTable(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}
	if (FAILED(m_pEffect->SetTexture(hHandle, pTexture)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CShader::SetUp_Bool(D3DXHANDLE hHandle, BOOL isBool)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	m_pEffect->SetBool(hHandle, isBool);
	return S_OK;
}

HRESULT CShader::Begin_Shader(_uint iIndexNum)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(iIndexNum);

	return S_OK;
}

void CShader::Commit()
{
	if (nullptr == m_pEffect)
	{
		return;
	}
	m_pEffect->CommitChanges();

}

HRESULT CShader::End_Shader()
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}
	m_pEffect->EndPass();
	m_pEffect->End();

	return S_OK;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath)
{
	CShader* pInstance = new CShader(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(pFilePath)))
	{
		MSGBOX("Failed_to_Creating_CShader_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CComponent*	pInstance = new CShader(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed_to_Creating_CShader_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
}
