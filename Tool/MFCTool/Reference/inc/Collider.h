#pragma once
#include "Component.h"

BEGIN(Engine)
class CTransform;
class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLIDER{ TYPE_AABB , TYPE_OBB, TYPE_SPHERE, TYPE_END};
public:
	typedef struct tagColliderDesc
	{
		const _matrix*		pParentMatrix	= nullptr;
		const _matrix*		pBoneMatrix		= nullptr;
		_float3				vScale		= _float3(1.f, 1.f, 1.f);
		_float3				vRadians	= _float3(0.f, 0.f, 0.f);
		_float3				vIniPos		= _float3(0.f, 0.f, 0.f);
	}COLLIDERDESC;
	
	typedef struct tagOBBDesc
	{
		_float3				vCenter;
		_float3				vAlignAxis[3];
		_float3				vCenterAxis[3];
	}OBBDESC;
public:
	explicit	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit	CCollider(const CCollider& rhs);
	virtual		~CCollider() = default;
public:
	void		Get_ColliderInfo(CTransform** ppTransform, LPD3DXMESH* ppMesh);
	_matrix		Get_WorldMatrix() const;
public:
	HRESULT		NativeConstruct_Prototype(COLLIDER	eType);
	HRESULT		NativeConstruct(void* pArg)	override;
	void		Update_Collider();
public:
	void	Get_PointPolygonIndex(_uint uiPolygonIndex, _float3* pPoints) const;
public:
	const COLLIDERDESC	Get_ColliderDesc() const;
	HRESULT				Reset_ColliderScalse();
	HRESULT				Update_ColliderScale(_float3 vScale);
private:
	HRESULT				SetUp_ColliderScale();
public:
	_bool		Collision_AABB(CCollider*	pTargetCollider);
	_bool		Collision_OBB(CCollider*	pTargetCollider);
	_bool		Collision_SPHERE(CCollider* pTargetCollider);
#ifdef _DEBUG
public:
	virtual HRESULT		Render() override;
	HRESULT		Compile_ShaderDebug();
private:
	class CShader*	m_pShader = nullptr;
#endif // _DEBUG
private:
	OBBDESC		Compute_OBBDesc(_float3* pPoints);
private:
	_float3			m_vOriginScale;
private:
	COLLIDER		m_eType			= TYPE_END;
	COLLIDERDESC	m_ColliderDesc;
	LPD3DXMESH		m_pMesh			= nullptr;
	_bool			m_isCollision	= false;
	_matrix			m_OriginalMatrix;
	_matrix			m_ParentMatrix;
	_matrix			m_BoneMatrix;
	CTransform*		m_pTransform = nullptr;
private:
	_float3			m_vMin;
	_float3			m_vMax;
	_float3			m_vPoints[8];
	_uint			m_iIndeces[36];
public:
	static  CCollider*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER eType);
	virtual CComponent*	Clone(void* pArg);
	virtual	void		Free() override;
};
END
