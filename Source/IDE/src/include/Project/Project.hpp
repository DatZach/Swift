/*
 *  Project.hpp
 *  Project Abstraction
 */

#ifndef __PROJECT_HPP
#define __PROJECT_HPP

#include <QString>
#include <QDir>
#include <Project/SceneData.hpp>
#include <Project/JsonObject.hpp>

#define Project ProjectSingleton::GetInstance()

enum class ProjectAssetType
{
    Texture,
    Sound,
    Font,
    Scene,
    Script,
    Configuration,
    Unknown
};

class ProjectAsset
{
public:
    virtual ~ProjectAsset()
    {

    }
};

class ProjectTextures : public JsonObject, public ProjectAsset
{
public:
    class Texture : public ProjectAsset
    {
    public:
        QString     name;
        QString     filename;
        bool        animated;
        int         originX;
        int         originY;
        bool        smooth;
        // BBox
        QString     collisionMode;
        bool        lazy;

    public:
        Texture();

        QString GetPath() const;
    };

public:
    QList<Texture>  textures;

public:
    ProjectTextures(const QString& filepath);
	bool Load();
	bool Save();

    void Delete(Texture* texture);
};

class ProjectSounds : public JsonObject, public ProjectAsset
{
public:
    class Sound : public ProjectAsset
    {
    public:
        QString         name;
        QString         filename;
        QString         mode;
        unsigned int    sampleRate;
        unsigned int    bitRate;

    public:
        Sound();

        QString GetPath() const;
    };

public:
    QList<Sound> sounds;

public:
    ProjectSounds(const QString& filepath);
	bool Load();
	bool Save();

    void Delete(Sound* sound);
};

class ProjectFonts : public JsonObject, public ProjectAsset
{
public:
    class Font : public ProjectAsset
    {
    public:
        QString         name;
        QString         fontFace;
        bool            antiAliasing;
        unsigned int    size;
        bool            bold;
        bool            italic;
        bool            underline;
        bool            overline;
        bool            strikeThrough;
        // Outline
        // DropShadow

    public:
        Font();

        QString GetPath() const;
    };

public:
    QList<Font> fonts;

public:
    ProjectFonts(const QString& filepath);
	bool Load();
};

class Scene : public ProjectAsset
{
public:
    QString                 name;
    SceneData::SceneData    sceneData;

public:
    Scene(const QString& name);

    bool Rename(const QString& value);
    bool Delete();
    QString GetPath() const;
};

class Script : public ProjectAsset
{
public:
    QString         name;

public:
    Script(const QString& name);

    bool Rename(const QString& value);
    bool Delete();
    QString GetPath() const;

    static bool Create(const QString& name);
};

class ProjectConfiguration : public JsonObject, public ProjectAsset
{
public:
    QString         title;
    QString         description;
    QString         company;
    QString         product;
    QString         copyright;
    QString         trademark;
    QString         version;
    QString         initialState;
    bool            incrementBuild;
    unsigned int    sheetSize;
    unsigned int    targetFramerate;
    unsigned int    windowWidth;
    unsigned int    windowHeight;
    bool            fullscreen;
    bool            vsync;
    bool            cursor;
    unsigned int    antiAliasingLevel;
    QString         icon;

public:
    ProjectConfiguration(const QString& filepath);
	bool Load();
	bool Save();
};

class ProjectProperties : public JsonObject, public ProjectAsset
{
public:
    QString         targetName;
    QStringList     assets;
    QStringList     sources;

public:
    ProjectProperties(const QString& filepath);
	bool Load();
};

class ProjectSingleton
{
private:
    bool isOpen;

    bool ReadAssetConfiguration(const QString& item);

public:
    QString                 projectRootDirectory;
    ProjectProperties*      project;
    ProjectTextures*        textures;
    ProjectSounds*          sounds;
    ProjectFonts*           fonts;
    QList<Scene>            scenes;
    ProjectConfiguration*   configuration;
    QList<Script>           scripts;
    QStringList             entities;
    QStringList             states;

public:
    ProjectSingleton();
    ~ProjectSingleton();

    QString New(const QString& name, const QString& location);
    bool Open(const QString& path);
    void Close();

    void DeleteScene(Scene* scene);
    void DeleteScript(Script* script);

    bool IsOpen() const;
    QString GetTargetPath() const;

    static ProjectSingleton& GetInstance();
};

#endif
