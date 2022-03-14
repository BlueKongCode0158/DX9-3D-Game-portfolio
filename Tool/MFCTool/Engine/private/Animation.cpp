#include "..\public\Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController)
	:m_pGraphic_Device(pGraphic_Device)
	,m_pAnimationControlor(pAnimationController)
{
	Safe_AddRef(m_pGraphic_Device);
}

CAnimation::CAnimation(const CAnimation & rhs)
	:m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	if (nullptr != rhs.m_pAnimationControlor)
	{
		rhs.m_pAnimationControlor->CloneAnimationController(
			rhs.m_pAnimationControlor->GetMaxNumAnimationOutputs(),
			rhs.m_pAnimationControlor->GetMaxNumAnimationSets(),
			rhs.m_pAnimationControlor->GetMaxNumTracks(),
			rhs.m_pAnimationControlor->GetMaxNumEvents(),
			&m_pAnimationControlor);
	}
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CAnimation::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CAnimation::NativeConstruct()
{
	return S_OK;
}

HRESULT CAnimation::Set_AnimationIndex(_uint iAnimIndex)
{
	if (m_iCurrentAnimationIndex == iAnimIndex)
	{
		return S_OK;
	}

	LPD3DXANIMATIONSET		pAS = nullptr;
	if (FAILED(m_pAnimationControlor->GetAnimationSet(iAnimIndex, &pAS)))
	{
		return E_FAIL;
	}

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;
	m_pAnimationControlor->SetTrackAnimationSet(m_iNewTrack, pAS);

	D3DXTRACK_DESC		TrackDesc;
	m_pAnimationControlor->GetTrackDesc(m_iCurrentTrack, &TrackDesc);
	
	Safe_Release(pAS);

	m_pAnimationControlor->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAnimationControlor->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAnimationControlor->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + 0.2);
	m_pAnimationControlor->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_TimeAcc, 0.2, D3DXTRANSITION_TYPE::D3DXTRANSITION_LINEAR);
	m_pAnimationControlor->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_TYPE::D3DXTRANSITION_LINEAR);

	m_pAnimationControlor->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAnimationControlor->KeyTrackWeight(m_iNewTrack, 0.9f, m_TimeAcc, 0.2, D3DXTRANSITION_TYPE::D3DXTRANSITION_LINEAR);
	m_pAnimationControlor->KeyTrackSpeed(m_iNewTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_TYPE::D3DXTRANSITION_LINEAR);
	
	m_pAnimationControlor->SetTrackPosition(m_iNewTrack, 0.0);
	m_pAnimationControlor->SetTrackSpeed(m_iNewTrack, 1.f);

	m_pAnimationControlor->ResetTime();
	m_TimeAcc = 0.0;

	m_iCurrentTrack = m_iNewTrack;
	m_iCurrentAnimationIndex = iAnimIndex;

	return S_OK;
}

HRESULT CAnimation::Play_Animation(_double TimeDelta)
{
	if (nullptr == m_pAnimationControlor)
	{
		return E_FAIL;
	}

	if (true == m_isStop)
	{
		return S_OK;
	}

	m_TimeAcc += TimeDelta;
	m_pAnimationControlor->AdvanceTime(TimeDelta, nullptr);
	return S_OK;
}


HRESULT CAnimation::Bind_Frame(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != m_pAnimationControlor)
	{
		if (FAILED(D3DXFrameRegisterNamedMatrices(pFrame, m_pAnimationControlor)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CAnimation::Set_FrameAnimation(_double AnimationPosition)
{
	if (nullptr == m_pAnimationControlor)
	{
		return E_FAIL;;
	}

	LPD3DXANIMATIONSET		pAs = nullptr;
	if (FAILED(m_pAnimationControlor->GetAnimationSet(m_iCurrentAnimationIndex, &pAs)))
	{
		return E_FAIL;
	}

	D3DXTRACK_DESC			TrackDesc;
	m_pAnimationControlor->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	TrackDesc.Position = AnimationPosition;
	if (FAILED(m_pAnimationControlor->SetTrackDesc(m_iCurrentTrack, &TrackDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAnimation::Reset_FrameAnimation()
{
	if (nullptr == m_pAnimationControlor)
	{
		return E_FAIL;;
	}

	LPD3DXANIMATIONSET		pAs = nullptr;
	if (FAILED(m_pAnimationControlor->GetAnimationSet(m_iCurrentAnimationIndex, &pAs)))
	{
		return E_FAIL;
	}

	D3DXTRACK_DESC			TrackDesc;
	m_pAnimationControlor->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	TrackDesc.Position = 0;
	if (FAILED(m_pAnimationControlor->SetTrackDesc(m_iCurrentTrack, &TrackDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_bool CAnimation::Get_AnimationStop()
{
	return m_isStop;
}

void CAnimation::Set_AnimationStop(_bool isStop)
{
	m_isStop = isStop;
}

_bool CAnimation::Animation_State_End(_uint iAnimIndex)
{
	if (nullptr == m_pAnimationControlor ||
		m_iCurrentAnimationIndex != iAnimIndex )
	{
		return false;
	}

	LPD3DXANIMATIONSET		pAS = nullptr;
	if (FAILED(m_pAnimationControlor->GetAnimationSet(iAnimIndex, &pAS)))
	{
		return E_FAIL;
	}
	D3DXTRACK_DESC		TrackDesc;
	m_pAnimationControlor->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	if (TrackDesc.Position >= pAS->GetPeriod())
	{
		Safe_Release(pAS);
		return true;
	}

	Safe_Release(pAS);

	return false;
}

_float CAnimation::Play_Animation_Per()
{
	if (nullptr == m_pAnimationControlor)
	{
		return _float();
	}

	LPD3DXANIMATIONSET		pAS = nullptr;
	if (FAILED(m_pAnimationControlor->GetAnimationSet(m_iCurrentAnimationIndex, &pAS)))
	{
		return _float();
	}

	D3DXTRACK_DESC		TrackDesc;
	m_pAnimationControlor->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	_float	fPersec = TrackDesc.Position / pAS->GetPeriod();

	Safe_Release(pAS);

	return fPersec;
}

CAnimation * CAnimation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController)
{
	CAnimation* pInstance = new CAnimation(pGraphic_Device, pAnimationController);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CAnimation_Prototpye");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation * CAnimation::Clone()
{
	CAnimation* pInstance = new CAnimation(*this);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CAnimation_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	if (nullptr != m_pAnimationControlor)
	{
		Safe_Release(m_pAnimationControlor);
	}
	Safe_Release(m_pGraphic_Device);
}
