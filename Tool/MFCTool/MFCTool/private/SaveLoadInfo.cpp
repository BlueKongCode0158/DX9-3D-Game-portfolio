#include "stdafx.h"
#include "..\public\SaveLoadInfo.h"


CSaveLoadInfo::CSaveLoadInfo()
{

}

HRESULT CSaveLoadInfo::NativeConstruct(HWND hWnd)
{
	m_hWnd = hWnd;
	return S_OK;
}

HRESULT CSaveLoadInfo::Texture_SaveInfo(LPDIRECT3DTEXTURE9 pTexture, const _tchar * pFileName)
{
	D3DXSaveTextureToFile(pFileName, D3DXIFF_BMP, pTexture, nullptr);
	return S_OK;
}

CSaveLoadInfo * CSaveLoadInfo::Create(HWND hWnd)
{
	CSaveLoadInfo*	pInstance = new CSaveLoadInfo();
	if (FAILED(pInstance->NativeConstruct(hWnd)))
	{
		MessageBoxA(pInstance->m_hWnd, " Failed to Creating CSaveLoadInfo_Class", "OK", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSaveLoadInfo::Free()
{

}
