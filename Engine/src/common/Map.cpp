#include"Map.h"

std::map<std::string, std::function<void(GameObject*, float)>> actions;

void Scripts::init()
{
	actions["rotate"] = Scripts::rotate;
	actions["hide"] = Scripts::hide;
	actions["show"] = Scripts::show;
}

void Scripts::rotate(GameObject* gameObject, float deltaTime)
{
	gameObject->ModelMatrix = glm::rotate(gameObject->ModelMatrix, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Scripts::hide(GameObject* gameObject, float deltaTime)
{
	gameObject->isRendered = false;
	Renderer::remove_render_object(gameObject->instanceId);
}

void Scripts::show(GameObject* gameObject, float deltaTime)
{
	gameObject->isRendered = true;
	Renderer::add_render_object(gameObject);
}

std::function<void(GameObject*, float)> Scripts::get_script(std::string scriptName)
{
	return actions[scriptName];
}

Map::Map(std::string mapName) : loadState(KEEP_MAP), state(DEFAULT), renderState(PLAYER1)
{
	fileName = mapName;
	Scripts::init();
}

void Map::save()
{
	writeMap = std::ofstream{ fileName + ".esf" };
	write_models();
	write_lights();
	write_player();
	writeMap.close();
}

void Map::write_models()
{
	writeMap << "models: [";
	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		writeMap << "{";

		writeMap << "assetId: " << entity->second->assetId << ", " << "position: "
			<< entity->second->Position.x << "," << entity->second->Position.y << "," << entity->second->Position.z << ", "
			<< "scale: " << entity->second->Size.x << "," << entity->second->Size.y << "," << entity->second->Size.z << ", "
			<< "rotation: " << entity->second->Rotation.x << "," << entity->second->Rotation.y << "," << entity->second->Rotation.z << ", " << "isRendered: " << (entity->second->isRendered == 1) << ", ";
		write_scripts(entity->second);

		if (entity == std::prev(entities.end()))
		{
			writeMap << "}";
		}
		else {
			writeMap << "}," << std::endl;
		}
	}
	writeMap << "]" << std::endl;
}

void Map::write_lights()
{
	writeMap << "lights: [";
	for (auto light = lights.begin(); light != lights.end(); ++light)
	{
		writeMap << "{";
		writeMap << "assetId: " << light->second->assetId << ", " << "position: "
			<< light->second->Position.x << "," << light->second->Position.y << "," << light->second->Position.z << ", "
			<< "scale: " << light->second->Size.x << "," << light->second->Size.y << "," << light->second->Size.z << ", "
			<< "rotation: " << light->second->Rotation.x << "," << light->second->Rotation.y << "," << light->second->Rotation.z;
		
		if (light == std::prev(lights.end()))
		{
			writeMap << "}";
		}
		else {
			writeMap << "}," << std::endl;
		}
	}
	writeMap << "]" << std::endl;
}

void Map::write_scripts(GameObject* entity)
{
	writeMap << "scripts: [";
	if (entity->actions.size() != 0)
	{
		writeMap << "{";
		for(auto action = entity->actions.begin(); action != entity->actions.end(); ++action)
		{
			if (action == std::prev(entity->actions.end()))
			{
				writeMap << action->first;
			} else {
				writeMap << action->first << ", ";
			}
		}
		writeMap << "}";
	}
	writeMap << "]";
}

void Map::write_player()
{
	writeMap << "player: [";
	for (int i = 0; i < playerStarts.size(); i++)
	{
		writeMap << "{";
		writeMap << "position: " << playerStarts[i].x << "," << playerStarts[i].y << "," << playerStarts[i].z;
		writeMap << "}";

		if (!(i + 1 == playerStarts.size()))
		{
			writeMap << "," << std::endl;
		}
	}
	writeMap << "]" << std::endl;
}

void Map::load(float windowWidth, float windowHeight)
{
	load_camera(windowWidth, windowHeight);

	readMap.open(fileName + ".esf");
	char peek = 'B';
	std::cout << "Loading Map: " << fileName << std::endl;

	while (getline(readMap, line, '['))
	{
		if (line == "models: ")
		{
			read_models();
		} else if (line == "lights: ") {
			read_lights();
		} else if (line == "player: ") {
			read_player();
		}
	} 
	readMap.close();
	load_skybox();
	load_physics_objects();
}

void Map::load_camera(float windowWidth, float windowHeight)
{
	camera = new Camera(windowWidth, windowHeight);
}

void Map::load_physics_objects()
{
	for (const auto& entity : entities)
	{
		ErlPhysics::add_physics_object(entity.second);
	}
}

void Map::read_models()
{
	std::cout << "Reading models" << std::endl;
	while (readMap.peek() != ']')
	{
		GameObject* entity = read_asset();
		entities[entity->instanceId] = (entity);
		if(entity->isRendered) Renderer::add_render_object(entity);
	}
	getline(readMap, line, '\n');
}

void Map::read_lights()
{
	std::cout << "Reading lights" << std::endl;
	while (readMap.peek() != ']')
	{
		GameObject* light = read_asset();
		lights[light->instanceId] = (light);
		Renderer::add_light_render_object(light);
	}
	getline(readMap, line, '\n');
}

void Map::read_player()
{
	std::cout << "Reading player(s)" << std::endl;
	while (readMap.peek() != ']')
	{
		Player* player = read_player_asset();
		players.push_back(player);
		Renderer::add_render_object(player);
		ErlPhysics::add_player_physics_object(player);
	}
	getline(readMap, line, '\n');
}


GameObject* Map::read_asset()
{
	std::string line;
	getline(readMap, line, ':');
	uint8_t assetId;
	float x, y, z;
	glm::vec3 position, scale, rotation;
	bool isRendered;

	getline(readMap, line, ',');
	assetId = std::stof(line);

	getline(readMap, line, ':');

	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, ',');
	z = std::stof(line);
	position = { x, y, z };

	getline(readMap, line, ':');

	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, ',');
	z = std::stof(line);
	scale = { x, y, z };

	getline(readMap, line, ':');

	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, ',');
	z = std::stof(line);
	rotation = { x, y, z };

	getline(readMap, line, ':');
	getline(readMap, line, ',');
	isRendered = std::stof(line) == 1;

	GameObject* entity = new GameObject(assetId, AssetManager::get_model(assetId), position, scale, rotation, isRendered);
	read_script(entity);
	return entity;
}

