#include "..\public\Canvas.h"
#include "GameInstacne.h"

CCanvas::CCanvas(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCanvas::CCanvas(const CCanvas & rhs)
	: CGameObject(rhs)
{
	for (int i = 0; i < CCanvas::TYPE::TYPE_END; i++)
	{
		m_vOriginal_Point[i] = rhs.m_vOriginal_Point[i];
	}
}

HRESULT CCanvas::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	m_vOriginal_Point[LEFT_TOP] = _float3(-1.f, 1.f, 0.5f);
	m_vOriginal_Point[RIGHT_TOP] = _float3(1.f, 1.f, 0.5f);
	m_vOriginal_Point[RIGHT_BOTTOM] = _float3(1.f, -1.f, 0.5f);
	m_vOriginal_Point[LEFT_BOTTOM] = _float3(-1.f, -1.f, 0.5f);
	m_vOriginal_Point[CENTER] = _float3(0.f, 0.f, 0.5f);
	return S_OK;
}

HRESULT CCanvas::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Component(*reinterpret_cast<int*>(pArg))))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int CCanvas::Tick(double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstacne* pInstance = GET_INSTANCE(CGameInstacne);
	_matrix ProjMatrix = pInstance->Get_Transform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION);
	_matrix ViewMatrix = pInstance->Get_Transform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW);
	_float3 ViewPosition = pInstance->Get_CamPosition();
	RELEASE_INSTANCE(CGameInstacne);

	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	for (int i = 0; i < TYPE_END; i++)
	{
		D3DXVec3TransformCoord(&m_vWorld_Point[i], &m_vOriginal_Point[i], &ProjMatrix);
		D3DXVec3TransformCoord(&m_vWorld_Point[i], &m_vOriginal_Point[i], &ViewMatrix);
	}
	D3DXPlaneFromPoints(&m_Plan, &m_vWorld_Point[0], &m_vWorld_Point[1], &m_vWorld_Point[2]);

	// 여기 카메라가 보는 방향에 따라 음수 / 양수 나올수도 있으니 +로 바꿔주는게 나을지도
	// 플랜의 정중앙 좌표 구하기.
	_float3 vLengthX = m_vWorld_Point[RIGHT_BOTTOM] - m_vWorld_Point[LEFT_BOTTOM];
	_float3 vLengthY = m_vWorld_Point[RIGHT_TOP] - m_vWorld_Point[RIGHT_BOTTOM];

	_float iLengthX = D3DXVec3Length(&vLengthX);
	_float iLengthY = D3DXVec3Length(&vLengthY);

	// normalize
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3Normalize(reinterpret_cast<_float3* >(&ViewMatrix.m[i][0]), reinterpret_cast<_float3*>(&ViewMatrix.m[i][0]));
	}

	if (m_isSetting == false)
	{
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, m_vWorld_Point[CENTER]);
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_RIGHT, *reinterpret_cast<_float3*>(&ViewMatrix.m[0][0]));
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_UP, *reinterpret_cast<_float3*>(&ViewMatrix.m[1][0]));
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_LOOK, *reinterpret_cast<_float3*>(&ViewMatrix.m[2][0]));
		m_pTransformCom->Set_MatrixScale(iLengthX, iLengthY, 1.f);
	}
	return _int();
}

_int CCanvas::Late_Tick(double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_ALPHA, this);

	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_W))
	{
		m_pTransformCom->Walk_Look(TimeDelta * 0.5f);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_S))
	{
		m_pTransformCom->Walk_Back(TimeDelta*0.5f);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_A))
	{
		m_pTransformCom->Rotation_Axis(_float3(0.f, -1.f, 0.f), TimeDelta * 0.2f);
	}
	if (pGameInstance->Input_KeyBoard_Pressing(DIK_D))
	{
		m_pTransformCom->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta * 0.2f);
	}
	if (pGameInstance->Input_KeyBoard_Down(DIK_Z))
	{
		if (m_isSetting)
		{
			m_isSetting = false;
		}
		else
		{
			m_isSetting = true;
		}
	}
	RELEASE_INSTANCE(CGameInstacne);
	return _int();
}

HRESULT CCanvas::Render()
{
	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CCanvas::Add_Component(_uint iLevel)
{
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Shader_UI_3D"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 30.f;
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CCanvas::SetUp_ConstantTable()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

CCanvas * CCanvas::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCanvas* pInstance = new CCanvas(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating Canvas_Prototype");
	}
	return pInstance;
}

CGameObject * CCanvas::Clone(void * pArg)
{
	CCanvas* pInstance = new CCanvas(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating Canvas_Clone");
	}
	return pInstance;
}

void CCanvas::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
