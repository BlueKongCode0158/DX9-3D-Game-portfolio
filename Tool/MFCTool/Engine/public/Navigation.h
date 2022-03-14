#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL  CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_uint	iCurrentCellIndex = 0;
	}NAVIDESC;
private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNavigation(const CNavigation& rhs);
	virtual  ~CNavigation() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pDataFiles);
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT	SetUp_Neighbor();
	_bool	Move_OnNavigation(_float3 vPosition, _float3** vNormal);
	HRESULT	Update_WorldMatrix(const _matrix* pWorldMatrix);
#ifdef _DEBUG
public:
	virtual HRESULT Render() override;
#endif // DEBUG
private:
	vector<class CCell*>			m_vecCells;
	typedef vector<class CCell*>	CELLS;
private:
	NAVIDESC	m_NeviDesc;
	_matrix		m_WorldMatrix;
public:
	static	CNavigation*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pDataFiles);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};
END

