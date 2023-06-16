#pragma once
#include "Base.h"

/*
������Ʈ�� ������Ʈ�� ������ �ֵ��� ����.
�ʿ��� ����� ���� �� �ش� ����� ������Ʈ�� ������Ʈ�� ����־� �۵� �ǵ��� �����.
*/

BEGIN(Engine)
class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	// ������Ÿ�� �����Լ�.
	virtual	HRESULT NativeConstruct_Prototype();
	// Clone �����Լ�.
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	// ������Ʈ Render ( �׸� �� ) ���ư��� ���
	virtual HRESULT Render();
protected:
	LPDIRECT3DDEVICE9	m_pGraphic_Device	= nullptr;
	_bool				m_isClone			= false;
public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;
	virtual void		Free() override;
};
END
