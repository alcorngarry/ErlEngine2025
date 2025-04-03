#include"Map.h"

Map::Map(std::string mapName) : loadState(KEEP_MAP), state(DEFAULT)
{
	fileName = mapName;
}

void Map::save()
{
	writeMap = std::ofstream{ fileName + ".esf" };

	write_models();
	write_lights();

	writeMap.close();
}

void Map::write_models()
{
	writeMap << "models: [";
	for (int i = 0; i < entities.size(); i++)
	{
		writeMap << "{";
		//don't save ball location
		if (entities[i]->assetId != 5)
		{
			writeMap << "assetId: " << entities[i]->assetId << ", " << "position: "
				<< entities[i]->Position.x << "," << entities[i]->Position.y << "," << entities[i]->Position.z << ", "
				<< "scale: " << entities[i]->Size.x << "," << entities[i]->Size.y << "," << entities[i]->Size.z << ", "
				<< "rotation: " << entities[i]->Rotation.x << "," << entities[i]->Rotation.y << "," << entities[i]->Rotation.z << ", " << "isRendered: " << (entities[i]->isRendered == 1);
		}
		if (i == entities.size() - 1)
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
	for (int i = 0; i < lights.size(); i++)
	{
		writeMap << "{";
		writeMap << "assetId: " << lights[i]->assetId << ", " << "position: "
			<< lights[i]->Position.x << "," << lights[i]->Position.y << "," << lights[i]->Position.z << ", "
			<< "scale: " << lights[i]->Size.x << "," << lights[i]->Size.y << "," << lights[i]->Size.z << ", "
			<< "rotation: " << lights[i]->Rotation.x << "," << lights[i]->Rotation.y << "," << lights[i]->Rotation.z;
		if (i == lights.size() - 1)
		{
			writeMap << "}";
		}
		else {
			writeMap << "}," << std::endl;
		}
	}
	writeMap << "]";
}

void Map::load(float windowWidth, float windowHeight)
{
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
		}
		else if (line == "boardSpaces: ")
		{
			read_board_spaces();
		}
	} 

	load_camera(windowWidth, windowHeight);
	readMap.close();
	load_skybox();
}

void Map::load_camera(float windowWidth, float windowHeight)
{
	camera = new Camera(windowWidth, windowHeight);
}

void Map::read_models()
{
	while (readMap.peek() != ']')
	{
		GameObject* entity = read_asset();
		entities.push_back(entity);
		if(entity->isRendered) Renderer::add_render_object(entity);
	}
	getline(readMap, line, '\n');
}

void Map::read_lights()
{
	while (readMap.peek() != ']')
	{
		GameObject* light = read_asset();
		lights.push_back(light);
		Renderer::add_light_render_object(light);
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

	//asset
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
	getline(readMap, line, '}');
	isRendered = std::stof(line) == 1;
	
	return new GameObject(assetId, AssetManager::get_model(assetId), position, scale, rotation, isRendered);
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

	entities.push_back(model);
	Renderer::add_render_object(model);
}

void Map::remove_model(int selectedIndex)
{
	entities.erase(entities.begin() + selectedIndex);
	Renderer::remove_render_object(selectedIndex);
}

void Map::toggle_render(int index)
{
	bool isRendered = entities[index]->isRendered;
	if (isRendered)
	{
		entities[index]->isRendered = false;
		Renderer::remove_render_object(index);
	}
	else {
		entities[index]->isRendered = true;
		Renderer::add_render_object(entities[index]);
	}
}