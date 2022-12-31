#pragma once
#include "Base.h"

BEGIN(Engine)
class CShader;
class CAttribute;
class CTransform;
class CVIBuffer_Point;
class ENGINE_DLL CParticle_System : public CBase
{
public:
	typedef struct PSystemDesc
	{
		_int	m_iMaxParticle = 0;
		_int	m_iMinParticle = 0;
		_bool	m_isEmission = true;
		_float	m_fEmissionTime = 0.f;
	}PSDESC;
private:
	explicit CParticle_System(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticle_System(const CParticle_System& rhs);
	virtual  ~CParticle_System() = default;
public:
	HRESULT	NativeConstruct_Prototype();
	HRESULT	NativeConstruct(void* pArg);
public:
	HRESULT	Reset();
	_int	Tick(_float Time_Delta);
	_int	Late_Tick(_float Time_Delta);
	HRESULT	Render();
public:
	void	Set_Index(_int iIndex);
	_int	Get_Index();
public:
	_bool	Set_ParticleDesc(PSDESC* pArg);
private:
	HRESULT	SetUp_ConstantTable();
private:
	list<CAttribute*>	m_pAttributeList;
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransfomCom = nullptr;
	CVIBuffer_Point*	m_pVIBufferCom = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
private:
	_int				m_iIndex = 0;
private:
	PSDESC				m_tDesc;
public:
	static CParticle_System*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticle_System*			Clone(void* pArg);
	virtual void				Free() override;
};
END
