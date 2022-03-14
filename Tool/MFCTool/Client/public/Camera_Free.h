#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
class CCamera_Free final : public CCamera
{
private:
	explicit CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Free(const CCamera_Free& rhs);
	virtual  ~CCamera_Free() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT Add_Component();
public:
	static CCamera_Free*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera*		Clone(void* pArg);
	virtual void			Free() override;
};
END
