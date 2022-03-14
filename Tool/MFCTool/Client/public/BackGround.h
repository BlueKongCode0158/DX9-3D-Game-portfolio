#pragma once
#include "UI.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBackGround final: public CUI 
{
public:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBackGround(CBackGround& rhs);
	virtual ~CBackGround() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	LPDIRECT3DTEXTURE9	m_pTexture		= nullptr;
private:
	HRESULT Add_Components();
public:
	static	CBackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END

