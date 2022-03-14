#pragma once
#include "Base.h"
#include "MFC_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(MFC_TOOL)
class CMeshList final : public CBase
{
private:
	explicit CMeshList();
	virtual  ~CMeshList() = default;
public:
	HRESULT	 Add_Mesh(CGameObject* pGameObject);
	HRESULT	 Assert_Mesh(_uint iIndex, CGameObject* pGameObject);
	_uint	 Get_vecMeshList_Size()
	{
		return m_vecMeshList.size();
	}
private:
	vector<CGameObject*>	m_vecMeshList;
public:
	static	CMeshList*	Create();
	virtual	void		Free() override;
};
END
