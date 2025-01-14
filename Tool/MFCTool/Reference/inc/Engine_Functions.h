#pragma once
namespace Engine
{
	float static GetRandomFloat(float fLowBound, float fHighBound)
	{
		if (fLowBound >= fHighBound)
		{
			return fLowBound;
		}

		float fTemp = (rand() % 10000) * 0.0001f;

		return (fTemp*(fHighBound - fLowBound)) + fLowBound;
	}

	void static GetRandomVector(D3DXVECTOR3& rOut, D3DXVECTOR3& rMin, D3DXVECTOR3& rMax)
	{
		rOut.x = GetRandomFloat(rMin.x, rMax.x);
		rOut.y = GetRandomFloat(rMin.y, rMax.y);
		rOut.z = GetRandomFloat(rMin.z, rMax.z);
	}

	void static GetRandomVector_Circle(D3DXVECTOR3& rOut, float fRadian, D3DXVECTOR3* pCenter)
	{
		D3DXVECTOR3 vDir = D3DXVECTOR3(GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), 0.f);
		D3DXVec3Normalize(&vDir, &vDir);

		vDir *= fRadian;
		if (nullptr != pCenter)
		{
			vDir += *pCenter;
		}
		rOut = vDir;
	}

	void static GetRandomVector_Rotate_AxisX(D3DXMATRIX& rOut, float fRadian)
	{
		float fRandomRadian = GetRandomFloat(min(0.f, fRadian), max(0.f, fRadian));
		D3DXMatrixRotationX(&rOut, fRandomRadian);
	}

	void static GetRandomVector_Rotate_AxisY(D3DXMATRIX& rOut, float fRadian)
	{
		float fRandomRadian = GetRandomFloat(min(0.f, fRadian), max(0.f, fRadian));
		D3DXMatrixRotationY(&rOut, fRandomRadian);
	}

	void static GetRandomVector_Rotate_AxisZ(D3DXMATRIX& rOut, float fRadian)
	{
		float fRandomRadian = GetRandomFloat(min(0.f, fRadian), max(0.f, fRadian));
		D3DXMatrixRotationZ(&rOut, fRandomRadian);
	}

	void static GetDirVertor(D3DXVECTOR3& rOut, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd)
	{
		rOut.x = vEnd.x - vStart.x;
		rOut.y = vEnd.y - vStart.y;
		rOut.z = vEnd.z - vStart.z;
	}
}

template <typename T>
void Safe_Delete(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete[] pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
unsigned long Safe_AddRef(T& pInstance)
{
	unsigned long		dwRefcnt = 0;

	if (nullptr != pInstance)
		dwRefcnt = pInstance->AddRef();

	return dwRefcnt;
}


template <typename T>
unsigned long Safe_Release(T& pInstance)
{
	unsigned long		dwRefcnt = 0;

	if (nullptr != pInstance)
	{
		dwRefcnt = pInstance->Release();

		if (0 == dwRefcnt)
			pInstance = nullptr;
	}

	return dwRefcnt;
}


// Len은 MAX_PATH로 통일한다.
void static WcharToChar(const wchar_t* pInText, char* pOutText)
{
	int		iLen = MAX_PATH;
	WideCharToMultiByte(CP_ACP, 0, pInText, iLen, pOutText, iLen, 0, 0);
}

// Len은 MAX_PATH로 통일한다.
void static CharToWchar(char* pInText, wchar_t* pOutText)
{
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pInText, strlen(pInText), pOutText, MAX_PATH);
}