/*
 *  SceneData.cpp
 *  Scene Data Abstraction
 */

#include <QMessageBox>
#include <Project/SceneData.hpp>
#include <Project/Project.hpp>

#define SD_MAGIC            ((unsigned int)'LFWS')
#define SD_VERSION          (unsigned short)3
#define SD_TAG_CONFIG       ((unsigned int)'GFNC')
#define SD_TAG_BACKGROUND   ((unsigned int)'DGKB')
#define SD_TAG_TILES        ((unsigned int)'ELIT')
#define SD_TAG_ENTITIES     ((unsigned int)'STNE')

namespace SceneData
{
    SceneData::SceneData()
        : filename(""),
          configuration(),
          background(),
          tiles(),
          entities()
    {

    }

    bool SceneData::Load(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
            return false;

        filename = filePath;

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        unsigned int magic;
        stream >> magic;

        if (magic != SD_MAGIC)
        {
            QMessageBox::warning(nullptr, "Error", "This is not a valid SceneData file.");
            return false;
        }

        unsigned short version;
        stream >> version;
        if (version != SD_VERSION)
        {
            QMessageBox::warning(nullptr, "Error",
                                 QString("SceneData version ") +
                                 QString::number(version) +
                                 QString(" unsupported."));
            return false;
        }

        // Read all sections
        unsigned char sectionCount;
        stream >> sectionCount;
        while(sectionCount--)
        {
            unsigned int tag;
            stream >> tag;

            switch(tag)
            {
                case SD_TAG_CONFIG:
                    configuration.Read(stream);
                    break;

                case SD_TAG_BACKGROUND:
                    background.Read(stream);
                    break;

                case SD_TAG_TILES:
                    tiles.Read(stream);
                    break;

                case SD_TAG_ENTITIES:
                    entities.Read(stream);
                    break;

                default:
                    QMessageBox::warning(nullptr, "Warning", "Unknown tag encountered.");
            }
        }

        return true;
    }

