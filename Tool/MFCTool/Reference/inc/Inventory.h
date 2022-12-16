#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CInventory final : public CComponent
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CInventory(const CInventory& rhs);
	virtual  ~CInventory() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
public:
	virtual HRESULT Add_Item(const _tchar* pItemName, class CItem* pItem);
	virtual HRESULT	Use_Item(const _tchar* pItemName);
private:
	class CItem*	Find_Item(const _tchar* pItemName);
private:
	map<const _tchar*, class CItem*>	m_mapItems;
public:
	static	CInventory*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg)		 override;
	virtual void		Free();
};
END
