/*
	 _____                            ___________ _   __
	/  ___|                          /  ___|  _  \ | / /
	\ `--.  ___  _   _ _ __ ___ ___  \ `--.| | | | |/ / 
	 `--. \/ _ \| | | | '__/ __/ _ \  `--. \ | | |    \ 
	/\__/ / (_) | |_| | | | (_|  __/ /\__/ / |/ /| |\  \
	\____/ \___/ \__,_|_|  \___\___| \____/|___/ \_| \_/

	Designed for Counter-Strike: Global Offensive
	Copyright: ValvE Corporation 2017 (C)
*/

#pragma once
#include "../SourceCommon.h"
#include "../Interfaces/Engine/IVRenderView.h"
#include "CUtlStack.h"

SOURCE_INIT

struct ViewStack_t
{
	CViewSetup m_View;

	// Matrices
	VMatrix	m_matrixView;
	VMatrix	m_matrixProjection;
	VMatrix	m_matrixWorldToScreen;

	bool m_bIs2DView;
	bool m_bNoDraw;
};

class CRender
{
public:
	char pad_0x0000[0x4]; //0x0000
	float m_yFOV; //0x0004 
	double m_frameStartTime; //0x0008 
	float m_framerate; //0x0010 
	float m_zNear; //0x0014 
	float m_zFar; //0x0018 
	VMatrix m_matrixView; //0x001C 
	VMatrix m_matrixProjection; //0x005C 
	VMatrix m_matrixWorldToScreen; //0x009C 
	CUtlStack<ViewStack_t> m_ViewStack; //0x00DC 
	int m_iLightmapUpdateDepth; //0x00F0 
}; //Size=0x00F4

SOURCE_END