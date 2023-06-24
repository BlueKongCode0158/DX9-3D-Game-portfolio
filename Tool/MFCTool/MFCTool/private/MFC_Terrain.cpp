#include "stdafx.h"
#include "..\public\MFC_Terrain.h"
#include "GameInstacne.h"
#include "Brush_Manager.h"
#include "MainFrm.h"
#include "ControlFormView.h"
#include "CellPoint_Manager.h"

CMFC_Terrain::CMFC_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CMFC_Terrain::CMFC_Terrain(const CMFC_Terrain & rhs)
	:CGameObject(rhs)
	,m_iIndexX(rhs.m_iIndexX)
	,m_iIndexZ(rhs.m_iIndexZ)
	,m_pTerrainPrototypeName(rhs.m_pTerrainPrototypeName)
{

}

HRESULT CMFC_Terrain::NativeConstruct_Prototype(const _uint iIndexX, const _uint iIndexZ, const _tchar* TerrainComponent_Name)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	m_iIndexX = iIndexX;
	m_iIndexZ = iIndexZ;
	m_pTerrainPrototypeName = TerrainComponent_Name;
	return S_OK;
}

HRESULT CMFC_Terrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component(m_pTerrainPrototypeName)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTexture(m_pGraphicDevice, m_iIndexX, m_iIndexZ, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pFilterTexture)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTexture(m_pGraphicDevice, m_iIndexX, m_iIndexZ, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pHightMapTexture)))
	{
		return E_FAIL;
	}

	D3DLOCKED_RECT		LockRect;
	m_pFilterTexture->LockRect(0, &LockRect, nullptr, 0);

	for (size_t i = 0; i < m_iIndexZ; ++i)
	{
		for (size_t j = 0; j < m_iIndexX; ++j)
		{
			_uint	iIndex = i * m_iIndexX + j;

			((_ulong*)LockRect.pBits)[iIndex] = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		}
	}
	m_pFilterTexture->UnlockRect(0);

	D3DXSaveTextureToFile(TEXT("../bin/Filter.bmp"), D3DXIFF_BMP, m_pFilterTexture, nullptr);

	return S_OK;
}

_int CMFC_Terrain::Tick(double TimeDelta)
{
	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));

	if (0 != pView->m_MapTab.GetCurSel())
	{
		return 0;
	}

	__super::Tick(TimeDelta);

	CGameInstacne* pGameInstacne = GET_INSTANCE(CGameInstacne);

	m_vBrushPoint = pGameInstacne->Compute_PickingPoint(m_pVIBufferCom, *m_pTransformCom->Get_WorldMatrix());

	if (pGameInstacne->Input_KeyMouse_Pressing(CInput_Device::DIM_LBUTTON))
	{
		if (nullptr == m_vBrushPoint)
		{
			RELEASE_INSTANCE(CGameInstacne);
			return CGameObject::OBJ_IDLE;
		}

		_float fSize = CBrush_Manager::Get_Instance()->Get_BrushSize();
		_float fTimeDelta = 0.f;
		_float fBrushPower = CBrush_Manager::Get_Instance()->Get_BrushPower();
		

		if (CBrush_Manager::Get_Instance()->is_BrushFunction(CBrush_Manager::BRUSHFUNCTION::TERRAIN_UP))
		{
			fTimeDelta = TimeDelta;
			UpAndDown_Brush(fTimeDelta * fBrushPower, fSize, *m_vBrushPoint);
		}
		else if (CBrush_Manager::Get_Instance()->is_BrushFunction(CBrush_Manager::BRUSHFUNCTION::TERRAIN_DOWN))
		{
			fTimeDelta = -TimeDelta;
			UpAndDown_Brush(fTimeDelta * fBrushPower, fSize, *m_vBrushPoint);
		}
		else if (CBrush_Manager::Get_Instance()->is_BrushFunction(CBrush_Manager::BRUSHFUNCTION::TERRAIN_SPLATTING))
		{
			Splatting_BrushType(TimeDelta, fSize, *m_vBrushPoint);
		}
		else if (CBrush_Manager::Get_Instance()->is_BrushFunction(CBrush_Manager::BRUSHFUNCTION::TERRAIN_SMOOTH))
		{

		}
		else if(CBrush_Manager::Get_Instance()->is_BrushFunction(CBrush_Manager::BRUSHFUNCTION::TERRAIN_FLAT))
		{
			Flat_Terrain(TimeDelta, fSize, *m_vBrushPoint);
		}

	}

	RELEASE_INSTANCE(CGameInstacne);

	return CGameObject::OBJ_IDLE;
}

