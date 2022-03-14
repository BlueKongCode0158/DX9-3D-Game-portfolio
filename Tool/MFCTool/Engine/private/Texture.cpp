#include "..\public\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CTexture::CTexture(const CTexture & rhs)
	:CComponent(rhs)
	,m_vTexture(rhs.m_vTexture)
{
	for (auto pTexture : m_vTexture)
	{
		Safe_AddRef(pTexture);
	}
}

HRESULT CTexture::NativeConstruct_Prototype(TEXTURETYPE eTextureType,const _tchar* pFilePath,_uint iTextureNum)
{
	m_vTexture.reserve(iTextureNum);

	_tchar szTexturePath[MAX_PATH] = TEXT("");

	if (nullptr == pFilePath)
	{
		MSGBOX("Failed to Creating Texture ProtoType");
		return E_FAIL;
	}

	for (_uint i = 0; i < iTextureNum; i++)
	{
		IDirect3DBaseTexture9*	pTexture = nullptr;

		//LPDIRECT3DCUBETEXTURE9
		wsprintf(szTexturePath, pFilePath, i);

		if (TEXTURE_GENERIC == eTextureType)
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szTexturePath, reinterpret_cast<LPDIRECT3DTEXTURE9*>(&pTexture))))
			{
				return E_FAIL;
			}
			//D3DXCreateTextureFromFileExW
		}
		else if (TEXTURE_CUBE == eTextureType)
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTexturePath, reinterpret_cast<LPDIRECT3DCUBETEXTURE9*>(&pTexture))))
			{
				return E_FAIL;
			}
		}
		m_vTexture.push_back(pTexture);
	}

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::SetUp_OnDevice_Texture(_uint iStage, _uint iTextureNum)
{
	if (nullptr == m_pGraphic_Device
		|| iTextureNum >= m_vTexture.size())
	{
		MSGBOX("Failed to Texture SetUp - Device");
		return E_FAIL;
	}
	return m_pGraphic_Device->SetTexture(iStage, m_vTexture[iTextureNum]);
}

CTexture * CTexture::Create(TEXTURETYPE eType, const _tchar* pFilePath, LPDIRECT3DDEVICE9 pGraphic_Device, _uint iTextureNum )
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pFilePath, iTextureNum)))
	{
		MSGBOX("Failed to Creating CTexture_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CComponent* pInstance = new CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTexture_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto pTexture : m_vTexture)
	{
		Safe_Release(pTexture);
	}
	m_vTexture.clear();
}
