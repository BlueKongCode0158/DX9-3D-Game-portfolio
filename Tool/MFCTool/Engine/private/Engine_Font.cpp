#include "Graphic_Device.h"
#include "Engine_Font.h"
#include "Transform.h"
#include "PipeLine.h"
#include "Shader.h"

CEngine_Font::CEngine_Font(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{

}

CEngine_Font::CEngine_Font(const CEngine_Font & rhs)
	:CComponent(rhs)
	,m_pShader(reinterpret_cast<CShader*>(rhs.m_pShader->Clone()))
	,m_pTransform(reinterpret_cast<CTransform*>(rhs.m_pTransform->Clone()))
	,m_pFont(rhs.m_pFont)
{
	Safe_AddRef(m_pFont);
}

HRESULT CEngine_Font::NativeConstruct_Prototype()
{
	CGraphic_Device* pGraphic_Device = GET_INSTANCE(CGraphic_Device);

	m_pFont = pGraphic_Device->Get_Font();
	Safe_AddRef(m_pFont);

	RELEASE_INSTANCE(CGraphic_Device);


	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
	{
		return E_FAIL;
	}

	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Font.hlsl"));
	if (nullptr == m_pShader)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEngine_Font::NativeConstruct(void * pArg)
{
	if (nullptr == pArg)
	{
		return E_FAIL;
	}
	memcpy(&m_tFontDesc, pArg, sizeof(FONTDESC));
	_float3		vPosition = m_pTransform->Get_MatrixRow(CTransform::STATE::STATE_POSITION);

	m_pTransform->Set_WorldMatrixRow(CTransform::STATE::STATE_POSITION, m_tFontDesc.vInitPos);
	m_pTransform->Set_MatrixScale(m_tFontDesc.vScale.x, m_tFontDesc.vScale.y, m_tFontDesc.vScale.z);
	m_pTransform->Rotation_Aixs_Accumulate(_float3(1.f, 0.f, 0.f), m_tFontDesc.vRotation.x);
	m_pTransform->Rotation_Aixs_Accumulate(_float3(0.f, 1.f, 0.f), m_tFontDesc.vRotation.y);
	m_pTransform->Rotation_Aixs_Accumulate(_float3(0.f, 0.f, 1.f), m_tFontDesc.vRotation.z);

	m_OriginMatrix = *m_pTransform->Get_WorldMatrix();
	D3DXMatrixIdentity(&m_ParentMatrix);
	return S_OK;
}

void CEngine_Font::Update_Font()
{
	if (nullptr != m_tFontDesc.pParentMatrix)
	{
		m_ParentMatrix = *m_tFontDesc.pParentMatrix;
	}

	D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_ParentMatrix.m[0][0]), reinterpret_cast<_float3*>(&m_ParentMatrix.m[0][0]));
	D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_ParentMatrix.m[1][0]), reinterpret_cast<_float3*>(&m_ParentMatrix.m[1][0]));
	D3DXVec3Normalize(reinterpret_cast<_float3*>(&m_ParentMatrix.m[2][0]), reinterpret_cast<_float3*>(&m_ParentMatrix.m[2][0]));

	_matrix WorldMatrix;

	WorldMatrix = m_OriginMatrix * m_ParentMatrix;
	m_pTransform->Set_WorldMatrix(WorldMatrix);

	return;
}

HRESULT CEngine_Font::Render_Font(const _tchar * pText, RECT rect)
{
	if (nullptr == m_pFont)
	{
		return E_FAIL;
	}

	//D3DXMATRIX world;
	////75.f, 0.f, 62.f
	//D3DXMatrixTranslation(&world, 75, 12, 62);
	//m_pSprite->SetTransform(&world);
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	//m_pShader->SetUp_ConstantTable("g_WorldMatrix", m_pTransform->Get_WorldMatrix(), sizeof(_matrix));
	//m_pShader->SetUp_ConstantTable("g_ViewMatrix", &pPipeLine->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	//m_pShader->SetUp_ConstantTable("g_ProjectionMatrix", &pPipeLine->Get_Transform(D3DTS_VIEW), sizeof(_matrix));

	//m_pShader->Begin_Shader(0);
	//m_pShader->End_Shader();

	RELEASE_INSTANCE(CPipeLine);
	return S_OK;
}

CEngine_Font * CEngine_Font::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEngine_Font*	pInstance = new CEngine_Font(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CEngine_Font_Prototype");
	}
	return pInstance;
}

CComponent * CEngine_Font::Clone(void * pArg)
{
	CEngine_Font*	pInstance = new CEngine_Font(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CEngine_Font_Clone");
	}
	return pInstance;
}

void CEngine_Font::Free()
{
	__super::Free();

	Safe_Release(m_pFont);
	Safe_Release(m_pShader);
	Safe_Release(m_pTransform);
}
