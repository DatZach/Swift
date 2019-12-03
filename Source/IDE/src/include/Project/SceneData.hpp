/*
 *  SceneData.cpp
 *  Scene Data Abstraction
 */

#ifndef __SCENEDATA_HPP
#define __SCENEDATA_HPP

#include <QFile>
#include <QList>
#include <QString>

namespace SceneData
{
    class ISection
    {
    protected:
        QString ReadString(QDataStream& stream);
        void WriteString(const QString& value, QDataStream& stream);

    public:
		virtual ~ISection();

        virtual void Read(QDataStream& stream) = 0;
    };

    class Configuration : public ISection
    {
    public:
		QSize		    size;
        QString         defaultBgMusic;

    public:
        Configuration();

		void Read(QDataStream& stream);
		void Save(QDataStream& stream);
    };

    class Background : public ISection
    {
    public:
		QColor		color;
        QString     textureName;

    public:
        Background();

		void Read(QDataStream& stream);
		void Save(QDataStream& stream);
    };

    class Tiles : public ISection
    {
    public:
        class TileMap : public ISection
        {
        public:
            class Tile : public ISection
            {
            public:
				QPoint	position;
				QPoint	tilePosition;

            public:
                Tile();

				void Read(QDataStream& stream);
				void Save(QDataStream& stream);
            };

        public:
            QString         textureName;
			QSize		    tileSize;
            QList<Tile>     tiles;

        public:
            TileMap();

			void Read(QDataStream& stream);
			void Save(QDataStream& stream);
        };

    public:
        QList<TileMap> maps;

    public:
        Tiles();

		void Read(QDataStream& stream);
		void Save(QDataStream& stream);

        size_t size() const;
        TileMap& operator[](size_t i);
    };

    class Entity : public ISection
    {
    public:
        QString         tag;
        QString         name;
        unsigned int    x;
        unsigned int    y;
        float           scaleX;
        float           scaleY;
		QColor			color;
        unsigned short  rotation;

    public:
        Entity();

		void Read(QDataStream& stream);
		void Save(QDataStream& stream);
    };

    class Entities : public ISection
    {
    public:
        QList<Entity> entities;

    public:
        Entities();

		void Read(QDataStream& stream);
		void Save(QDataStream& stream);

        size_t size() const;
        Entity& operator[](size_t i);
    };

    class SceneData
    {
    public:
        QString         filename;
        Configuration   configuration;
        Background      background;
        Tiles           tiles;
        Entities        entities;

    public:
        SceneData();

        bool Load(const QString& filePath);
        bool Save(const QString& filePath);

        static bool Create(const QString& name);
    };
}

#endif
