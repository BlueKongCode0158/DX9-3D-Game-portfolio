#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CNavigation;
class CCollider;
class CRenderer;
class CTransform;
class CDynamic_Mesh;
END

BEGIN(Client)
class CPlayerState;
class CPlayer final : public CGameObject
{
public:
	enum ANIMATION_KEY
	{
		CUTSCENE_ANIMATION,
		RELOAD_PISTOLSEXIT_OVERRIDE,
		RELOAD_PISTOLSEXIT_ADDITIVE,
		RELOAD_PISTOLS_OVERRIDE,
		RELOAD_PISTOLS_ADDITIVE,
		SLIDE_FORWARD,
		THROWGRENADE_OVERRIDE,
		THROWGRENADE_ADDITIVE,
		JUMP,
		POSE_LOOP,
		FIRE_BARRAGE_OVERRIDE,
		FLINCH_3,
		FLINCH_2,
		FLINCH_1,
		FIRE_BARRAGE_ADDITIVE,
		FIRE_FMJ_ADDITIVE,
		RUN_RIGHT,
		RUN_BACKWARD,
		IDLE_LOOP,
		SPRINT_FORWARD,
		ROLL_BACKWARD,
		ROLL_RIGHT,
		ROLL_LEFT,
		ROLL_FORWARD,
		FIRE_PISTOL_RIGHT,
		FIRE_PISTOL_LEFT,
		RUN_LEFT,
		FIRE_FMJ_OVERRIDE,
		LIGHT_IMPACT,
		AIR_UP,
		AIR_DOWN,
		RUN_FOWARD,
		IDLE_IN,
		AIM_VERTICAL,
		AIM_HORIZONTAL,
		ANIMATIONKEY_END
	};
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual  ~CPlayer() = default;
public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;
	virtual _int		Tick(double TimeDelta) override;
	virtual _int		Late_Tick(double TimeDelta) override;
	virtual HRESULT		Render() override;
	virtual HRESULT		Render_Shadow();
private:
	HRESULT		Add_Component();
	HRESULT		SetUp_ConstantTable();
public:
	_float*		Get_HpPer();
public:
	HRESULT	Set_PlayerAnimation(_uint iIndex_Upper, _uint iIndex_Lower);
	void	Set_PlayerSpeed(_float fSpeed);
	void	Set_PlayerDead(_bool isDead);
	void	Set_PlayerAnimationSpeed(_float fUpperSpeed, _float fLowerSpeer);
public:
	CDynamic_Mesh*	Get_MeshCom();
	CTransform*		Get_TransformCom();
private:
	void		Character_MoveKey(_double TimeDelta);
private:
	void		Update_HpPer();
private:
	CNavigation*		m_pNavigationCom	= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CTransform*			m_pTransformCom		= nullptr;
	CDynamic_Mesh*		m_pMeshCom			= nullptr;
	CCollider*			m_pColliderCom		= nullptr;
	CCollider*			m_pOBBColliderCom	= nullptr;
	CShader*			m_pShaderCom		= nullptr;
private:
	CPlayerState*		m_pPlayerState		= nullptr;
private:
	_float				m_fSpeed			= 0.f;
	_float				m_fUpperSpeed		= 1.f;
	_float				m_fLowerSpeer		= 1.f;
	_float3	*			m_pTarget			= nullptr;
	_float				m_fBulletTime		= 0.f;
	_float				m_fTimeDelta		= 0.f;
	_float				m_fHpPer			= 0.f;
private:
	_bool				m_isDead			= false;
	_int				m_iHp				= 0;
	_int				m_iMaxHp			= 0;
private:
	_bool				m_isHurt			= false;
public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

