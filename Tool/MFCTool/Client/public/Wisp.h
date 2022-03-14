#pragma once
#include "Client_Defines.h"
#include "Monster.h"


BEGIN(Client)
class CNormal_Monster_Hp;
class CWisp final : public CMonster
{
private:
	explicit CWisp(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWisp(const CWisp& rhs);
	virtual  ~CWisp() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT Render()					override;
private:
	HRESULT Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CNormal_Monster_Hp*		m_pHpBar = nullptr;
public:
	static  CWisp*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};
END
