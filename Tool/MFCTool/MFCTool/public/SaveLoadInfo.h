#pragma once
#include "Base.h"
#include "MFC_Defines.h"

BEGIN(MFC_TOOL)
class CSaveLoadInfo final : public CBase
{
private:
	explicit CSaveLoadInfo();
	virtual  ~CSaveLoadInfo() = default;
public:
	virtual	HRESULT NativeConstruct(HWND hWnd);
	virtual HRESULT Texture_SaveInfo(LPDIRECT3DTEXTURE9	pTexture, const _tchar* pFileName);
private:
	HWND	m_hWnd;
public:
	static CSaveLoadInfo*	Create(HWND m_hWnd);
	virtual void			Free() override;
};
END

