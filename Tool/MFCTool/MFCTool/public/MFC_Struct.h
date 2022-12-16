#pragma once

typedef struct Terrain_Info
{
	CString		 StringName{};
	CString		 Layer_Name{};
	CString		 Terrain_Prototype_Name{};
	unsigned int iTerrainX	= 0;
	unsigned int iTerrainZ	= 0;
}TERRAININFO;

typedef	struct tagCell_Info
{
	_uint		uiIndex;
	_float3		vPoint[3];
	_float3*	vNeighbor[3];
}CELLINFO;

typedef	struct tagObject_Info
{
	_matrix		matrix_OriginState;
}OBJECTINFO;

typedef	struct tagUI_Info
{
	_float3		vScale;
	_float3		vPosition;
	_float		fRadian;
}UIINFO;