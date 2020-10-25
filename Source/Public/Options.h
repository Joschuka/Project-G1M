#pragma once

#ifndef G1MOPT_H
#define G1MOPT_H

//bMerge
void getMerge(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::merge"), buffer, 1))
	{
		bMerge = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bMerge);
}
int setMerge(int handle, void* userData)
{
	bMerge = !bMerge;
	BYTE buffer[1] = { bMerge };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::merge"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bMerge);
	return 1;
}

//bMergeG1MOnly
void getMergeG1MOnly(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::mergeG1MOnly"), buffer, 1))
	{
		bMergeG1MOnly = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bMergeG1MOnly);
}
int setMergeG1MOnly(int handle, void* userData)
{
	bMergeG1MOnly = !bMergeG1MOnly;
	BYTE buffer[1] = { bMergeG1MOnly };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::mergeG1MOnly"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bMergeG1MOnly);
	return 1;
}

//bG1TMergeG1MOnly
void getG1TMergeG1MOnly(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::G1TMergeG1MOnly"), buffer, 1))
	{
		bG1TMergeG1MOnly = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bG1TMergeG1MOnly);
}
int setG1TMergeG1MOnly(int handle, void* userData)
{
	bG1TMergeG1MOnly = !bG1TMergeG1MOnly;
	BYTE buffer[1] = { bG1TMergeG1MOnly };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::G1TMergeG1MOnly"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bG1TMergeG1MOnly);
	return 1;
}

//bAdditive
void getAdditive(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::additive"), buffer, 1))
	{
		bAdditive = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bAdditive);
}
int setAdditive(int handle, void* userData)
{
	bAdditive = !bAdditive;
	BYTE buffer[1] = { bAdditive };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::additive"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bAdditive);
	return 1;
}

//bColor
void getColor(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::color"), buffer, 1))
	{
		bColor = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bColor);
}
int setColor(int handle, void* userData)
{
	bColor = !bColor;
	BYTE buffer[1] = { bColor };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::color"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bColor);
	return 1;
}

//bDisplayDriver
void getDisplayDriver(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::displayDriver"), buffer, 1))
	{
		bDisplayDriver = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bDisplayDriver);
}
int setDisplayDriver(int handle, void* userData)
{
	bDisplayDriver = !bDisplayDriver;
	BYTE buffer[1] = { bDisplayDriver };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::displayDriver"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bDisplayDriver);
	return 1;
}

//bDisableNUNNodes
void getDisableNUNNodes(int handle)
{
	BYTE buffer[1];
	if (g_nfn->NPAPI_UserSettingRead(const_cast<wchar_t*>(L"g1m::disableNUN"), buffer, 1))
	{
		bDisableNUNNodes = buffer[0] == 1;
	}
	g_nfn->NPAPI_CheckToolMenuItem(handle, bDisableNUNNodes);
}
int setDisableNUNNodes(int handle, void* userData)
{
	bDisableNUNNodes = !bDisableNUNNodes;
	BYTE buffer[1] = { bDisableNUNNodes };
	g_nfn->NPAPI_UserSettingWrite(const_cast<wchar_t*>(L"g1m::disableNUN"), buffer, 1);
	g_nfn->NPAPI_CheckToolMenuItem(handle, bDisableNUNNodes);
	return 1;
}

#endif // !G1MOPT_H