    bool SceneData::Save(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly))
            return false;

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        // Write magic & version
        stream << SD_MAGIC;
        stream << SD_VERSION;

		// Write sections
        // TODO Maybe make this non-static in the future
        stream << (unsigned char)4;

        configuration.Save(stream);
        background.Save(stream);
        tiles.Save(stream);
        entities.Save(stream);

        return true;
    }

    bool SceneData::Create(const QString& name)
    {
        // TODO Don't resolve paths like this
        QDir path(Project.projectRootDirectory);
        path.cd("Scenes");

        // Create scene
        SceneData scene;
        return scene.Save(path.filePath(name));
    }

	ISection::~ISection()
	{

	}

    QString ISection::ReadString(QDataStream& stream)
    {
        QString value;

        // TODO This needs to read 7bit encoded integers
        unsigned char len = 0;
        stream >> len;

        while(len--)
        {
            unsigned char ch;
            stream >> ch;
            value.append(ch);
        }

        return value;
    }

    void ISection::WriteString(const QString& value, QDataStream& stream)
    {
        // TODO This needs to write a 7bit encoded integer
        stream << (unsigned char)value.length();

        size_t length = value.length();
        for(size_t i = 0; i < length; ++i)
        {
            unsigned char ch = value[i].toLatin1();
            stream << ch;
        }
    }

    Configuration::Configuration()
        : ISection(),
          size(Project.configuration->windowWidth, Project.configuration->windowHeight),
          defaultBgMusic("")
    {

    }

    void Configuration::Read(QDataStream &stream)
    {
		unsigned int value;

		stream >> value;
		size.setWidth(value);

		stream >> value;
		size.setHeight(value);

        defaultBgMusic = ReadString(stream);
    }

    void Configuration::Save(QDataStream &stream)
    {
        stream << SD_TAG_CONFIG;

		stream << size.width();
		stream << size.height();
        WriteString(defaultBgMusic, stream);
    }

    Background::Background()
        : ISection(),
		  color(100, 149, 237),
          textureName("")
    {

    }

    void Background::Read(QDataStream &stream)
    {
        unsigned int rgba = 0;
        stream >> rgba;
		color = QColor(rgba & 0xFF, (rgba >> 8) & 0xFF, (rgba >> 16) & 0xFF);
        textureName = ReadString(stream);
    }

    void Background::Save(QDataStream &stream)
    {
        stream << SD_TAG_BACKGROUND;

		int r, g, b;
		color.getRgb(&r, &g, &b);

		stream << (unsigned int)((0xFF << 24) | (b << 16) | (g << 8) | r);
        WriteString(textureName, stream);
    }

    Tiles::TileMap::Tile::Tile()
        : ISection(),
          position(),
          tilePosition()
    {

    }

    void Tiles::TileMap::Tile::Read(QDataStream &stream)
    {
        unsigned short w;
        unsigned char b;

        stream >> w;
		position.setX(w);
        stream >> w;
		position.setY(w);
        stream >> b;
		tilePosition.setX(b);
        stream >> b;
		tilePosition.setY(b);
    }

    void Tiles::TileMap::Tile::Save(QDataStream &stream)
    {
		stream << (unsigned short)position.x();
		stream << (unsigned short)position.y();
		stream << (unsigned char)tilePosition.x();
		stream << (unsigned char)tilePosition.y();
    }

    Tiles::TileMap::TileMap()
        : ISection(),
          textureName(""),
          tileSize(),
          tiles()
    {

    }

    void Tiles::TileMap::Read(QDataStream &stream)
    {
        textureName = ReadString(stream);

        unsigned short v;
        stream >> v;
		tileSize.setWidth(v);
        stream >> v;
		tileSize.setHeight(v);

        unsigned int count = 0;
        stream >> count;
        while(count--)
        {
            Tile tile;
            tile.Read(stream);

            tiles.push_back(tile);
        }
    }

    void Tiles::TileMap::Save(QDataStream &stream)
    {
		stream << (unsigned short)tileSize.width();
		stream << (unsigned short)tileSize.height();

        stream << (unsigned int)tiles.size();
        for(int i = 0; i < tiles.size(); ++i)
            tiles[i].Save(stream);
    }

    Tiles::Tiles()
        : ISection(),
          maps()
    {

    }

    void Tiles::Read(QDataStream &stream)
    {
        unsigned int count;
        stream >> count;

        while(count--)
        {
            TileMap map;
            map.Read(stream);

            maps.push_back(map);
        }
    }

    void Tiles::Save(QDataStream &stream)
    {
        stream << SD_TAG_TILES;

        stream << (unsigned int)maps.size();
        for(int i = 0; i < maps.size(); ++i)
            maps[i].Save(stream);
    }

    size_t Tiles::size() const
    {
        return maps.size();
    }

    Tiles::TileMap& Tiles::operator [](size_t i)
    {
        return maps[i];
    }

    Entity::Entity()
        : ISection(),
          tag(""),
          name(""),
          x(0),
          y(0),
          scaleX(0.0f),
          scaleY(0.0f),
		  color(255, 255, 255),
          rotation(0)
    {

    }

    void Entity::Read(QDataStream &stream)
    {
        tag = ReadString(stream);
        name = ReadString(stream);
        stream >> x;
        stream >> y;
        stream >> scaleX;
        stream >> scaleY;

        unsigned int rgba;
        stream >> rgba;
		color = QColor(rgba & 0xFF, (rgba >> 8) & 0xFF, (rgba >> 16) & 0xFF);

        stream >> rotation;
    }

    void Entity::Save(QDataStream &stream)
    {
		int r, g, b;
		color.getRgb(&r, &g, &b);

        WriteString(tag, stream);
        WriteString(name, stream);
        stream << x;
        stream << y;
        stream << scaleX;
        stream << scaleY;
		stream << (unsigned int)((0xFF << 24) | (b << 16) | (g << 8) | r);
        stream << rotation;
    }

    Entities::Entities()
        : ISection(),
          entities()
    {

    }

    void Entities::Read(QDataStream &stream)
    {
        unsigned int count;
        stream >> count;

        while(count--)
        {
            Entity entity;
            entity.Read(stream);

            entities.push_back(entity);
        }
    }

    void Entities::Save(QDataStream &stream)
    {
        stream << SD_TAG_ENTITIES;

        stream << (unsigned int)entities.size();
        for(int i = 0; i < entities.size(); ++i)
            entities[i].Save(stream);
    }

    size_t Entities::size() const
    {
        return entities.size();
    }

    Entity& Entities::operator [](size_t i)
    {
        return entities[i];
    }
}
