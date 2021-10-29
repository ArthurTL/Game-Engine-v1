#include <glad/glad.h>
#include "Player.h"
#include "MasterRenderer.h"
#include "OBJ_Loader.h"
#include "GuiRenderer.h"
#include "WaterRenderer2.h"
#include "WaterShader.h"
#include "GuiShader.h"
#include <stdio.h>
#include <random>
using namespace std;

GLFWwindow* window = NULL;
Camera* camera = NULL;
Player* player = NULL;
Loader loader = Loader();
objl::Loader objLoader;

double previous_x_position = 0.;
double previous_y_position = 0.;

std::vector<Entity*> entities;
std::vector<Entity*> normalEntities;
std::vector<Terrain*>terrains;
std::vector<Light*> lights;


void initOpenGL() {
	// Load extensions for modern OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "ERROR: Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/*CAMERA CONTROL
	if (key == GLFW_KEY_LEFT_SHIFT && action==GLFW_PRESS ) {
		camera->setMoveDown(true);
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
		camera->setMoveDown(false);
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		camera->setMoveUp(true);
	}
	if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
		camera->setMoveUp(false);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		camera->setMoveRight(true);
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		camera->setMoveRight(false);
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		camera->setMoveLeft(true);
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		camera->setMoveLeft(false);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		camera->setMoveForward(true);
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		camera->setMoveForward(false);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		camera->setMoveBack(true);
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		camera->setMoveBack(false);
	}*/
	player->checkInputs(key, action);
	
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->calculateZoom(yoffset);
}
void  mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &previous_x_position, &previous_y_position);
		;
	}
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		camera->setPitch(glm::max(2.,camera->getPitch() - (previous_y_position - ypos)*0.5));
		camera->setAngleAroundPlayer(camera->getAngleAroundPlayer() + (previous_x_position - xpos) * 0.5);
		glfwGetCursorPos(window, &previous_x_position, &previous_y_position);
	}
	/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
	}*/
}
void windowSizeCallback(GLFWwindow* window2, int width, int height) {
	camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
	WindowManager::HEIGHT = height;
	WindowManager::WIDTH = width;
	camera->setAspectRatio(width / height);
	glViewport(0, 0, (GLint)width, (GLint)height); // Dimension of the rendering region in the window
}

