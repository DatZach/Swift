#include <QJson/Parser>
#include <QJson/Serializer>
#include <QMessageBox>
#include <QVariant>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <MainWindow.hpp>
#include <Project/Project.hpp>
#include <Compiler.hpp>

// TODO Probably a really stupid way to do this
extern MainWindow* mainWindow;

ProjectSingleton::ProjectSingleton()
    : isOpen(false),
      projectRootDirectory(""),
      project(nullptr),
      textures(nullptr),
      sounds(nullptr),
      fonts(nullptr),
      scenes(),
      configuration(nullptr),
      scripts(),
      entities(),
      states()
{

}

ProjectSingleton::~ProjectSingleton()
{
    Close();
}

QString ProjectSingleton::New(const QString& name, const QString& location)
{
    QDir dir(location);
    if (!dir.exists())
        dir.mkdir(".");

    QStringList arguments;
    arguments.push_back("create");
    arguments.push_back(location + "/" + name);

    QString createResult = Compiler.Execute(arguments, location);
    if (createResult != "")
        return createResult;

    QDir projectDir(location);
    projectDir.cd(name);

	return !Open(projectDir.absoluteFilePath("project.sproj"))
            ? "Cannot open newly created project."
            : "";
}

bool ProjectSingleton::Open(const QString& path)
{
    // NOTE Might not want to implicitly close an old project?
    if (isOpen)
        Close();

    projectRootDirectory = QFileInfo(path).absolutePath();

    project = new ProjectProperties(path);
    project->Load();

    // TODO Not exactly the best way to do this
	if (project->targetName.isEmpty())
        return false;

	for(QString item : project->assets)
    {
        if (ReadAssetConfiguration(item))
            continue;

        if (item.toLower() == "scenes")
        {
            QDir sceneDirectory(projectRootDirectory);
            sceneDirectory.cd(item);
            QStringList sceneFiles = sceneDirectory.entryList(QStringList("*.sd"));
            for(QString scene : sceneFiles)
                scenes.push_back(Scene(scene));
        }
    }

    for(QString item : project->sources)
    {
        QDir scriptDirectory(projectRootDirectory);
        scriptDirectory.cd(item);

        if (item.toLower() == "entities")
            entities = scriptDirectory.entryList(QStringList("*.ss"));
        else if (item.toLower() == "entities")
            entities = scriptDirectory.entryList(QStringList("*.ss"));
        else
        {
            QStringList scriptFiles = scriptDirectory.entryList(QStringList("*.ss"));
            for(QString script : scriptFiles)
                scripts.push_back(Script(script));
        }
    }

    mainWindow->SyncWithProject();

    // Update recent projects
    QStringList recentProjectsList;
    recentProjectsList.push_back(path.simplified().replace('\\', '/'));

	QDir recentProjectsPath(QCoreApplication::applicationDirPath());
	QFile recentProjects(recentProjectsPath.absoluteFilePath("recent.txt"));
    if (recentProjects.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!recentProjects.atEnd())
        {
            QString line = recentProjects.readLine().simplified();
            if (!recentProjectsList.contains(line.replace('\\', '/')))
                recentProjectsList.push_back(line);
        }

        recentProjects.close();
    }

    if (recentProjects.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream recentProjectsStream(&recentProjects);
        for(QString project : recentProjectsList)
            recentProjectsStream << project << '\n';

        recentProjects.close();
    }

    return true;
}

