#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(MFC_TOOL)
class CFree_Camera : public CCamera
{
public:
	explicit CFree_Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFree_Camera(const CFree_Camera& rhs);
	virtual	 ~CFree_Camera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
private:
	HRESULT Add_Component();
public:
	static	CFree_Camera*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