std::vector<TexturedModel*> importOBJ(const string& objName, const string& texture) {
	bool loadout = objLoader.LoadFile(objName);
	if (loadout)
	{
		vector<TexturedModel*> meshes;
		for (int i = 0; i < objLoader.LoadedMeshes.size(); i++) {
			objl::Mesh curMesh = objLoader.LoadedMeshes[i];
			vector<float> positions;
			vector<float> textureCoords;
			vector<float> normals;
			for (int j = 0; j < curMesh.Vertices.size(); j++) {
				objl::Vertex vertex = curMesh.Vertices[j];
				positions.push_back(vertex.Position.X);
				positions.push_back(vertex.Position.Y);
				positions.push_back(vertex.Position.Z);
				textureCoords.push_back(vertex.TextureCoordinate.X);
				textureCoords.push_back(vertex.TextureCoordinate.Y);
				normals.push_back(vertex.Normal.X);
				normals.push_back(vertex.Normal.Y);
				normals.push_back(vertex.Normal.Z);
			}
			RawModel* mesh = loader.loadToVAO(positions, textureCoords, normals, curMesh.Indices);
			std::cout << curMesh.Indices.size();
		
			if (texture == "none") {
				ModelTexture tex = ModelTexture(loader.loadTexture(curMesh.MeshMaterial.map_Kd));
				objl::Vector3 Kd = curMesh.MeshMaterial.Kd;
				tex.setAmbiantColor(vec3(Kd.X, Kd.Y, Kd.Z));
				tex.setReflectivity(0.5);
				tex.setShineDamper(curMesh.MeshMaterial.Ns);

				TexturedModel* texturedMesh = new TexturedModel(*mesh, tex);
				meshes.push_back(texturedMesh);
			}
			else {
				ModelTexture tex = ModelTexture(loader.loadTexture(texture));
				TexturedModel* texturedMesh = new TexturedModel(*mesh, tex);
				meshes.push_back(texturedMesh);
			}
			
		}
		return meshes;
	}
	return {};
}
TexturedModel* createTexturedModel(const string& objName, const string& texture, float reflectivity = 0., float shineDamper = 30., int nbRows=1,  bool transparency=false, bool fakeLighting=false) {
	RawModel* model = OBJLoader::loadObjModel(objName, loader,false);
	ModelTexture modelTexture = ModelTexture(loader.loadTexture(texture));
	modelTexture.setNumberOfRows(nbRows);
	modelTexture.setReflectivity(reflectivity);
	modelTexture.setShineDamper(shineDamper);
	modelTexture.setHasTransparency(transparency);
	modelTexture.setUseFakeLighting(fakeLighting);
	return new TexturedModel(*model, modelTexture);
}
TexturedModel* createNormalMapModel(const string& objName, const string& texture, const string& normalMap, float reflectivity = 0., float shineDamper = 30.) {
	RawModel* model = OBJLoader::loadObjModel(objName, loader,true);
	ModelTexture modelTexture = ModelTexture(loader.loadTexture(texture));
	modelTexture.setReflectivity(reflectivity);
	modelTexture.setShineDamper(shineDamper);
	TexturedModel *texturedModel = new TexturedModel(*model, modelTexture);
	texturedModel->texture.setNormalMap(loader.loadTexture(normalMap));
	return texturedModel;
}
std::vector<Entity*> createEntity(std::vector<TexturedModel*> meshes, glm::vec3 pos, float rx, float ry, float rz, float scale) {
	std::vector<Entity*> assembledMesh;
	for (TexturedModel* mesh : meshes) {
		Entity* entity = new  Entity(mesh, pos, rx, ry, rz, scale);
		assembledMesh.push_back(entity);
	}
	return assembledMesh;
}
glm::vec3 pos(float x, float z, float yoffset=0.0) { return glm::vec3(x, terrains[Terrain::determineTerrain(x, z)]->getHeightOfTerrain(x, z)+yoffset, z); }
int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	window = WindowManager::createWindow();
	initOpenGL();


	TexturedModel* playerPointer = createTexturedModel("fox.obj", "fox_texture.png",0.0,30);
	
	
	player = new Player(playerPointer, glm::vec3(80, 0, -20), 0, 180, 0, 0.07);
	entities.push_back(player);

	camera = new Camera(player);

	MasterRenderer* renderer = new MasterRenderer(loader,camera);


	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);


	//CREATING TERRAINS
	TerrainTexture backgroundTexture = TerrainTexture(loader.loadTexture("grass_texture_medium.png"));
	TerrainTexture rTexture = TerrainTexture(loader.loadTexture("dirt.png"));
	TerrainTexture gTexture = TerrainTexture(loader.loadTexture("wildGrass2.png"));
	TerrainTexture bTexture = TerrainTexture(loader.loadTexture("path2.png"));
	TerrainTexture* blendMap = new TerrainTexture(loader.loadTexture("blendMapr.png"));
	TerrainTexturePack* texturePack = new  TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);

	Terrain* terrain = new Terrain(0, -1, loader, texturePack, blendMap, "heightmapr.png");
	terrains.push_back(terrain);
	Terrain* terrain2 = new Terrain(-1, -1, loader, texturePack, blendMap, "heightmap.png");
	terrains.push_back(terrain2);
	Terrain* terrain3 = new Terrain(0, 0, loader, texturePack, blendMap, "heightmap.png");
	terrains.push_back(terrain3);
	Terrain* terrain4 = new Terrain(-1, 0, loader, texturePack, blendMap, "heightmap.png");
	terrains.push_back(terrain4);

	/*//SHREK
	std::vector<TexturedModel*> shrekTex = importOBJ("shrek.obj","none");
	std::vector<Entity*> shrekParts = createEntity(shrekTex, vec3(80, 230, -900), 0, 0, 0, 100000);
	for (Entity* part : shrekParts) {entities.push_back(part);}
	*/

	//CREATING ENTITIES

	TexturedModel* modelPointer = createTexturedModel( "dragon.obj", "blank.png", 0.75, 60);
	
	Entity* dragon = new  Entity(modelPointer, pos(70,-50), 0.f, 0.f, 0.f, 2.f);
	entities.push_back(dragon);

	
	//CREATING TREES
	TexturedModel* treePointer = createTexturedModel("pine.obj", "pine.png");
	TexturedModel* tree2Pointer = createTexturedModel("lowPolyTree.obj", "tree2.png");

	for (int i = 0; i < 200; i++) {
		float x = fmod(rand(), 1000.) - 500.;
		float z = fmod(rand(), 1000.) - 500.;
		float y = terrains[Terrain::determineTerrain(x, z)]->getHeightOfTerrain(x, z);
		if (!(x > 40 && x < 170 && z > -270 && z < -140)&& !(x > 75 && x < 85 && z > -30 && z < -15)) {
			Entity* tree = new  Entity(treePointer, glm::vec3(x, y, z), 0, 0, 0, rand() % 4 + 8.);
			entities.push_back(tree);
		}
		/*if (i % 2 == 0) {
			float x2 = fmod(rand(), 1000.f) - 500.;
			float z2 = fmod(rand(), 1000.) - 500.;
			float y2 = terrains[Terrain::determineTerrain(x, z)]->getHeightOfTerrain(x2, z2);
			Entity* tree2 = new  Entity(tree2Pointer, glm::vec3(x2, y2, z2), 0, (float)(rand() % 90 - 45), 0, rand()%2+2.);
			entities.push_back(tree2);
		}*/
	};
	
	//CREATING GRASS
	
	//TexturedModel* grassTexModel = createTexturedModel("grassModel.obj", "grassAtlas.png",0.,30.,4,true,true);
	
	TexturedModel* fernTexModel = createTexturedModel("fern.obj", "fernAtlas.png", 0., 30., 2, true,false);
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			/*float x = i * 40. + fmod(rand(), 30) - 415.;
			float z = j * 40. + fmod(rand(), 30) - 415.;
			float y = terrains[Terrain::determineTerrain(x, z)]->getHeightOfTerrain(x, z);
			Entity* grass = new  Entity(grassTexModel, rand() % 9, glm::vec3(x, y, z), 0, (float)(rand() % 90 - 45), 0, 4.);
			entities.push_back(grass);*/
			float x2 = i * 40. + fmod(rand(),30) - 120.;
			float z2 = j * 40. + fmod(rand(), 30) - 220.;
			float y2 = terrains[Terrain::determineTerrain(x2, z2)]->getHeightOfTerrain(x2, z2);
			if (!(x2 > 40 && x2 < 170 && z2 > -270 && z2 < -140)) {
				Entity* fern = new  Entity(fernTexModel, rand() % 4, glm::vec3(x2, y2, z2), 0, (float)(rand() % 90 - 45), 0, 1.3 + (rand() % 5) / 10);
				entities.push_back(fern);
			}
			
		}
	};
	//ROCKS AND BARREL

	TexturedModel* barrelModel = createNormalMapModel("barrel.obj", "barrel.png","barrelNormal.png", 0.5, 50.);
	Entity* barrel= new  Entity(barrelModel,pos(80.,-95.,6.), 0, 0, 0, 1);
	normalEntities.push_back(barrel);
	TexturedModel* boulderModel = createNormalMapModel("boulder.obj", "boulder.png","boulderNormal.png", 0.3, 50.);
	Entity* boulder = new  Entity(boulderModel, pos(120., -45.), 10, 10, 0, 1.2);
	normalEntities.push_back(boulder);
	Entity* boulder2 = new  Entity(boulderModel, pos(115., -35.), 0, 10, -60, 0.8);
	normalEntities.push_back(boulder2);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			float x = i * 80. + fmod(rand(), 60) - 430.;
			float z = j * 80. + fmod(rand(), 60) - 430.;
			float y = terrains[Terrain::determineTerrain(x, z)]->getHeightOfTerrain(x, z);
			Entity* boulderRand = new  Entity(boulderModel, glm::vec3(x, y, z), (float)(rand() % 160 - 80), (float)(rand() % 160 - 80), (float)(rand() % 160 - 80), 0.4 + (rand() % 16) / 10);
			normalEntities.push_back(boulderRand);
		
				
		}
	};



	//LAMP
	std::vector<TexturedModel*> lampTex = importOBJ("lamp2.obj","lamp2_texture.png");
	std::vector<Entity*> lamps = createEntity(lampTex, glm::vec3(78, -3, -85), 0, 45, 0, 12);
	for (Entity* part : lamps) {entities.push_back(part);}
	std::vector<Entity*> lamps2 = createEntity(lampTex, glm::vec3(175, terrain->getHeightOfTerrain(175, -120), -120), 0, 45, 0, 12);
	for (Entity* part : lamps2) { entities.push_back(part); }
	std::vector<Entity*> lamps3 = createEntity(lampTex, glm::vec3(68, 0, 30), 0, 45, 0, 12);
	for (Entity* part : lamps3) { entities.push_back(part); }
	std::vector<Entity*> lamps4 = createEntity(lampTex, glm::vec3(190, terrain->getHeightOfTerrain(190, -190), -190), 0, 60, 0, 12);
	for (Entity* part : lamps4) { entities.push_back(part); }
	std::vector<Entity*> lamps5 = createEntity(lampTex, pos(25,-265), 0, 45, 0, 12);
	for (Entity* part : lamps5) { entities.push_back(part); }
	
	/*
	std::vector<TexturedModel*> aimModel = importOBJ("fox.obj", "fox_texture.png");
	std::vector<Entity*> aim = createEntity(aimModel, glm::vec3(85, 14, -77.2), 0, 45, 0, 0.01);
	*/
	Light* sun = new Light(vec3(100000.f, 350000.f, 100000.f), vec3(255. / 256., 225. / 256., 140. / 256.));
	lights.push_back(sun);
	Light * lampLight = new Light(vec3(85, 14, -77.2), vec3(255. / 256., 160. / 256., 8. / 256.)*1.2f,vec3(1.,0.002f,0.00025f));
	lights.push_back(lampLight);
	Light *lampLight2 = new Light(vec3(182, terrain->getHeightOfTerrain(175, -120)+17, -112.2), vec3(255. / 256., 160. / 256., 8. / 256.)*1.2f, vec3(1., 0.002f, 0.00025f));
	lights.push_back(lampLight2);
	Light *lampLight3 = new Light(pos(197,-182.8,17.), vec3(255. / 256., 160. / 256., 8. / 256.)*1.2f, vec3(1., 0.002f, 0.00025f));
	lights.push_back(lampLight3);
	Light* lampLight4 = new Light(pos(32, -257.2, 17.), vec3(255. / 256., 160. / 256., 8. / 256.) * 1.2f, vec3(1., 0.002f, 0.00025f));
	lights.push_back(lampLight4);
	
	

	//**************************Water************************
	WaterShader* waterShader = new WaterShader();
	WaterFrameBuffers* fbos = new WaterFrameBuffers();
	WaterRenderer waterRenderer = WaterRenderer(loader, waterShader, *camera,fbos);
	std::vector<WaterTile> waters = { WaterTile(102,-220,-4.5),WaterTile(40,-80,-4.5) };

	
	//GUIS
	std::vector<GuiTexture> guis{};
	GuiTexture gui = GuiTexture(loader.loadTexture("health.png"), vec2(-0.75, -0.75), vec2(0.15, 0.25));
	//guis.push_back(gui);
	GuiTexture instr = GuiTexture(loader.loadTexture("instructions.png"), vec2(0.8, 0.77), vec2(0.15, 0.25));
	guis.push_back(instr);
	GuiTexture shadowMap = GuiTexture(renderer->getShadowMapTexture(), vec2(-0.5, 0.5), vec2(0.25, 0.25));
	//guis.push_back(shadowMap);
	GuiRenderer* guiRenderer = new GuiRenderer(loader);

	

	while (!glfwWindowShouldClose(window)) {
		
		
		camera->move();
		player->move(*(terrains[Terrain::determineTerrain(player->position.x,player->position.z)]));
		float time = renderer->getTime();
		sun->updateSun(time);
		for (int i = 1; i<5; i++) {
			lights[i]->updateLamp(time);
		}
		dragon->increaseRotation(0, 1, 0);
		
		renderer->renderShadowMap(entities,normalEntities, sun,camera);
		glEnable(GL_CLIP_DISTANCE0);

		fbos->bindReflectionFrameBuffer();
		float distance = 2 * (camera->getPosition().y - waters[0].getHeight());
		camera->setPosition(camera->getPosition() - vec3(0, distance, 0));
		camera->invertPitch();
		renderer->renderScene(entities, normalEntities, terrains, lights, camera, vec4(0,1,0,-waters[0].getHeight()+0.5));
		camera->setPosition(camera->getPosition() + vec3(0, distance, 0));
		camera->invertPitch();

		fbos->bindRefractionFrameBuffer();
		renderer->renderScene(entities, normalEntities, terrains, lights, camera, vec4(0, -1, 0, waters[0].getHeight()));
		
		glDisable(GL_CLIP_DISTANCE0);
		fbos->unbindCurrentFrameBuffer();
		renderer->renderScene(entities,normalEntities, terrains, lights, camera, vec4(0.0));
		waterRenderer.render(waters, *camera,lights);
		guiRenderer->render(guis);
		
		WindowManager::updateWindow(window);

	}
	fbos->cleanUp();
	waterShader->cleanUp();
	renderer->cleanUp();
	guiRenderer->cleanUp();
	
	delete renderer;
	renderer = 0;
	delete guiRenderer;
	guiRenderer = 0;
	delete fbos;
	fbos = 0;
	delete camera;
	camera = 0;
	

	for (Light* light : lights) {
		delete light;
		light = 0;
	};
	
	for (Entity* entity : entities) {
		delete entity;
		entity = 0;
	};
	
	for (Terrain* terrain: terrains) {
		delete terrain;
		terrain = 0;
	};
	
	loader.cleanUp();
	
	WindowManager::closeWindow(window);
}

