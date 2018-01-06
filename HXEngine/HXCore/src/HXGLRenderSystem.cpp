#include "..\include\HXGLRenderSystem.h"
#include "vgl.h"
#include "HXDisplayListener.h"
#include "HXGLRenderable.h"
#include "HXGLCamera.h"
#include "HXGLFreeTypeFont.h"
#include "HXGLTransform.h"
#include "HXGLShadowMap.h"

namespace HX3D
{
	HXGLShadowMap* HXGLRenderSystem::mShadowMap = NULL;
	HXGLRenderSystem::HXGLRenderSystem():mFont(NULL)
	{
	}

	HXGLRenderSystem::~HXGLRenderSystem()
	{
		if (mFont)
		{
			delete mFont;
			mFont = NULL;
		}
		if (mShadowMap)
		{
			delete mShadowMap;
			mShadowMap = NULL;
		}
	}

	void HXGLRenderSystem::CreateRenderWindow(std::string strName, int nWidth, int nHeight, bool bFullScreen)
	{
		int one = 1;
		char * name = "name";
		strName += "-GL";

#ifdef _DEBUG
		glutInitContextFlags(GLUT_DEBUG);
#endif

		glutInitWindowSize(nWidth, nHeight);
		gCurScreenWidth = nWidth;
		gCurScreenHeight = nHeight;
		//gAspect = (float)gCurScreenHeight / (float)gCurScreenWidth;
		// glutInitWindowPosition(140, 140);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// 双缓存
		// glutInitContextVersion(4, 3);	// 使用后会宕 ?
		glutInitContextProfile(GLUT_CORE_PROFILE);

		glutInit(&one, &name);

		glutCreateWindow(strName.c_str());

		if (glewInit())
		{
			std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
			exit(EXIT_FAILURE);
		}

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// InitTriangle();
		mFont = new HXGLFreeTypeFont();
		mFont->Initialize("default.ttf", 16);
		mShadowMap = new HXGLShadowMap();
		mShadowMap->Initialize();
	}

	void HXGLRenderSystem::MainLoop()
	{
		glutDisplayFunc(Display);
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(Keyboard);
		glutMouseFunc(Mouse);
		glutMotionFunc(MouseMotion);

		glutMainLoop();
	}

	void HXGLRenderSystem::Display()
	{
		//begin shadow
		if (mShadowMap)
		{
			mShadowMap->PreRender();
			// TODO: draw
			if (m_pDisplayListener)
			{
				// TODO: gameobject和UI分离
				m_pDisplayListener->OnDisplay(true);
			}
			mShadowMap->PostRender();
		}
		//end shadow

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_pDisplayListener)
		{
			// TODO: gameobject和UI分离
			m_pDisplayListener->OnDisplay(false);
		}

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void HXGLRenderSystem::Reshape(int nWidth, int nHeight)
	{
		glViewport(0, 0, nWidth, nHeight);
		HXRenderSystem::Reshape(nWidth, nHeight);
	}

	HXRenderable* HXGLRenderSystem::GenerateRenderable(HXSubMesh* pSubMesh)
	{
		HXRenderable* pRenderable = new HXGLRenderable();
		pRenderable->GenerateArguments(pSubMesh);
		return pRenderable;
	}


	void HXGLRenderSystem::RenderSingle(HXRenderable* pRenderable, bool shadow)
	{
		if (shadow)
		{
			pRenderable->RenderShadowMap();
		}
		else
		{
			pRenderable->Render();
		}
	}

	HXICamera* HXGLRenderSystem::CreateCamera(const HXVector3D& eye, const HXVector3D& at, const HXVector3D& up,
		float ffov, float nearZ, float farZ, float viewportWidth, float viewportHeigth,
		float left, float right, float bottom, float top)
	{
		HXICamera* pCam = new HXGLCamera();
		pCam->Initialize(eye, at, up, ffov, nearZ, farZ, viewportWidth, viewportHeigth, left, right, bottom, top);
		return pCam;
	}

	void HXGLRenderSystem::BeginText()
	{
		if (mFont)
		{
			mFont->BeginText();
		}
	}

	void HXGLRenderSystem::_DrawText(float x, float y, std::string text, const HXCOLOR& color)
	{
		if (mFont)
		{
			std::wstring wstr = std::wstring(text.begin(), text.end());
			mFont->_DrawText(x, y, wstr.c_str(), color);
		}
	}

	void HXGLRenderSystem::EndText()
	{
		if (mFont)
		{
			mFont->EndText();
		}
	}

	HXITransform* HXGLRenderSystem::CreateTransform()
	{
		return new HXGLTransform();
	}
}
