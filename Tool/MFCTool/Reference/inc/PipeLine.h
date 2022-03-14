#pragma once
#include "Base.h"

BEGIN(Engine)
class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
private:
	explicit CPipeLine();
	virtual  ~CPipeLine() = default;
public:
	_matrix		Get_Transform(D3DTRANSFORMSTATETYPE eState)
	{
		return (eState == D3DTS_VIEW) ? m_ViewMatrix : m_ProjMatrix;
	}
	_float3		Get_CamPosition()
	{
		return m_vCamPosition;
	}
public:
	HRESULT		Set_Transform(D3DTRANSFORMSTATETYPE eState, const _matrix& TransformMatrix);
private:
	_matrix		m_ViewMatrix;
	_matrix		m_ProjMatrix;
private:
	_float3		m_vCamPosition;
public:
	virtual void Free() override;
};
END

