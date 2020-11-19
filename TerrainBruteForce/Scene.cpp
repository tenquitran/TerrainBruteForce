#include "framework.h"
#include "Scene.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace TerrainBruteForceApp;

//////////////////////////////////////////////////////////////////////////


Scene::Scene(GLfloat terrainScaleFactor)
	: m_terrain(terrainScaleFactor)
{
}

Scene::~Scene()
{
}

bool Scene::initialize(const OpenGLInfo& openGlInfo)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_MULTISAMPLE);

	glClearColor(0.8f, 0.93f, 0.96f, 1.0f);    // very light blue

	// Initial scale factor for the camera.
	const GLfloat CameraScaleFactor = 0.02f;    // 1.0f

	glm::vec3 cameraPosition = { 0.0f, 0.0f, -5.0f };

	m_spCamera = std::make_unique<Camera>(cameraPosition, CameraScaleFactor,
		openGlInfo.FieldOfView, openGlInfo.FrustumNear, openGlInfo.FrustumFar);

	// Get our future terrain in focus.
	m_spCamera->translate({ -0.8f, -0.8f, -1.5f });

	if (!m_terrain.initialize())
	{
		std::wcerr << L"Terrain initialization failed\n";
		assert(false); return false;
	}

	updateViewMatrices();

	return true;
}

void Scene::updateViewMatrices() const
{
	m_terrain.updateViewMatrices(m_spCamera);
}

void Scene::translateCamera(const glm::vec3& diff)
{
	m_spCamera->translate(diff);

	updateViewMatrices();
}

void Scene::rotateCamera(const glm::vec3& degrees)
{
	m_spCamera->rotate(degrees);

	updateViewMatrices();
}

GLfloat Scene::getCameraScale() const
{
	return m_spCamera->getScale();
}

void Scene::scaleCamera(GLfloat amount)
{
	m_spCamera->scale(amount);

	updateViewMatrices();
}

void Scene::resize(GLfloat aspectRatio)
{
	if (m_spCamera)
	{
		m_spCamera->resize(aspectRatio);

		updateViewMatrices();
	}
}

void Scene::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_terrain.render();
}
