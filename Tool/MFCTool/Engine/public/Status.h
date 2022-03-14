#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CStatus final : public CComponent
{
public:
	typedef struct tagStatusDesc
	{
		_uint	iHp;
	}STATUSDESC;
private:
	explicit CStatus(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatus(const CStatus& rhs);
	virtual  ~CStatus() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	static	CStatus*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg)		override;
	virtual void		Free();
};
END