void Map::read_script(GameObject* entity)
{
	std::string line;
	std::string scriptName;

	getline(readMap, line, '[');
	while (readMap.peek() != ']')
	{
		getline(readMap, line, ':');
		scriptName = line.substr(1, line.size());
		entity->actions[scriptName] = actions[scriptName];
	}
	getline(readMap, line, '}');
}

Player* Map::read_player_asset()
{
	std::string line;
	glm::vec3 position;
	float x, y, z;

	getline(readMap, line, ':');
	getline(readMap, line, ',');
	x = std::stof(line);
	getline(readMap, line, ',');
	y = std::stof(line);
	getline(readMap, line, '}');
	z = std::stof(line);
	position = { x, y, z };

	// glm::vec3(95.0f, 60.0f, 1045.0f)
	// glm::vec3(115.0f, 60.0f, 1045.0f)
	playerStarts.push_back(position);
	return new Player(players.size(), AssetManager::get_model(0), camera, position);
}

void Map::load_skybox()
{
	Renderer::add_sky_box(AssetManager::get_sky_box());
}

void Map::draw(float deltaTime)
{
	Renderer::render(camera);
}

void Map::duplicate_model(int selectedIndex)
{
	GameObject* model = new GameObject(entities[selectedIndex]->assetId,
		entities[selectedIndex]->GameModel, entities[selectedIndex]->Position, entities[selectedIndex]->Size,
		entities[selectedIndex]->Rotation, entities[selectedIndex]->isRendered);
	model->actions = entities[selectedIndex]->actions;

	entities[model->instanceId] = model;
	Renderer::add_render_object(model);
	ErlPhysics::add_physics_object(model);
	
}

void Map::remove_model(int selectedIndex)
{
	entities.erase(selectedIndex);
	Renderer::remove_render_object(selectedIndex);
}

void Map::toggle_render(int index)
{
	bool isRendered = entities[index]->isRendered;
	if (isRendered)
	{
		entities[index]->isRendered = false;
		Renderer::remove_render_object(entities[index]->instanceId);
	}
	else {
		entities[index]->isRendered = true;
		Renderer::add_render_object(entities[index]);
	}
}

void Map::set_controls()
{
	ToggleConsoleCommand tc;
	InputManager::set_key_binding(GLFW_KEY_GRAVE_ACCENT, [tc](float dt) mutable { tc.execute(dt); });
}

void Map::clear_controls()
{
	InputManager::remove_key_binding(GLFW_KEY_GRAVE_ACCENT);
}