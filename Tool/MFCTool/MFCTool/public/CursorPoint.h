#pragma once
#include "MFC_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
END

BEGIN(MFC_TOOL)
class CCursorPoint final : public CGameObject
{
private:
	explicit CCursorPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCursorPoint(const CCursorPoint& rhs);
	virtual  ~CCursorPoint() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	HRESULT	Create_Dynamic_Object();
	HRESULT Create_Static_Object();
	HRESULT	Create_Cell();
	HRESULT Create_Dynamic_Assert_Object();
	HRESULT Create_Static_Assert_Object();
	HRESULT Create_Cell_Assert();
	HRESULT	Create_Monster_Create();
	HRESULT Create_Player_Create();
private:
	void	RadioButton_Remove();
private:
	LPD3DXMESH	m_pMesh			= nullptr;
	CTransform* m_pTransformCom = nullptr;
	CRenderer*	m_pRendererCom	= nullptr;
	CShader*	m_pShaderCom	= nullptr;
public:
	static	CCursorPoint*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

