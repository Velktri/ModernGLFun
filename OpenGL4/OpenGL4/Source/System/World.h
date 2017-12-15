#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Camera;
class Manager;
class Light;
class Universe;

class World {
public:
	World(Universe* InUniverse, Manager* InManager);
	~World();

	void RenderWorld(glm::vec3 InCameraPosition, glm::mat4 InView, glm::mat4 InProjection, glm::vec2 FrameSize);
	void RenderColorWorld(Camera* InCamera, glm::vec2 FrameSize);

	std::vector<Light*> GetLights();
	Manager* GetManager();

private:
	Manager* MyManager;
	Universe* OwningUniverse;
};

