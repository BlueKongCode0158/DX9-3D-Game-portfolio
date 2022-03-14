#include "..\public\PipeLine.h"


IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
	ZeroMemory(&m_vCamPosition, sizeof(_float3));
}

HRESULT CPipeLine::Set_Transform(D3DTRANSFORMSTATETYPE eState, const _matrix & TransformMatrix)
{
	if (D3DTS_VIEW == eState)
	{
		m_ViewMatrix = TransformMatrix;

		_matrix ViewMatrixInverse;
		D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &m_ViewMatrix);

		memcpy(&m_vCamPosition, &ViewMatrixInverse.m[3][0], sizeof(_float3));
	}
	else if (D3DTS_PROJECTION == eState)
	{
		m_ProjMatrix = TransformMatrix;
	}
	else
	{
		MSGBOX("PipeLine is Error");
		return E_FAIL;
	}
	return S_OK;
}

void CPipeLine::Free()
{
}
