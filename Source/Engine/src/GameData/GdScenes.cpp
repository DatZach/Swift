/*
 *	gdscenes.cpp
 *	Game Data Scenes
 */

#include <GameData/Scenes.hpp>

namespace GameData
{
	Scene::Scene()
		: IAsset(),
		  name(""),
		  stream(nullptr)
	{

	}

	Scene::~Scene()
	{
		delete stream;
	}

	void Scene::Read(Util::Stream* stream)
	{
		name			= stream->ReadString();
		this->stream	= stream->Deserialize(false);
	}

	Scenes::Scenes()
		: IAsset(),
		  scenes()
	{

	}

	Scenes::~Scenes()
	{
		for(Scene* scene : scenes)
			delete scene;
	}
	
	void Scenes::Read(Util::Stream* stream)
	{
		unsigned int count = stream->ReadDword();
		while(count--)
		{
			Scene* scene = new Scene();
			scene->Read(stream);

			scenes.push_back(scene);
		}
	}

	size_t Scenes::size() const
	{
		return scenes.size();
	}

	Scene* Scenes::operator[](const std::string& value) const
	{
		for(Scene* scene : scenes)
		{
			if (scene->name == value)
				return scene;
		}

		return nullptr;
	}

	Scene* Scenes::operator[](size_t i) const
	{
		return scenes[i];
	}
}
