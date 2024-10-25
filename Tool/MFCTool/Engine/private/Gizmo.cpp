#include "..\public\Gizmo.h"

Gizmo::Gizmo()
{
}

HRESULT Gizmo::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT Gizmo::NativeContruct(void * pArg)
{
	return S_OK;
}

HRESULT Gizmo::Render()
{
	return S_OK;
}

Gizmo * Gizmo::Create(void * pArg)
{
	return nullptr;
}

void Gizmo::Free()
{
}
