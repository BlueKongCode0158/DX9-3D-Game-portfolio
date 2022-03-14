#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
class CCharacter_Camera final : public CCamera
{
private:
	explicit CCharacter_Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCharacter_Camera(const CCharacter_Camera& rhs);
	virtual  ~CCharacter_Camera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT Add_Component();
private:
	const _matrix*	m_pParentWorldMatrix = nullptr;
	const _matrix*	m_pParentBoneMatrix = nullptr;
	_matrix		m_OriginMatrix;
private:
	const _matrix* m_pEndMatrix = nullptr;
	const _matrix* m_pForwardMatrix = nullptr;
private:
	/* 맨 처음 카메라 위치 정할 때*/
	_bool	m_isCameraSetting = false;
private:
	_bool	m_isLookSetting = false;
	_float3	m_AccYAxisVector = _float3(0.f,0.f,0.f);
public:
	static CCharacter_Camera*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera*		Clone(void* pArg);
	virtual void			Free() override;
};
END
