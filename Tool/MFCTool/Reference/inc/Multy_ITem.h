#pragma once
#include "Item.h"

BEGIN(Engine)
class ENGINE_DLL CMulty_ITem final : public CItem
{
private:
	explicit CMulty_ITem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMulty_ITem(const CMulty_ITem& rhs);
	virtual	 ~CMulty_ITem() = default;
public:
	virtual HRESULT			NativeConstruct_Prototype()	override;
	virtual HRESULT			NativeConstruct(void* pArg) override;
public:
	virtual void			Add_Item()					override;
	virtual void			Use_Item()					override;
public:
	static	CMulty_ITem*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CItem*			Clone(void* pArg) override;
	virtual void			Free();
};
END
