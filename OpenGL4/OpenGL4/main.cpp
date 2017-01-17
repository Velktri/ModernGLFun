#include <chrono>
#include "Shader.h"
#include "Camera.h"
#include "Models/Grid.h"
#include "Models/Pyramid.h"
#include "Lights/SpotLight.h"
#include "Lights/SunLight.h"
#include "Lights/PointLight.h"
#include "Texture.h"

/* Constants */
static const GLuint WIDTH = 1920;
static const GLuint HEIGHT = 1080;
static const int GRIDRADIUS_X = 10;
static const int GRIDRADIUS_Y = 10;
static const float GRIDSPACING = 1.0;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;

/* Prototypes */


int main(int argc, char *argv[]) {
	auto t_start = std::chrono::high_resolution_clock::now();

	/* SDL Init */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow("GeoViewer", /*760*/150, 180, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	/*Build Camera */
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 1000.0f);
	Camera* worldCamera = new Camera(glm::vec3(0.0f, 10.0f, 20.0f));

	/* Build Shaders */
	Shader* SceneShader = new Shader("assets/Shaders/Scene.vert", "assets/Shaders/Scene.frag");
	Shader* LampShader = new Shader("assets/Shaders/Lamp.vert", "assets/Shaders/Lamp.frag");
	Shader* LightingShader = new Shader("assets/Shaders/Lighting.vert", "assets/Shaders/Lighting.frag");

	/* Build scene elements */
	Grid* Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);

	/* Build Textures */
	Texture* ModelDiffuse = new Texture("../../OpenGL4/assets/Textures/WoodenBox_diff.png");
	Texture* ModelSpecular = new Texture("../../OpenGL4/assets/Textures/WoodenBox_spec.png");

	/* Build Models */
	std::vector<Model*> modelList;
	for (int i = 0; i < 5; i++) {
		Model* data = new Model();
		//data->GenerateModel();
		data->GenerateArrayModel();
		data->TranslateModel(i, 0, i);

		data->AttachDiffuseTexture(ModelDiffuse);
		data->AttachSpecularTexture(ModelSpecular);

		modelList.push_back(data);
	}

	/* Build Lights */
	std::vector<Light*> LightList;
	Light* ALight = new Light(glm::vec3(-2.0f, 2.0f, 2.0f));
	LightList.push_back(ALight);

	/* Main Loop */
	SDL_Event windowEvent;
	while (true) {
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		deltaTime = time - lastFrame;
		lastFrame = time;

		ALight->Translate(sin(time) / 20.0f, 0, cos(time) / 20.0f);

#pragma region Inputs
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}

			if (windowEvent.type == SDL_MOUSEWHEEL) {
				worldCamera->ZoomCamera(windowEvent, deltaTime);
			}

			if (windowEvent.type == SDL_KEYDOWN) {
				worldCamera->processKeyEvents(windowEvent.key.keysym.sym, deltaTime);
			}

			if (windowEvent.type == SDL_MOUSEMOTION || windowEvent.type == SDL_MOUSEBUTTONDOWN || windowEvent.type == SDL_MOUSEBUTTONUP) {
				worldCamera->ProcessMouseEvents(windowEvent, deltaTime);
			}		
		}
#pragma endregion

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render Scene */
		SceneShader->Use();
		glUniformMatrix4fv(SceneShader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(Scene->orientation));
		glUniformMatrix4fv(SceneShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(worldCamera->GetViewMatrix()));
		glUniformMatrix4fv(SceneShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(projection));
		Scene->Draw();


		/* Render Models */
		LightingShader->Use();
		glUniformMatrix4fv(LightingShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(worldCamera->GetViewMatrix()));
		glUniformMatrix4fv(LightingShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(LightingShader->ShaderList["viewPos"], worldCamera->GetPosition().x, worldCamera->GetPosition().y, worldCamera->GetPosition().z);
		glUniform3f(LightingShader->ShaderList["dirLight.position"], 0.0f, 0.0f, 0.0f);
		glUniform3f(LightingShader->ShaderList["dirLight.ambient"], 0.05f, 0.05f, 0.05f);
		glUniform3f(LightingShader->ShaderList["dirLight.diffuse"], 1.0f, 1.0f, 1.0f);
		glUniform3f(LightingShader->ShaderList["dirLight.specular"], 0.5f, 0.5f, 0.5f);
		glUniform1i(LightingShader->ShaderList["material.diffuse"], 0);
		glUniform1i(LightingShader->ShaderList["material.specular"], 1);
		glUniform1f(LightingShader->ShaderList["material.shininess"], 32.0f);

		glUniform3f(LightingShader->ShaderList["pointLight.position"], LightList[0]->WorldPosition.x, LightList[0]->WorldPosition.y, LightList[0]->WorldPosition.z);
		glUniform3f(LightingShader->ShaderList["pointLight.ambient"], 0.05f, 0.05f, 0.05f);
		glUniform3f(LightingShader->ShaderList["pointLight.diffuse"], 0.8f, 0.8f, 0.8f);
		glUniform3f(LightingShader->ShaderList["pointLight.specular"], 1.0f, 1.0f, 1.0f);
		glUniform1f(LightingShader->ShaderList["pointLight.constant"], 1.0f);
		glUniform1f(LightingShader->ShaderList["pointLight.linear"], 0.09);
		glUniform1f(LightingShader->ShaderList["pointLight.quadratic"], 0.032);
		glUniform1i(LightingShader->ShaderList["lightAmount"], LightList.size());

		for each (Model* mod in modelList) {
			glUniformMatrix4fv(LightingShader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(mod->orientation));
			mod->Draw();
		}

		/* Render Lights */
		LampShader->Use();
		glUniformMatrix4fv(LampShader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(worldCamera->GetViewMatrix()));
		glUniformMatrix4fv(LampShader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(projection));

		for each (Light* li in LightList) {
			glUniformMatrix4fv(LampShader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(li->GetOrientation()));
			glBindVertexArray(li->GetVAO());
			glDrawElements(GL_TRIANGLES, li->indicesSize, GL_UNSIGNED_INT, 0);
		}


		SDL_GL_SwapWindow(window);
	}

#pragma region Clean Up
	/* Clean up */
	for each (Model* mod in modelList) {
		mod->~Model();
	}

	//TransformShader->~Shader();
	Scene->~Grid();
	LampShader->~Shader();
	LightingShader->~Shader();
	

	SDL_GL_DeleteContext(context);
	SDL_Quit();
#pragma endregion 

	return 0;
}