#include"Map.h"

Map::Map(std::string mapName) : loadState(KEEP_MAP), state(DEFAULT)
{
	fileName = mapName;
	//could be moved elsewhere don't see why at the moment
	camera = new Camera();
}

void Map::save()
{
	//probably fix this in the future, and add lights and skins
	entities = Renderer::get_rendered_entities();

	writeMap = std::ofstream{ fileName + ".esf" };
	
	writeMap << "models: [";
	for (int i = 0; i < entities.size(); i++)
	{
		writeMap << "{";
		//don't save ball location
		if (entities[i]->id != 5)
		{
			writeMap << "assetId: " << entities[i]->id << ", " << "position: " 
				<< entities[i]->Position.x << "," << entities[i]->Position.y << "," << entities[i]->Position.z << ", "
				<< "scale: " << entities[i]->Size.x << "," << entities[i]->Size.y << "," << entities[i]->Size.z << ", "
				<< "rotation: " << entities[i]->Rotation.x << "," << entities[i]->Rotation.y << "," << entities[i]->Rotation.z;
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

	writeMap << "lights: [";
	for (int i = 0; i < lights.size(); i++)
	{
		writeMap << "{";
		writeMap << "assetId: " << lights[i]->id << ", " << "position: "
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

	writeMap.close();
}

void Map::load()
{
	readMap.open(fileName + ".esf");
	std::string line;
	uint8_t id, boardSpaceId;

	char peek = 'B';

	std::cout << "Starting Map Load-----" << std::endl;

	while (getline(readMap, line, '['))
	{
		// Parse id
		//id = std::stof(line);
		if (line == "models: ")
		{
			while (readMap.peek() != ']')
			{
				GameObject* entity = read_asset();
				entities.push_back(entity);
				Renderer::add_render_object(entity);
			}
			getline(readMap, line, '\n');
		} else if (line == "lights: ") {
			while (readMap.peek() != ']')
			{
				GameObject* light = read_asset();
				lights.push_back(light);
				Renderer::add_light_render_object(light);

			}
		}

		////check space type 0 : blank, 1 : add3, 2 add6, 3 remove3
		//if (id == 4)
		//{
		//	getline(readMap, line, ',');
		//	boardSpaceId = std::stof(line);
		//}

		////parse position
		//getline(readMap, line, ',');
		//x = std::stof(line);
		//getline(readMap, line, ',');
		//y = std::stof(line);
		//getline(readMap, line, ',');
		//z = std::stof(line);
		
		////parse size
		//getline(readMap, line, ',');
		//sizeX = std::stof(line);
		//getline(readMap, line, ',');
		//sizeY = std::stof(line);
		//getline(readMap, line, ',');
		//sizeZ = std::stof(line);

		////parse rotation
		//getline(readMap, line, ',');
		//rotationX = std::stof(line);
		//getline(readMap, line, ',');
		//rotationY = std::stof(line);
		//getline(readMap, line);
		//rotationZ = std::stof(line);

		//if light
		//if (id == 1)
		//{
		//	//set Id to 10 right now to avoid collisions with board space Idds
		//	GameObject* light = new GameObject(10, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ));
		//	lights.push_back(light);
		//	Renderer::add_light_render_object(light);
		//}
		////if board space
		//else if (id == 4)
		//{
		//	GameObject* boardSpace = new GameObject(boardSpaceId, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ));
		//	entities.push_back(boardSpace);
		//	Renderer::add_render_object(boardSpace);
		//}
		//else
		//{
		//	GameObject* entity = new GameObject(11, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ));
		//	entities.push_back(entity);
		//	Renderer::add_render_object(entity);
		//} 
	} 
	readMap.close();
	load_skybox();
}

GameObject* Map::read_asset()
{
	std::string line;
	getline(readMap, line, ':');
	uint8_t assetId;
	float x, y, z;
	glm::vec3 position, scale, rotation;

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
	getline(readMap, line, '}');
	z = std::stof(line);
	rotation = { x, y, z };
	
	return new GameObject(assetId, AssetManager::get_model(assetId), position, scale, rotation);
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
	GameObject* model = new GameObject(*entities[selectedIndex]);
	entities.push_back(model);
	Renderer::add_render_object(model);
}

void Map::remove_model(int selectedIndex)
{
	entities.erase(entities.begin() + selectedIndex);
	Renderer::remove_render_object(selectedIndex);
}