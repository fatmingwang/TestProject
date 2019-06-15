#include "stdafx.h"
#include "GameApp.h"
#include "TestMap.h"
#include "Event.h"
#include "LogID.h"
cBaseImage*g_pRMImage = nullptr;
#if defined(ANDROID)
cTestApp::cTestApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cTestApp::cTestApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cTestApp::cTestApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	FMLog::LogWithFlag("cTestMap start ", LOG_ID_TESTAPP, false);
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
	g_pRMImage = new cBaseImage("Test/RM.png");
}

cTestApp::~cTestApp()
{
	cTestMap::DestroyInstance();
	Destroy();
}

void	cTestApp::Init()
{
	FMLog::LogWithFlag("cTestApp::Init start ", LOG_ID_TESTAPP, false);
#ifndef WASM
	cGameApp::Init();
#endif
	cTestMap::GetInstance()->Init();
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
	FMLog::LogWithFlag("cTestApp::Init end ", LOG_ID_TESTAPP, false);
}

void	cTestApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	cTestMap::GetInstance()->Update(e_fElpaseTime);
}

void	cTestApp::Render()
{
	//glEnable(GL_ALPHA_TEST);,opengl es 2 don't have this one.
	MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();

	cTestMap::GetInstance()->Render();
	if (cGameApp::m_sbDebugFunctionWorking)
	{
		//cGameApp::ShowInfo();
		cTestMap::GetInstance()->DebugRender();
	}
	if (g_pRMImage)
	{
		g_pRMImage->SetLocalPosition(Vector3(cGameApp::m_svGameResolution.x/2 - g_pRMImage->GetWidth()/2, cGameApp::m_svGameResolution.y / 2,0 - g_pRMImage->GetHeight() / 2));
		g_pRMImage->Render();
	}
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
	if (g_pRMImage)
	{
		SAFE_DELETE(g_pRMImage);
	}
    cGameApp::MouseDown(e_iPosX,e_iPosY);
}

void	cTestApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
}

void	cTestApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
}

void	cTestApp::KeyDown(char e_char)
{
	if (g_pRMImage)
	{
		SAFE_DELETE(g_pRMImage);
	}
	 cGameApp::KeyDown(e_char);
	 seT_E_KEY_DOWNData l_seT_E_KEY_DOWNData;
	 l_seT_E_KEY_DOWNData.bPressed = true;
	 l_seT_E_KEY_DOWNData.ucKey = e_char;
	 cGameApp::EventMessageShot(eT_E_KEY_DOWN, &l_seT_E_KEY_DOWNData, sizeof(l_seT_E_KEY_DOWNData));


}
void	cTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	seT_E_KEY_DOWNData l_seT_E_KEY_DOWNData;
	l_seT_E_KEY_DOWNData.bPressed = false;
	l_seT_E_KEY_DOWNData.ucKey = e_char;
	cGameApp::EventMessageShot(eT_E_KEY_DOWN, &l_seT_E_KEY_DOWNData, sizeof(l_seT_E_KEY_DOWNData));
}