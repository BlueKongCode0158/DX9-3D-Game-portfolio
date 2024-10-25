#pragma once
#include "Base.h"
// - Tranform �� �̿��Ͽ� Gizmo�� Render�ϴ� Class

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

