#pragma once
#include "MFC_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(MFC_TOOL)
class CCellPoint final : public CGameObject
{
private:
	explicit CCellPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCellPoint(const CCellPoint& rhs);
	virtual  ~CCellPoint() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT	NativeConstruct(void* pArg);
	virtual _int	Tick(double TimeDelta);
	virtual _int	Late_Tick(double TimeDelta);
	virtual HRESULT Render();
public:
	_bool	Get_isSelect()
	{
		return m_isSelect;
	}
	void	Set_isSelect(_bool isSelect)
	{
		m_isSelect = isSelect;
	}
	void	Set_isDead(_bool isDead)
	{
		m_isDead = isDead;
	}
	_float3	Get_Position();
	_float3	Get_LocalPosition();
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CShader*			m_pShaderCom	= nullptr;
	CRenderer*			m_pRendererCom	= nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom	= nullptr;
private:
	_bool				m_isSelect	= false;
	_bool				m_isDead	= false;
public:
	static  CCellPoint*  Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)	override;
	virtual void		 Free()				override;
};
END