_int CMFC_Terrain::Late_Tick(double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this)))
	{
		return -1;
	}
	return _int();
}

HRESULT CMFC_Terrain::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	if (nullptr != m_pFilterTexture)
	{
		m_pShaderCom->SetUp_TextureConstantTable("g_FilterTexture", m_pFilterTexture);
	}
	if (nullptr != m_pTextureCom)
	{
		m_pShaderCom->SetUp_TextureConstantTable("g_RedDiffuseTexture"	,	 m_pTextureCom, 0);
		m_pShaderCom->SetUp_TextureConstantTable("g_GreenDiffuseTexture",	 m_pTextureCom, 1);
		m_pShaderCom->SetUp_TextureConstantTable("g_BlueDiffuseTexture" ,	 m_pTextureCom, 2);
		m_pShaderCom->SetUp_TextureConstantTable("g_AlphaDiffuseTexture",	 m_pTextureCom, 3);
	}

	if (nullptr != m_vBrushPoint)
	{
		_float fSize = CBrush_Manager::Get_Instance()->Get_BrushSize();
		m_pShaderCom->SetUp_ConstantTable("g_vBrushPos", m_vBrushPoint, sizeof(_float4));
		m_pShaderCom->SetUp_ConstantTable("g_fRange", &fSize, sizeof(_float));
	}
	m_pShaderCom->SetUp_ConstantTable("g_vCamPosition", pGameInstance->Get_CamPosition(), sizeof(_float4));

	_uint RenderState = CBrush_Manager::Get_Instance()->Get_Terrain_RenderState();

	m_pShaderCom->Begin_Shader(RenderState);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();


	CControlFormView *pView = (CControlFormView*)(((CMainFrame*)AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));


	if (1 == pView->m_MapTab.GetCurSel())
	{
#ifdef _DEBUG
		if (pGameInstance->Input_KeyBoard_Pressing(DIK_Q))
		{
			CCellPoint_Manager::Get_Instance()->Render(m_pTransformCom->Get_WorldMatrix());
		}
#endif // _DEBUG
	}

	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

HRESULT CMFC_Terrain::Filter_Texture_Load(const _tchar* pFile)
{
	if (nullptr == m_pFilterTexture)
	{
		return E_FAIL;
	}

	D3DXCreateTextureFromFileEx(m_pGraphicDevice, pFile,m_iIndexX - 1,m_iIndexZ - 1,0,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_DEFAULT, D3DX_DEFAULT,0,NULL,NULL,&m_pFilterTexture);

	return S_OK;
}

HRESULT CMFC_Terrain::Add_Component(const _tchar* pPrototypeName)
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 5.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}
	if(FAILED(__super::Add_Component(LEVEL_STATIC,TEXT("Prototype_Renderer"),TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pPrototypeName, TEXT("Com_Terrain"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Terrain_Shader"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Terrain_Texture"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CMFC_Terrain::UpAndDown_Brush(_float TimeDelta, _float fBrushSize, _float3 vPosition)
{
	if (CBrush_Manager::Get_Instance()->is_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE))
	{
		m_pVIBufferCom->Update_Terrain_Height(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE, m_pHightMapTexture, TimeDelta, fBrushSize, vPosition, TEXT("../bin/Test.bmp"));
	}
	else if (CBrush_Manager::Get_Instance()->is_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_RECT))
	{
		m_pVIBufferCom->Update_Terrain_Height(CBrush_Manager::BRUSHTYPE::BRUSH_RECT, m_pHightMapTexture, TimeDelta, fBrushSize, vPosition, TEXT("../bin/Test.bmp"));
	}
}

void CMFC_Terrain::Flat_Terrain(_float TimeDelta, _float fBrushSize, _float3 vPosition)
{
	if (CBrush_Manager::Get_Instance()->is_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE))
	{
		m_pVIBufferCom->Update_Terrain_Aver(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE, m_pHightMapTexture, TimeDelta, fBrushSize, vPosition, TEXT("../bin/Test.bmp"));
	}
	else if (CBrush_Manager::Get_Instance()->is_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_RECT))
	{
		m_pVIBufferCom->Update_Terrain_Aver(CBrush_Manager::BRUSHTYPE::BRUSH_RECT, m_pHightMapTexture, TimeDelta, fBrushSize, vPosition, TEXT("../bin/Test.bmp"));
	}

}

