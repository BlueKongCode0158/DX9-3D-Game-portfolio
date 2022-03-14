#pragma once
#include "Client_Defines.h"
#include "MonsterState.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CEngine_Font;
class CDynamic_Mesh;
END

BEGIN(Client)
class CMonsterState;
class CMonster : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMonster(const CMonster& rhs);
	virtual  ~CMonster() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual	HRESULT	Render() override;
public:
	CDynamic_Mesh*	Get_MeshCom() const
	{
		return m_pMeshCom;
	}
	CTransform*		Get_TransformCom() const
	{
		return m_pTransformCom;
	}
public:
	CCollider*		Get_ColliderCom() const
	{
		return m_pColliderCom;
	}
	CCollider*		Get_RecognitionCom() const
	{
		return m_pRecognition_ColliderCom;
	}
	CCollider*		Get_AttackColliderCom() const
	{
		return m_pColliderAttackCom;
	}
	CCollider*		Get_ColliderHand_Left_Com() const
	{
		return m_pColliderHand_LCom;
	}
	CCollider*		Get_ColliderHand_Right_Com() const
	{
		return m_pColliderHand_RCom;
	}
public:
	CCollider*		Get_ColliderArea_Com() const
	{
		return m_pColliderAttackAreaCom;
	}
	CCollider*		Get_VagrantCom() const
	{
		return m_pColliderLegCom;
	}
public:
	void			Set_isHurt(_bool isHurt)
	{
		m_isHurt = isHurt;
	}
	_bool			Get_isHurt()
	{
		return m_isHurt;
	}
public:
	const _int	Get_Hp() const
	{
		return m_iHp;
	}
	void	Sup_Hp(_int iHp)
	{
		m_iHp -= iHp;
	}
public:
	const _int Get_State() const
	{
		return m_iMonsterState;
	}
protected:
	CCollider*				m_pColliderAttackAreaCom	= nullptr;
	CCollider*				m_pColliderLegCom			= nullptr;
protected:
	CCollider*				m_pColliderHand_LCom = nullptr;
	CCollider*				m_pColliderHand_RCom = nullptr;
protected:
	CCollider*		m_pRecognition_ColliderCom	= nullptr;
	CCollider*		m_pColliderAttackCom		= nullptr;
	CCollider*		m_pColliderCom				= nullptr;
protected:
	CShader*		m_pShaderCom				= nullptr;
	CRenderer*		m_pRendererCom				= nullptr;
	CTransform*		m_pTransformCom				= nullptr;
	CDynamic_Mesh*	m_pMeshCom					= nullptr;
protected:
	CEngine_Font*	m_pNameFont					= nullptr;
protected:
	_bool			m_isHurt					= false;
protected:
	_uint			m_iMonsterState				= CMonsterState::MOTION::MOTION_END;
protected:
	_int			m_iHp		= 0;
	_int			m_iMaxHp	= 0;
protected:
	CMonsterState*	m_pMonsterState				= nullptr;
public:
	virtual CGameObject* 	Clone(void* pArg = nullptr) PURE;
	virtual void			Free() override;
};
END

