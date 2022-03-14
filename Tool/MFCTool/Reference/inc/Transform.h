#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT,STATE_UP,STATE_LOOK,STATE_POSITION,STATE_END };
public:
	typedef struct tagTransformDesc
	{
		_float fRotationSpec, fTransformSpec;
	}TRANSFORMDESC;
public:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual  ~CTransform() = default;
public:
	const _matrix*	Get_WorldMatrix() const
	{
		return &m_WorldMatrix;
	}
	_matrix	Get_WorldMatrixInverse() const
	{
		_matrix WorldMatrixInverse{};
		return *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &m_WorldMatrix);
	}
	const _float3	Get_MatrixRow(STATE eState) const
	{
		return *(_float3*)(&m_WorldMatrix.m[eState][0]);
	}
	const _float3	Get_Scale();
public:
	void Set_WorldMatrix(_matrix WorldMatrix)
	{
		m_WorldMatrix = WorldMatrix;
	}
	void Set_WorldMatrixRow(STATE eState, _float3 fVector3)
	{
		memcpy(&m_WorldMatrix.m[eState][0], fVector3, sizeof(_float3));
	}
	void Set_MatrixScale(_float fScaleX, _float fScaleY, _float _fScaleZ);
	void Set_MovingState(_bool eState)
	{
		m_isMove = eState;
	}
	void Set_JumpState(_bool isJump)
	{
		m_isJump = isJump;
	}
	_bool	Get_JumpState()
	{
		return m_isJump;
	}
public:
	void Walk_Back(_float TimeDelta, class CNavigation* pNavigation = nullptr);
	void Walk_Look(_float TimeDelta, class CNavigation* pNavigation = nullptr);
	void Walk_Left(_float TimeDelta, class CNavigation* pNavigation = nullptr);
	void Walk_Right(_float TimeDelta,class CNavigation* pNavigation = nullptr);
public:
	void Jump(_float TimeDelta);
public:
	void SetUp_RotatinAxis(_float3 fAxis, _float fRadian);
	void Rotation_Aixs_Accumulate(_float3 fAxis, _float fRadian);
	void Rotation_Axis(_float3 fAxis, _float TimeDelta);
	void TargetToMove(_float TimeDelta, CTransform* tTarget_Transform);
	void TargetToMove(_float TimeDelta, _float3 TargetPosition);
	void Stand_On_Plane(const CTransform* pPlaneTransform, const class CVIBuffer_Terrain* pTerrain);
public:
	void TargetToRotation(_float3 vTargetPosition);
	void TargetToRotation(CTransform* tTarget_Transform);
public:
	void Remove_Rotation();
public:
	virtual	HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
private:
	_matrix			m_WorldMatrix;
	TRANSFORMDESC	m_TransformInfo;
private:
	_bool			m_isMove;
	_bool			m_isJump = false;
private:
	_float			m_fJumpPower	= 0.35f;
	_float			m_fGravity		= 0.5f;
	_float			m_fAccel		= 0.f;
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg = nullptr) override;
	virtual void		Free() override;
};
END

