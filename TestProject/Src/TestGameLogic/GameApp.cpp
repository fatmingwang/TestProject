#include "stdafx.h"
#include "GameApp.h"


#if defined(ANDROID)
cTestApp::cTestApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize,NvEGLUtil*e_pNvEGLUtil ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#elif defined(WIN32)
cTestApp::cTestApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cTestApp::cTestApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	this->m_sbDebugFunctionWorking = true;
	this->m_sbSpeedControl = true;
	m_bLeave = false;
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
}

cTestApp::~cTestApp()
{
	Destroy();
}

void	cTestApp::Init()
{
	cGameApp::Init();	
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cTestApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
}

void	cTestApp::Render()
{
	//glEnable(GL_ALPHA_TEST);,opengl es 2 don't have this one.
	MyGLEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	cGameApp::ShowInfo();

#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
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
	 cGameApp::KeyDown(e_char);
}
void	cTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
}