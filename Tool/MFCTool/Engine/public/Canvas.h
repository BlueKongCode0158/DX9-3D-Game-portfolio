#pragma once
#include <GameObject.h>

/*
3D UI 부모격 Canvas [ Unity 에서 아이디어를 따왔다. ]
*/

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class ENGINE_DLL CCanvas : public CGameObject
{
public:
	enum TYPE
	{
		LEFT_TOP, RIGHT_TOP, RIGHT_BOTTOM, LEFT_BOTTOM, CENTER, TYPE_END
	};
private:
	explicit CCanvas(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCanvas(const CCanvas& rhs);
	virtual  ~CCanvas() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Add_Component(_uint LEVEL);
	HRESULT SetUp_ConstantTable();
private:
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
private:
	_float3		m_vOriginal_Point[TYPE_END];
	_float3		m_vWorld_Point[TYPE_END];
	D3DXPLANE	m_Plan;
private:
	_bool m_isSetting = false;
public:
	static CCanvas*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;;
	virtual void			Free() override;
};
END
