#pragma once
#include "Base.h"
// - Tranform 을 이용하여 Gizmo를 Render하는 Class

BEGIN(Engine)
class ENGINE_DLL Gizmo : public CBase
{
private:
	explicit Gizmo();
	virtual  ~Gizmo() = default;
private:
	HRESULT NativeConstruct_Prototype();
	HRESULT NativeContruct(void* pArg);
public:

	HRESULT	Render();
public:
	static	Gizmo*	Create(void* pArg);
	virtual void	Free() override;
private:

};
END

