#pragma once
#include "Shader.h"
#include "Frustum.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "Static_Mesh.h"
#include "Dynamic_Mesh.h"
#include "Engine_Font.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"


BEGIN(Engine)
class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	explicit CComponent_Manager();
	virtual  ~CComponent_Manager() = default;
public:
	void		Clear(_uint iLevel);
public:
	HRESULT		Reserve_Container(_uint iNumLevel);
	HRESULT		Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent*	Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
private:
	map<const _tchar*, CComponent*>*		m_pProtoType = nullptr;
	typedef map<const _tchar*, CComponent*> PROTOTYPES;
private:
	_uint		m_iNumLevel = 0;
private:
	CComponent*		Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);
	virtual	void	Free() override;
};
END

