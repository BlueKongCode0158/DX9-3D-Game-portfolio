#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

class CBullet final : public CGameObject
{
public:
	enum GUN_BULLET { GUN_LEFT, GUN_RIGHT, GUN_END };
public:
	typedef struct tagBulletInfo
	{
		_matrix			m_OriginMatrix;
		_float3			m_vTargetPos;
		const _matrix*	m_pParentWorldMatrix	= nullptr;
		const _matrix*	m_pParentBoneMatrix		= nullptr;
		_int			m_iAttackDamage			= 0;
		GUN_BULLET		eType = GUN_END;
	}BULLETINFO;
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBullet(const CBullet& rhs);
	virtual  ~CBullet() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	BULLETINFO	m_tBulletInfo;
	_float		m_fDeadTime			=0.f;
private:
	CVIBuffer_Cube* m_pVIBufferCom	= nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CCollider*		m_pColliderCom	= nullptr;
	CShader*		m_pShaderCom	= nullptr;
private:
	_bool			m_isDead		= false;
public:
	static CBullet*		 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void		 Free() override;
};

