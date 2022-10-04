#pragma once
#include "io.h"
#include "fmod.h"
#include "Sound_Manager.h"
#define RETURN(INPUT)				\
RELEASE_INSTANCE(CGameInstacne);	\
return INPUT

enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_LOADING, LEVEL_SELECT ,LEVEL_GAMEPLAY0, LEVEL_GAMEPLAY1, LEVEL_GAMEPLAY2 , LEVEL_END };

const unsigned int iWinCX = 1920;
const unsigned int iWinCY = 1080;

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

extern	bool		g_isCheat;

namespace Client {}
using namespace Client;