bool ProjectSingleton::ReadAssetConfiguration(const QString& item)
{
    QString assetFullPath;
    QDir assetPath(projectRootDirectory);
    // TODO This needs to be fixed, should check for ext
    if (!item.contains('.'))
    {
        assetPath.cd(item);
        assetFullPath = assetPath.filePath(item + ".json");
    }
    else
        assetFullPath = assetPath.filePath(item);

    QFile projectFile(assetFullPath);
    if (!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(projectFile.readAll(), &ok).toMap();
    if (!ok)
        return false;

    if (result.contains("Configuration") || result.contains("configuration"))
    {
        configuration = new ProjectConfiguration(assetFullPath);
        configuration->Load();
    }
    else if (result.contains("Textures") || result.contains("textures"))
    {
        textures = new ProjectTextures(assetFullPath);
        textures->Load();
    }
    else if (result.contains("Sounds") || result.contains("sounds"))
    {
        sounds = new ProjectSounds(assetFullPath);
        sounds->Load();
    }
    else if (result.contains("Fonts") || result.contains("fonts"))
    {
        fonts = new ProjectFonts(assetFullPath);
        fonts->Load();
    }
    else
        return false;

    isOpen = true;

    return true;
}

void ProjectSingleton::Close()
{
    isOpen = false;

    projectRootDirectory = "";

    delete project;
    delete textures;
    delete sounds;
    delete fonts;
    scenes.clear();
    delete configuration;
    scripts.clear();
    entities.clear();
    states.clear();
}

void ProjectSingleton::DeleteScene(Scene* scene)
{
    for(int i = 0; i < scenes.size(); ++i)
    {
        if (scene != &scenes[i])
            continue;

        if (!scenes[i].Delete())
            return;

        scenes.removeAt(i);
        return;
    }
}

void ProjectSingleton::DeleteScript(Script* script)
{
    for(int i = 0; i < scripts.size(); ++i)
    {
        if (script != &scripts[i])
            continue;

        if (!scripts[i].Delete())
            return;

        scripts.removeAt(i);
        return;
    }
}

bool ProjectSingleton::IsOpen() const
{
    return isOpen;
}

QString ProjectSingleton::GetTargetPath() const
{
    if (!isOpen)
        return "";

    QDir dir(projectRootDirectory);
    dir.cd("bin");
    return dir.filePath(project->targetName + ".sgd");
}

ProjectSingleton& ProjectSingleton::GetInstance()
{
    static ProjectSingleton instance;
    return instance;
}

ProjectProperties::ProjectProperties(const QString& filepath)
    : JsonObject(filepath),
      targetName(""),
      assets(),
      sources()
{

}

bool ProjectProperties::Load()
{
    if (!LoadJson())
		return false;

    targetName  = GetString("target", targetName);
    assets      = GetStringList("assets", assets);
    sources     = GetStringList("sources", sources);

	return true;
}

ProjectTextures::ProjectTextures(const QString& filepath)
    : JsonObject(filepath),
      textures()
{

}

bool ProjectTextures::Load()
{
    if (!LoadJson())
		return false;

    QVariantMap assets = GetMap("textures");

    auto itr = assets.cbegin();
    for(; itr != assets.cend(); ++itr)
    {
        JsonObject asset(itr.value().toMap());
        Texture texture;

        texture.name = itr.key();
        texture.filename = asset.GetString("filename", texture.filename);
        texture.animated = asset.GetBoolean("animated", texture.animated);
        JsonObject windowSize(asset.GetMap("origin"));
            texture.originX = windowSize.GetInteger("x", texture.originX);
            texture.originY = windowSize.GetInteger("y", texture.originY);
        texture.smooth = asset.GetBoolean("smooth", texture.smooth);
        texture.collisionMode = asset.GetString("cmode", texture.collisionMode);
        texture.lazy = asset.GetBoolean("lazy", texture.lazy);

        textures.push_back(texture);
    }

	return true;
}

bool ProjectTextures::Save()
{
    QVariantMap saveTextures;

    for(Texture texture : textures)
    {
        // Copy file to local directory if needed
        if (QDir::isAbsolutePath(texture.filename))
        {
            QFileInfo fileInfo(texture.filename);

            // TODO Don't resolve paths like this
            QDir texturePath(Project.projectRootDirectory);
            texturePath.cd("Textures");
            QString textureFilePath = texturePath.filePath(fileInfo.fileName());

            QFile::copy(texture.filename, textureFilePath);

            texture.filename = fileInfo.fileName();
        }

        QVariantMap saveTexture;
        saveTexture.insert("filename", texture.filename);
        saveTexture.insert("animated", texture.animated);
        QVariantMap origin;
        origin.insert("x", texture.originX);
        origin.insert("y", texture.originY);
        saveTexture.insert("origin", origin);
        saveTexture.insert("smooth", texture.smooth);
        saveTexture.insert("cmode", texture.collisionMode);
        saveTexture.insert("lazy", texture.lazy);

        saveTextures.insert(texture.name, saveTexture);
    }

    QVariantMap saveTexturesTopLevel;
    saveTexturesTopLevel.insert("Textures", saveTextures);

	return SaveJson(saveTexturesTopLevel);
}

void ProjectTextures::Delete(Texture* texture)
{
    for(int i = 0; i < textures.size(); ++i)
    {
        if (texture != &textures[i])
            continue;

        textures.removeAt(i);
		Save();
		return;
    }
}

ProjectTextures::Texture::Texture()
    : name(""),
      filename(""),
      animated(false),
      originX(0),
      originY(0),
      smooth(false),
      collisionMode("none"),
      lazy(false)
{

}

QString ProjectTextures::Texture::GetPath() const
{
    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Textures");
    return path.filePath(QFileInfo(filename).fileName());
}

ProjectSounds::ProjectSounds(const QString& filepath)
    : JsonObject(filepath),
      sounds()
{

}

bool ProjectSounds::Load()
{
    if (!LoadJson())
		return false;

    QVariantMap assets = GetMap("sounds");

    auto itr = assets.cbegin();
    for(; itr != assets.cend(); ++itr)
    {
        JsonObject asset(itr.value().toMap());
        Sound sound;

        sound.name = itr.key();
        sound.filename = asset.GetString("filename", sound.filename);
        sound.mode = asset.GetString("mode", sound.mode);
        sound.sampleRate = asset.GetInteger("samplerate", sound.sampleRate);
        sound.bitRate = asset.GetInteger("bitrate", sound.bitRate);

        sounds.push_back(sound);
    }

	return true;
}

bool ProjectSounds::Save()
{
    QVariantMap saveSounds;

    for(Sound sound : sounds)
    {
        // Copy file to local directory if needed
        if (QDir::isAbsolutePath(sound.filename))
        {
            QFileInfo fileInfo(sound.filename);

            // TODO Don't resolve paths like this
            QDir soundPath(Project.projectRootDirectory);
            soundPath.cd("Sounds");
            QString soundFilePath = soundPath.filePath(fileInfo.fileName());

            QFile::copy(sound.filename, soundFilePath);

            sound.filename = fileInfo.fileName();
        }

        QVariantMap saveSound;
        saveSound.insert("filename", sound.filename);
        saveSound.insert("mode", sound.mode);
        saveSound.insert("samplerate", sound.sampleRate);
        saveSound.insert("bitrate", sound.bitRate);

        saveSounds.insert(sound.name, saveSound);
    }

    QVariantMap saveSoundsTopLevel;
    saveSoundsTopLevel.insert("Sounds", saveSounds);

	return SaveJson(saveSoundsTopLevel);
}

void ProjectSounds::Delete(Sound* sound)
{
    for(int i = 0; i < sounds.size(); ++i)
    {
        if (sound != &sounds[i])
            continue;

        sounds.removeAt(i);
        Save();
        return;
    }
}

ProjectSounds::Sound::Sound()
    : filename(""),
      mode("memory"),
      sampleRate(44100),
      bitRate(192)
{

}

QString ProjectSounds::Sound::GetPath() const
{
    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Sounds");
    return path.filePath(QFileInfo(filename).fileName());
}

ProjectFonts::ProjectFonts(const QString& filepath)
    : JsonObject(filepath),
      fonts()
{

}

bool ProjectFonts::Load()
{
    if (!LoadJson())
		return false;

    QVariantMap assets = GetMap("fonts");

    auto itr = assets.cbegin();
    for(; itr != assets.cend(); ++itr)
    {
        JsonObject asset(itr.value().toMap());
        Font font;

        font.name = itr.key();
        font.fontFace = asset.GetString("fontface", font.fontFace);
        font.antiAliasing = asset.GetInteger("antialiasing", font.antiAliasing);
        font.size = asset.GetInteger("size", font.size);
        font.bold = asset.GetBoolean("bold", font.bold);
        font.italic = asset.GetBoolean("italic", font.italic);
        font.underline = asset.GetBoolean("underline", font.underline);
        font.overline = asset.GetBoolean("overline", font.overline);
        font.strikeThrough = asset.GetBoolean("strikethrough", font.strikeThrough);

        fonts.push_back(font);
    }

	return true;
}

ProjectFonts::Font::Font()
    : name(""),
      fontFace("@Arial"),
      antiAliasing(true),
      size(10),
      bold(false),
      italic(false),
      underline(false),
      overline(false),
      strikeThrough(false)
{

}

QString ProjectFonts::Font::GetPath() const
{
    if (fontFace.startsWith('@'))
        return fontFace;

    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Fonts");
    return path.filePath(QFileInfo(fontFace).fileName());
}

Scene::Scene(const QString& name)
    : name(name),
      sceneData()
{
    // TODO Don't resolve shit like this
    QDir scenePath(Project.projectRootDirectory);
    scenePath.cd("Scenes");
    QString filePath = scenePath.filePath(name);
    if (!sceneData.Load(filePath))
        QMessageBox::warning(nullptr, "Error", "Cannot load scene.");
}

bool Scene::Rename(const QString& value)
{
    if (name == value)
        return true;

    QString currentPath = GetPath();

    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Scenes");
    QString newPath = path.filePath(QFileInfo(value).fileName());

    return QFile::rename(currentPath, newPath);
}

bool Scene::Delete()
{
    return QFile::remove(GetPath());
}

QString Scene::GetPath() const
{
    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Scenes");

    return path.filePath(QFileInfo(name).fileName());
}

Script::Script(const QString &name)
    : name(name)
{

}

bool Script::Rename(const QString& value)
{
    if (name == value)
        return true;

    QString currentPath = GetPath();

    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Scripts");
    QString newPath = path.filePath(QFileInfo(value).fileName());

    return QFile::rename(currentPath, newPath);
}

bool Script::Delete()
{
    return QFile::remove(GetPath());
}

bool Script::Create(const QString& name)
{
    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Scripts");

    QFile newScriptFile(path.filePath(name));
    return newScriptFile.open(QIODevice::WriteOnly | QIODevice::Text);
}

QString Script::GetPath() const
{
    // TODO Don't resolve paths like this
    QDir path(Project.projectRootDirectory);
    path.cd("Scripts");

    return path.filePath(name);
}

ProjectConfiguration::ProjectConfiguration(const QString& filepath)
    : JsonObject(filepath),
      title("UntitledGame"),
      description(""),
      company(""),
      product(""),
      copyright(""),
      trademark(""),
      version("1.0.0"),
      initialState(""),
      incrementBuild(false),
      sheetSize(2048),
      targetFramerate(60),
      windowWidth(800),
      windowHeight(600),
      fullscreen(false),
      vsync(true),
      cursor(true),
      antiAliasingLevel(0),
      icon("")
{

}

bool ProjectConfiguration::Load()
{
    if (!LoadJson())
		return false;

    JsonObject asset(GetMap("configuration"));

    title = asset.GetString("title", title);
    description = asset.GetString("description", description);
    company = asset.GetString("company", company);
    product = asset.GetString("product", product);
    copyright = asset.GetString("copyright", copyright);
    trademark = asset.GetString("trademark", trademark);
    version = asset.GetString("version", version);
    initialState = asset.GetString("initialstate", initialState);
    incrementBuild = asset.GetBoolean("incbuild", incrementBuild);
    sheetSize = asset.GetInteger("sheetsize", sheetSize);
    targetFramerate = asset.GetInteger("targetframerate", targetFramerate);
    JsonObject windowSize(asset.GetMap("size"));
        windowWidth = windowSize.GetInteger("x", windowWidth);
        windowHeight = windowSize.GetInteger("y", windowHeight);
    fullscreen = asset.GetBoolean("fullscreen", fullscreen);
    vsync = asset.GetBoolean("vsync", vsync);
    cursor = asset.GetBoolean("cursor", cursor);
    antiAliasingLevel = asset.GetInteger("antialiasinglevel", antiAliasingLevel);
    icon = asset.GetString("icon", icon);

	return true;
}

bool ProjectConfiguration::Save()
{
    QVariantMap configuration;
    configuration.insert("title", title);
    configuration.insert("description", description);
    configuration.insert("company", company);
    configuration.insert("product", product);
    configuration.insert("copyright", copyright);
    configuration.insert("trademark", trademark);
    configuration.insert("version", version);
    configuration.insert("initialstate", initialState);
    configuration.insert("incbuild", incrementBuild);
    configuration.insert("sheetsize", sheetSize);
    QVariantMap size;
    size.insert("x", windowWidth);
    size.insert("y", windowHeight);
    configuration.insert("size", size);
    configuration.insert("fullscreen", fullscreen);
    configuration.insert("vsync", vsync);
    configuration.insert("cursor", cursor);
    configuration.insert("antialiasinglevel", antiAliasingLevel);
    configuration.insert("icon", icon);

    QVariantMap topLevelConfig;
    topLevelConfig.insert("Configuration", configuration);

	return SaveJson(topLevelConfig);
}
