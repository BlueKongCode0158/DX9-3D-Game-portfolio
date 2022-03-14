#pragma once
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		_float3 fEye;
		_float3 fAt;
		_float3 fAxisY = _float3(0.f, 1.f, 0.f);

		_float fFov;
		_float fAspect;
		_float fNear;
		_float fFar;
	}CAMERADESC;
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual  ~CCamera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT	Set_Transform();
protected:
	HRESULT Set_CameraTransform();
protected:
	CAMERADESC			m_CameraDesc;
	class CPipeLine*	m_pPipeLine		= nullptr;
	class CTransform*	m_pTransform	= nullptr;
public:
	virtual CGameObject*	Clone(void* pArg = nullptr)PURE;
	virtual void			Free()override;
};
END

