#pragma once
#include "MFC_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CPicking;
class CCollider;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(MFC_TOOL)
class CPivotPoint final : public CGameObject
{
public:
	enum DIR_STATE
	{
		RIGHT, UP, LOOK, CENTER, DIR_END
	};

	typedef struct tagPivotPointInfo
	{
		DIR_STATE			eType				= DIR_END;
		const CTransform*	pTargetTransform	= nullptr;
	}PIVOT_INFO;
private:
	explicit CPivotPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPivotPoint(const CPivotPoint& rhs);
	virtual  ~CPivotPoint() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT	Render()					override;
public:
	HRESULT		Set_TargetObject(CTransform* pTransform);
	HRESULT		Set_PivotType(CPivotPoint::DIR_STATE eType);
	CTransform*	Get_Transform();
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();
	HRESULT SetUp_DirTransform(DIR_STATE eType);
private:
	PIVOT_INFO		m_tPointInfo;
private:
	CShader*		m_pShaderCom	= nullptr;
	CPicking*		m_pPicking		= nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CCollider*		m_pCollidercom	= nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CVIBuffer_Cube*	m_pVIBufferCom	= nullptr;
private:
	_bool			m_isContect		= false;
public:
	static	CPivotPoint*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr)		override;
	virtual void			Free()							override;
};
END
