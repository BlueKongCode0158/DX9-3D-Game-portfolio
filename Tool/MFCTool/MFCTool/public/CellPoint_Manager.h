#pragma once
#include "Base.h"
#include "MFC_Defines.h"
#include "CellPoint.h"

BEGIN(MFC_TOOL)
class CCellPoint_Manager final : public CBase
{
	DECLARE_SINGLETON(CCellPoint_Manager)
private:
	explicit CCellPoint_Manager();
	virtual  ~CCellPoint_Manager();
public:
	void	Add_CellPoint(CCellPoint* pPoint);
	void	Insert_CellPoint(_uint iIndex, CCellPoint* pPoint);
	void	Cut_vecCellPoint();
	_uint	Find_CellPointIndex();
public:
	_uint	Get_CellPoint_Size()
	{
		return m_VecCellPoint.size();
	}
public:
	_float3*	is_Point_Collision(_float3 vPosition, CCellPoint** ppCellPoint);
public:
	HRESULT	Index_VecCellPoint_Select(_uint iIndex);
	HRESULT	Select_VecCellPoint_Delete(_uint iIndex);
public:
	HRESULT	Save_VecCellPoint(const _tchar* pFileName);
	HRESULT	Load_VecCellPoint(const _tchar* pFileName);
public:
	_bool	Is_in(_float3 vPosition, CCellPoint*& ppCellPoint);
public:
	HRESULT	CellPoint_CW_Sort();
#ifdef _DEBUG
	HRESULT	Render(const _matrix* pWorldMatrix);
#endif // _DEBUG
private:
	vector<CCellPoint*>	m_VecCellPoint;
private:
	_float3		m_vMinPosition	= _float3(0.f, 0.f, 0.f);
	LPD3DXLINE	m_pLine			= nullptr;
public:
	virtual void Free() override;
};
END