void CMFC_Terrain::Splatting_BrushType(_float TimeDelta, _float fBrushSize, _float3 vPosition)
{
	D3DLOCKED_RECT lockedRect;
	m_pFilterTexture->LockRect(0, &lockedRect, 0, 0);

	RECT tRect{};

	tRect.left = vPosition.x - fBrushSize;
	tRect.right = vPosition.x + fBrushSize;
	tRect.bottom = vPosition.z - fBrushSize;
	tRect.top = vPosition.z + fBrushSize;

	if (tRect.left <= 0)
	{
		tRect.left = 0;
	}
	else if(tRect.left >= m_iIndexX)
	{
		tRect.left = m_iIndexX;
	}

	if (tRect.right >= m_iIndexX)
	{
		tRect.right = m_iIndexX;
	}
	else if (tRect.right <= 0)
	{
		tRect.right = 0;
	}

	if (tRect.bottom <= 0)
	{
		tRect.bottom = 0;
	}
	else if (tRect.bottom >= m_iIndexZ)
	{
		tRect.bottom = m_iIndexZ;
	}

	if (tRect.top >= m_iIndexZ)
	{
		tRect.top = m_iIndexZ;
	}
	else if (tRect.top <= 0)
	{
		tRect.top = 0;
	}

	D3DXCOLOR XColor = CBrush_Manager::Get_Instance()->Get_BrushColor();

	if (CBrush_Manager::Get_Instance()->is_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_CIRCLE))
	{
		for (_uint i = tRect.left; i < tRect.right; i++/*_uint j = tRect.bottom; j < tRect.top; j++*/)
		{
			for (_uint j = tRect.bottom; j < tRect.top; j++)
			{
				_uint iIndex = i*m_iIndexX + j;

				_float fDistanceX = i - vPosition.x;
				_float fDistanceZ = j - vPosition.z;

				_float fDistance = sqrt(fDistanceX*fDistanceX + fDistanceZ*fDistanceZ);

				if (fabs(fDistance) > fBrushSize)
				{
					continue;
				}
				((_ulong*)lockedRect.pBits)[iIndex] = XColor;
			}
		}
	}
	else if (CBrush_Manager::Get_Instance()->is_BrushType(CBrush_Manager::BRUSHTYPE::BRUSH_RECT))
	{
		for (_uint i = tRect.left; i < tRect.right; i++/*_uint j = tRect.bottom; j < tRect.top; j++*/)
		{
			for (_uint j = tRect.bottom; j < tRect.top; j++)
			{
				_uint iIndex = i*m_iIndexX + j;

				((_ulong*)lockedRect.pBits)[iIndex] = XColor;
			}
		}
	}
	m_pFilterTexture->UnlockRect(0);
	D3DXSaveTextureToFile(TEXT("../bin/Filter.bmp"), D3DXIFF_BMP, m_pFilterTexture, nullptr);
}

CMFC_Terrain * CMFC_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint iIndexX, const _uint iIndexZ, const _tchar* TerrainPrototype_ComponentName)
{
	CMFC_Terrain*	pInstance = new CMFC_Terrain(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype(iIndexX,iIndexZ,TerrainPrototype_ComponentName)))
	{
		MSGBOX("Failed to Creating CMFC_Terrain_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMFC_Terrain::Clone(void * pArg)
{
	CGameObject*	pInstance = new CMFC_Terrain(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMFC_Terrain_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMFC_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pHightMapTexture);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
