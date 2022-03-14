#pragma once
#include "MFC_Defines.h"
#include "Base.h"
#include "MeshList.h"

BEGIN(MFC_TOOL)
class CMesh_Manager final : public CBase
{
	DECLARE_SINGLETON(CMesh_Manager)
private:
	explicit CMesh_Manager();
	virtual  ~CMesh_Manager();
public:
	HRESULT	 Add_StaticMeshList(const _tchar* pLayerTag, CGameObject* pMesh);
	HRESULT	 Assert_StaticMeshList(const _tchar* pLayerTag, CGameObject* pMesh);
public:
	HRESULT	 Add_DynamicMeshList(const _tchar* pLayerTag, CGameObject* pMesh);
	HRESULT	 Assert_DynamicMeshList(const _tchar* pLayerTag, CGameObject* pMesh);
public:
	_uint	 Get_StaticMapVectorSize(const _tchar* pLayerTag);
	_uint	 Get_DynamicMapVectorSize(const _tchar* pLayerTag);
private:
	CMeshList*	Find_StaticMeshList(const _tchar* pLayerTag);
	_uint		Find_StaticMeshList_Size(const _tchar* pLayerTag);
private:
	CMeshList*	Find_DynamicMeshList(const _tchar* pLayerTag);
	_uint		Find_DynamicMeshList_Size(const _tchar* pLayerTag);
private:
	map<const _tchar*, CMeshList*>	m_mapStaticMeshList;
	typedef	map<const _tchar*, CMeshList*> STATICLIST;
private:
	map<const _tchar*, CMeshList*>	m_mapDynamicMeshList;
	typedef	map<const _tchar*, CMeshList*> DYNAMICLIST;
public:
	HRESULT	Save_StaticMeshList(const _tchar* pLayerTag);
	HRESULT Save_DynamicMeshList(const _tchar* pLayerTag);
public:
	virtual void Free() override;
};
END
