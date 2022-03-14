#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CDynamic_Mesh;
END

BEGIN(Client)
class CPod final : public CGameObject
{
private:
	explicit CPod(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPod(const CPod& rhs);
	virtual  ~CPod() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	_bool	isOpen();
	_bool	isFinish();
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	CShader*		m_pShaderCom	= nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CDynamic_Mesh*	m_pMeshCom		= nullptr;
private:
	_bool			m_isOpen = false;
	_bool			m_isFinish = false;
public:
	static  CPod*		 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;;
	virtual void Free() override;
};
END
