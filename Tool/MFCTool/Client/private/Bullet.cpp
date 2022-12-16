#include "stdafx.h"
#include "..\public\Bullet.h"
#include "GameInstacne.h"
#include "Monster.h"
#include "ShotUI.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBullet::CBullet(const CBullet & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBullet::NativeConstruct(void * pArg)
{
	ZeroMemory(&m_tBulletInfo, sizeof(BULLETINFO));

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);

	if (nullptr != pArg)
	{
		memcpy(&m_tBulletInfo, pArg, sizeof(BULLETINFO));
		
		D3DXMatrixIdentity(&m_tBulletInfo.m_OriginMatrix);
		m_pTransformCom->Set_MatrixScale(2.5f, 2.5f, 10.f);

		CTransform* pTransform = dynamic_cast<CTransform*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));
		if (nullptr != pTransform)
		{
			m_tBulletInfo.m_pParentWorldMatrix = pTransform->Get_WorldMatrix();
		}
		if (GUN_BULLET::GUN_LEFT == m_tBulletInfo.eType)
		{
			CDynamic_Mesh* pMesh = dynamic_cast<CDynamic_Mesh*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Mesh"), 0));
			if (nullptr != pTransform)
			{
				m_tBulletInfo.m_pParentBoneMatrix = pMesh->Get_BoneMatrixPointer_Upper("gun_l");
			}
		}
		else if (GUN_BULLET::GUN_RIGHT == m_tBulletInfo.eType)
		{
			CDynamic_Mesh* pMesh = dynamic_cast<CDynamic_Mesh*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Mesh"), 0));
			if (nullptr != pTransform)
			{
				m_tBulletInfo.m_pParentBoneMatrix = pMesh->Get_BoneMatrixPointer_Upper("gun_r");
			}

		}

		m_tBulletInfo.m_OriginMatrix = *m_pTransformCom->Get_WorldMatrix();
		m_pTransformCom->Set_WorldMatrix(m_tBulletInfo.m_OriginMatrix * (*m_tBulletInfo.m_pParentBoneMatrix * *m_tBulletInfo.m_pParentWorldMatrix));

		m_pTransformCom->Remove_Rotation();

		_float3	vScale = m_pTransformCom->Get_Scale();
		_float3 vRight, vUp, vLook;


		CShotUI* pShotUI = dynamic_cast<CShotUI*>(pGameInstance->Find_GameObject(LEVEL_GAMEPLAY0/*이거 유지보수 할 때 변수로 바꿔줘야할 듯*/,
			TEXT("Layer_UI"), 0));

		if (nullptr != pShotUI)
		{
			if (nullptr != pShotUI->Get_TargetPosition())
			{
				
				vLook = *pShotUI->Get_TargetPosition() - m_pTransformCom->Get_MatrixRow(CTransform::STATE::STATE_POSITION);
			}
			else
			{
				vLook = m_tBulletInfo.m_vTargetPos;
			}
		}
		else
		{
			vLook = m_tBulletInfo.m_vTargetPos;
		}
		D3DXVec3Normalize(&vLook, &vLook);

		D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVec3Cross(&vUp, &vLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_RIGHT, vRight*vScale.x);
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_UP, vUp*vScale.y);
		m_pTransformCom->Set_WorldMatrixRow(CTransform::STATE::STATE_LOOK, vLook*vScale.z);

	}
	RELEASE_INSTANCE(CGameInstacne);

	return S_OK;
}

_int CBullet::Tick(double TimeDelta)
{
	if (true == m_isDead)
	{
		return CGameObject::STATE::OBJ_DEAD;
	}
	m_fDeadTime += TimeDelta;

	if (m_fDeadTime >= 5.f)
	{
		return CGameObject::STATE::OBJ_DEAD;
	}

	m_pTransformCom->Walk_Look(TimeDelta);
	m_pColliderCom->Update_Collider();
	CGameInstacne*  pGameInstance = GET_INSTANCE(CGameInstacne);
	_uint			iMosterListSize = pGameInstance->Get_GameObject_ListSize(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"));


	for (_uint i = 0; i < iMosterListSize; i++)
	{
		CCollider*		pColliderCom	= dynamic_cast<CCollider*>(pGameInstance->Find_Component(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), TEXT("Com_Collider"), i));
		CMonster*		pMonster		= dynamic_cast<CMonster*>(pGameInstance->Find_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Monster"), i));

		if (true == m_pColliderCom->Collision_AABB(pColliderCom))
		{
			if (false == g_isCheat)
			{
				pMonster->Set_isHurt(true);
				_uint iDamage = m_tBulletInfo.m_iAttackDamage;

				pMonster->Sup_Hp(iDamage);
				m_isDead = true;
			}
			else
			{
				pMonster->Set_isHurt(true);
				_uint iDamage = 100;

				pMonster->Sup_Hp(iDamage);
				m_isDead = true;
			}
		}
	}
	RELEASE_INSTANCE(CGameInstacne);

	return _int();
}

_int CBullet::Late_Tick(double TimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONALPHA, this);
	
	return _int();
}

HRESULT CBullet::Render()
{
	__super::Render();

	if (FAILED(SetUp_ConstantTable()))
	{
		return E_FAIL;
	}

	m_pShaderCom->Begin_Shader(0);
	m_pVIBufferCom->Render_VIBuffer();
	m_pShaderCom->End_Shader();

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
#endif // _DEBUG



	return S_OK;
}

HRESULT CBullet::Add_Component()
{
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationSpec = D3DXToRadian(90.f);
	TransformDesc.fTransformSpec = 50.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Cube"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	{
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Cube"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	{
		return E_FAIL;
	}
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(0.05f, 0.05f, 0.05f);
	ColliderDesc.vIniPos = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom),&ColliderDesc)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CBullet::SetUp_ConstantTable()
{
	CGameInstacne* pGameInstance = GET_INSTANCE(CGameInstacne);
	
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjectionMatrix", pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));


	RELEASE_INSTANCE(CGameInstacne);
	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBullet* pInstance = new CBullet(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CBullet_Prototype");
	}
	return pInstance;
}

CGameObject * CBullet::Clone(void * pArg)
{
	CBullet* pInstance = new CBullet(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("Failed to Creating CBullet_Clone");
	}
	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
