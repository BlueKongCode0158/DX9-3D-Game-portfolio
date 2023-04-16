#pragma once
#include "Base.h"

BEGIN(Engine)
class CStatic_Mesh;
class CGraphic_Device;
class ENGINE_DLL CShape : public CBase
{
private:
	explicit CShape();
	virtual  ~CShape() = default;
private:
	HRESULT	NativeContruct(void* pArg);
	HRESULT	SetUp_Mesh(const _tchar * pFilePath, const _tchar* pFileName);
public:
	CStatic_Mesh*	Get_Mesh()
	{
		return m_pMesh;
	}
	_float3	Get_RandomVector();
private:
	CStatic_Mesh*		m_pMesh = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphicDevice = nullptr;
public:
	static	CShape*	Create(void* pArg);
	virtual void	Free() override;
};
END

