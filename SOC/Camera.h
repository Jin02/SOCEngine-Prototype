#pragma once

#include "Transform.h"
#include "Frustum.h"
#include "Skybox.h"
#include "LightManager.h"
#include "Component.h"
#include "Shader.h"
#include "RenderTarget.h"

namespace Rendering
{
	class Camera : public Component
	{
	public:
		static const Component::Type ComponentType = Component::Type::Camera;

	public:
		enum Type { Perspective, Orthographic };
		enum ClearFlag { FlagSkybox, FlagSolidColor, FlagTarget, FlagDontClear };
		//CLEAR_FLAG_DEPTHONLY�� ������. ��� �����϶�°��� �� �𸣰��� -��-;

	private:
		Frustum			*frustum;
		Shader::Shader	*rtShader;
		Texture::RenderTarget *renderTarget;

	private:
		Common::Rect<float>	  normalizedViewPortRect;

	public:  //���� private�� �� �ʿ�� ����.
		float				FOV;
		float				clippingNear;
		float				clippingFar;
		ClearFlag			clearFlag;
		Type				camType;
		float				aspect;
		Color				clearColor;
		Skybox				*skybox;
		//1. hdr�� ���⼭ ó���ϴµ�?
		/*2. �ø� ����ũó������ �������� �±׸� ������ �� �־�.
		     �׷� �±� �ý����� �ٲ�� �Ѵ� �Ҹ��� ��.. */
		//3. ���� �ؽ��� ����. �̰� ���� �Ұ� �ƴϾ�


	public:
		Camera();
		~Camera(void);

	private:
		void CalcAspect();
		void Clear(Device::Graphics *gp);
		void RenderObjects(std::vector<Object*>::iterator &objectBegin,	std::vector<Object*>::iterator &objectEnd,	Light::LightManager* sceneLights);

	public:
		void GetPerspectiveMatrix(SOC_Matrix *outMatrix, float farGap);
		void GetOrthoGraphicMatrix(SOC_Matrix *outMatrix);
		void GetProjectionMatrix(SOC_Matrix *outMatrix, float farGap = 0);
		void GetViewMatrix(SOC_Matrix *outMatrix);
		void GetViewProjectionMatrix(SOC_Matrix *outMatrix, float farGap = 0);

	public:
		//static void SceneUpdate(float dt, std::vector<Object*> *sceneObjects);
		static void SceneRender(Camera *cam, 
			std::vector<Object*>::iterator &objectBegin,
			std::vector<Object*>::iterator &objectEnd,
			Light::LightManager* sceneLights);

		void Render(std::vector<Object*>::iterator &objectBegin,
			std::vector<Object*>::iterator &objectEnd,
			Light::LightManager* sceneLights);

	public:
		void SetViewPort(Common::Rect<float> rect);

	public:
		virtual void Initialize();
		virtual void Destroy();
//		virtual Component::Type GetComponentType();
	};
}