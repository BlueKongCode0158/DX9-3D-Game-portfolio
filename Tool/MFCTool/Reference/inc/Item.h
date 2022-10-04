#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CItem : public CBase
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem(const CItem& rhs);
	virtual  ~CItem() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	virtual	void	Add_Item()				PURE;
	virtual void	Use_Item()				PURE;
private:
	_matrix				m_ProjMatrix;
	_matrix				m_OrhProjMatrix;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	virtual CItem*	Clone(void* pArg)		PURE;
	virtual void	Free()					override;
};
END
