#include "..\public\Target_Manager.h"
#include "Target.h"


IMPLEMENT_SINGLETON(CTarget_Manager)
CTarget_Manager::CTarget_Manager()
{
}
LPDIRECT3DTEXTURE9 CTarget_Manager::Get_TargetTexture(const _tchar * pTargetTag)
{
	CTarget*	pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
	{
		return nullptr;
	}
	return pRenderTarget->Get_TargetTexture();
}
HRESULT CTarget_Manager::Add_RenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
	{
		return E_FAIL;
	}


	CTarget*	pRenderTarget = CTarget::Create(pGraphic_Device, iWidth, iHeight, eFormat, ClearColor);
	if (nullptr == pRenderTarget)
	{
		return E_FAIL;
	}

	m_mapRenderTargets.emplace(pTargetTag, pRenderTarget);
	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget*	pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
	{
		return E_FAIL;
	}

	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		list<CTarget*> MRTList;
		MRTList.push_back(pRenderTarget);
		m_mapMRTs.emplace(pMRTTag, MRTList);
	}
	else
	{
		pMRTList->push_back(pRenderTarget);
	}
	Safe_AddRef(pRenderTarget);
	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		return E_FAIL;
	}

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Clear();
	}

	_uint		iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Bind_RenderTarget(iIndex++);
	}
	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		return E_FAIL;
	}

	_uint		iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Relese_RenderTarget(iIndex++);
	}
	return S_OK;
}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug_Buffer(const _tchar * pTargetTag, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	CTarget*		pTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pTarget)
	{
		return E_FAIL;
	}

	return pTarget->Ready_Debug_Buffet(fLeftX, fTopY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug_Buffer(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		return E_FAIL;
	}

	for (auto& pTarget : *pMRTList)
	{
		pTarget->Render_Debug_Buffer();
	}
	return S_OK;
}
#endif // _DEBUG
CTarget * CTarget_Manager::Find_RenderTarget(const _tchar * pTargetTag)
{
	auto	iter = find_if(m_mapRenderTargets.begin(), m_mapRenderTargets.end(), CTagFinder(pTargetTag));
	if (iter == m_mapRenderTargets.end())
	{
		return nullptr;
	}
	return iter->second;
}

list<class CTarget*>* CTarget_Manager::Find_MRT(const _tchar * pTargetTag)
{
	auto iter = find_if(m_mapMRTs.begin(), m_mapMRTs.end(), CTagFinder(pTargetTag));
	if (iter == m_mapMRTs.end())
	{
		return nullptr;
	}
	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_mapMRTs)
	{
		for (auto& pTarget : Pair.second)
		{
			Safe_Release(pTarget);
		}
		Pair.second.clear();
	}
	m_mapMRTs.clear();

	for (auto& Pair : m_mapRenderTargets)
	{
		Safe_Release(Pair.second);
	}
	m_mapRenderTargets.clear();
}
