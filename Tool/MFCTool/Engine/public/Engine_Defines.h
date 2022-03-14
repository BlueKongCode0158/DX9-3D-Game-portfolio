#pragma once

#pragma warning (disable : 4251)

#pragma region DirectX9

#include <d3d9.h>
#include <d3dx9.h>	

#pragma endregion

#pragma region Default

#include <assert.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <process.h>

#pragma endregion

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma region EngineDefine

#include "Engine_Macro.h"
#include "Engine_Functions.h"
#include "Engine_Typedef.h"
#include "Engine_Functor.h"
#include "Engine_Struct.h"

#pragma endregion

#pragma region Memories_Check
/* ¸¯Ã¼Å© */
#ifdef _ENGINE_DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG
#pragma endregion

using namespace Engine;
using namespace std;