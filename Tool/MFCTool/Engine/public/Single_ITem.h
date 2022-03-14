#pragma once
#include "Item.h"

BEGIN(Engine)
class ENGINE_DLL CSingle_ITem final : public CItem
{
private:
	explicit CSingle_ITem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSingle_ITem(const CSingle_ITem& rhs);
	virtual	 ~CSingle_ITem() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype()	override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	virtual void		 Add_Item()				override;
	virtual void		 Use_Item()			override;
public:
	static CSingle_ITem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CItem *		 Clone(void * pArg)		override;
	virtual void		 Free()					override;
};
END
