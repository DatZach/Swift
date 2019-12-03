/*
 *  SceneEditor.cpp
 *  Scene Editor Form
 */

#include <QDebug>
#include <QList>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <cmath>
#include <Forms/SceneEditor.hpp>
#include <Forms/QSFMLCanvas.hpp>
#include <Dialogs/ScenePropertiesDialog.hpp>
#include <Project/Lexer/TokenStream.hpp>
#include <Project/Project.hpp>

/*
 *  TODO
 *  [X] Background texture
 *  [X] Set BG color
 *  [X] BG Music
 *  [X] Set width/height
 *  [X] Entities
 *      [X] View
 *      [X] Add
 *      [X] Remove
 *  [X] Tiles
 *      [X] View
 *      [X] Add
 *      [X] Remove
 */

class SceneTexture
{
public:
    QString     name;
    sf::Texture texture;

public:
    SceneTexture(const QString& name)
        : name(name),
          texture()
    {
        ProjectTextures::Texture textureProperties;
        for(auto& texture : Project.textures->textures)
        {
            if (texture.name == name)
            {
                textureProperties = texture;
                break;
            }
        }

        // TODO Don't resolve paths like this
        QDir texturePath(Project.projectRootDirectory);
        texturePath.cd("Textures");
        QString filePath = texturePath.filePath(textureProperties.filename);

        bool textureLoaded = texture.loadFromFile(filePath.toStdString());
        if (!textureLoaded)
            qDebug() << "Cannot load texture \"" + filePath + "\".";
    }
};

class SceneEntity
{
public:
    QString     name;
    sf::Texture texture;

public:
    SceneEntity(const QString& scriptFilename)
        : name(""),
          texture()
    {
        /*
         *  Read //!entity Ident TextureIdent
         *  TODO Remove this stupid shit
         */

		//qDebug() << "SceneEntity()";
		bool ok;
		Lexer::TokenStream stream(scriptFilename, ok);
		if (!ok)
			return;

		if (!stream.Accept("entity"))
			return;

		name = stream.ReadWord();
		if (stream.Accept(Lexer::TokenType::Delimiter, "{"))
		{
			while(!stream.Accept(Lexer::TokenType::Delimiter, "}"))
			{
				QString ident = stream.ReadWord();
				if (ident == "Sprite")
				{
					stream.Read(); // =
					stream.Read(); // Sprite
					stream.Read(); // (
					if (stream.Accept("Textures"))
					{
						stream.Read(); // .
						SceneTexture sceneTexture(stream.ReadWord());
						texture = sceneTexture.texture;
					}
					else if (stream.Pass(Lexer::TokenType::String))
					{
						QString value = stream.ReadVariant().toString();
						SceneTexture sceneTexture(value);
						texture = sceneTexture.texture;
					}
				}
			}
		}
    }
};

class SceneEditorView : public QSFMLCanvas
{
private:
    enum class Mode
    {
        View,
        Entity,
        Tile
    };

    enum class Align
    {
        Left,
        Center,
        Right
    };

private:
    SceneData::SceneData* sceneData;
    QList<SceneEntity> sceneEntities;
    QList<SceneTexture> sceneTextures;

    sf::RenderTexture bgGrid;
    QByteArray fontBuffer;
    sf::Font uiFont;

    Mode mode;
    int entityOffset;
    int tilesetOffset;

    sf::Vector2i mouseDelta;
    bool dragging;
    bool showGrid;
    unsigned int gridSize;

    int SnapValue(int value, int x)
    {
        if (x == 0)
            return value;

        while(value % x != 0)
            --value;

        return value;
    }

    void CreateCheckerBoardBackground()
    {
        if (!bgGrid.create(48 * 2, 48 * 2))
            qDebug() << "Cannot create checker board texture.";

        bgGrid.setRepeated(true);

        bgGrid.clear(sf::Color(170 + 64, 170 + 64, 170 + 64));

        sf::RectangleShape checker(sf::Vector2f(48.0f, 48.0f));
        checker.setFillColor(sf::Color::White);
        checker.setPosition(0.0f, 0.0f);
        bgGrid.draw(checker);

        checker.move(48.0f, 48.0f);
        bgGrid.draw(checker);

        bgGrid.display();
    }

    void CameraCenter()
    {
        sf::Vector2u windowSize = RenderWindow::getSize();
        sf::View centerView = RenderWindow::getView();
        centerView.setCenter(windowSize.x / 2, windowSize.y / 2);
        centerView.move(-((int)windowSize.x - (int)sceneData->configuration.size.x) / 2,
                        -((int)windowSize.y - (int)sceneData->configuration.size.y) / 2);
        RenderWindow::setView(centerView);
    }

    void DrawTextDropShadow(const QString& value, const sf::Vector2f& pos, int size = 14,
                            Align align = Align::Left)
    {
        sf::Text text(value.toStdString(), uiFont, size);
            text.setPosition(pos + sf::Vector2f(zoomFactor, zoomFactor));
            text.setColor(sf::Color(22, 35, 40, 224));
            text.setScale(zoomFactor, zoomFactor);
        if (align == Align::Center)
            text.move(-std::floor(text.getLocalBounds().width / 2.0f) * zoomFactor, 0.0f);
        else if (align == Align::Right)
            text.move(-text.getLocalBounds().width * zoomFactor, 0.0f);
        RenderWindow::draw(text);
            text.move(-zoomFactor, -zoomFactor);
            text.setColor(sf::Color::White);
        RenderWindow::draw(text);
    }

    void DrawUI()
    {
        // Draw mode specific UI elements
        switch(mode)
        {
            case Mode::Entity:
                DrawUIEntityMode();
                break;

            case Mode::Tile:
                DrawUITileMode();
                break;
        }

        // Draw status bar
        DrawUIStatusBar();
    }

    void DrawUIStatusBar()
    {
        sf::Vector2f viewCenter = RenderWindow::getView().getCenter();
        sf::Vector2f viewSize = RenderWindow::getView().getSize();

        // Status bar
        sf::RectangleShape statusBar(sf::Vector2f(viewSize.x, 20.0f * zoomFactor));
        statusBar.setPosition(viewCenter.x - viewSize.x / 2.0f,
                              viewCenter.y + viewSize.y / 2.0f - 20.0f * zoomFactor);
        statusBar.setFillColor(sf::Color(0, 0, 0, 160));
        RenderWindow::draw(statusBar);

        // Mode text
        const char* modeText[3] = { "View Mode", "Entity Mode", "Tile Mode" };
        QString modeStatus = modeText[(int)mode];
        modeStatus += " | ";
        modeStatus += QString::number(gridSize);
        modeStatus += "x";
        modeStatus += QString::number(gridSize);
        modeStatus += " grid | ";
        modeStatus += QString::number(1 / zoomFactor, '.', 1);
        modeStatus += "x zoom";
        DrawTextDropShadow(modeStatus, sf::Vector2f(viewCenter.x - viewSize.x / 2.0f + 6.0f * zoomFactor,
                                                    viewCenter.y + viewSize.y / 2.0f - 19.0f * zoomFactor));

        unsigned int tileCount = 0;
        for(auto& tileMap : sceneData->tiles.maps)
            tileCount += tileMap.tiles.size();

        QString sceneStatus = "";
        sceneStatus += QString::number(sceneData->configuration.size.x);
        sceneStatus += "x";
        sceneStatus += QString::number(sceneData->configuration.size.y);
        sceneStatus += " | ";
        sceneStatus += QString::number(sceneData->entities.size());
        sceneStatus += " Entities | ";
        sceneStatus += QString::number(tileCount);
        sceneStatus += " Tiles";

        DrawTextDropShadow(sceneStatus,
                           sf::Vector2f(viewCenter.x + viewSize.x / 2.0f - 6.0f * zoomFactor,
                                        viewCenter.y + viewSize.y / 2.0f - 19.0f * zoomFactor), 14, Align::Right);
    }

    void DrawUIEntityMode()
    {
        sf::Vector2f viewCenter = RenderWindow::getView().getCenter();
        sf::Vector2f viewSize = RenderWindow::getView().getSize();

        sf::RectangleShape shape(sf::Vector2f(viewSize.x, 128.0f * zoomFactor));
            shape.setPosition(viewCenter.x - viewSize.x / 2,
                              viewCenter.y - viewSize.y / 2);
            shape.setFillColor(sf::Color(0, 0, 0, 160));
        RenderWindow::draw(shape);

        // Draw entities
        int i = 0;
        for(SceneEntity& entity : sceneEntities)
        {
            sf::Vector2u textureSize = entity.texture.getSize();
            sf::Sprite sprite(entity.texture);
                sprite.setPosition(viewCenter.x - entityOffset * 128 * zoomFactor + i * 128 * zoomFactor,
                                   viewCenter.y - viewSize.y / 2 + 64 * zoomFactor);
                sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);

            if (textureSize.x > 128 || textureSize.y > 128)
            {
                float xScale = 128.0f / textureSize.x;
                float yScale = 128.0f / textureSize.y;
                float scale = xScale < yScale ? xScale : yScale;

                sprite.setScale(scale * zoomFactor, scale * zoomFactor);
            }
            else
                sprite.setScale(zoomFactor, zoomFactor);

            sprite.setColor(i == entityOffset
                                ? sf::Color::White
                                : sf::Color(144, 144, 144));

            RenderWindow::draw(sprite);

            DrawTextDropShadow(entity.name,
                               sf::Vector2f(viewCenter.x - entityOffset * 128 * zoomFactor +
                                            i * 128 * zoomFactor,
                                            viewCenter.y - viewSize.y / 2 + (128 - 20) * zoomFactor),
                               14, Align::Center);

            ++i;
        }
    }

    void DrawUITileMode()
    {
        sf::Vector2f viewCenter = RenderWindow::getView().getCenter();
        sf::Vector2f viewSize = RenderWindow::getView().getSize();

        // UI Window
        sf::RectangleShape shape(sf::Vector2f(328.0f * zoomFactor, 400.0f * zoomFactor));
            shape.setPosition(viewCenter.x + viewSize.x / 2 - 332.0f * zoomFactor,
                              viewCenter.y - viewSize.y / 2 + 4.0f * zoomFactor);
            shape.setFillColor(sf::Color(0, 0, 0, 160));
        RenderWindow::draw(shape);

        sf::RectangleShape shapeHeader(sf::Vector2f(328.0f * zoomFactor, 72.0f * zoomFactor));
            shapeHeader.setPosition(viewCenter.x + viewSize.x / 2 - 332.0f * zoomFactor,
                                    viewCenter.y - viewSize.y / 2 + 4.0f * zoomFactor);
            shapeHeader.setFillColor(sf::Color(0, 0, 0, 160));
        RenderWindow::draw(shapeHeader);

        // Selected Texture thumbnail
        sf::Vector2u textureSize = sceneTextures[tilesetOffset].texture.getSize();
        sf::Sprite selectedTextureThumbnail(sceneTextures[tilesetOffset].texture);
            selectedTextureThumbnail.setPosition(viewCenter.x + viewSize.x / 2 - 328.0f * zoomFactor
                                                 + 32.0f * zoomFactor,
                                                 viewCenter.y - viewSize.y / 2 + 8.0f * zoomFactor
                                                 + 32.0f * zoomFactor);
            selectedTextureThumbnail.setOrigin(textureSize.x / 2, textureSize.y / 2);
        if (textureSize.x > 64 || textureSize.y > 64)
        {
            float xScale = 64.0f / textureSize.x;
            float yScale = 64.0f / textureSize.y;
            float scale = xScale < yScale ? xScale : yScale;

            selectedTextureThumbnail.setScale(scale * zoomFactor, scale * zoomFactor);
        }
        else
            selectedTextureThumbnail.setScale(zoomFactor, zoomFactor);

        // Selected Texture text
        DrawTextDropShadow(sceneTextures[tilesetOffset].name,
                sf::Vector2f(viewCenter.x + viewSize.x / 2 - 328.0f * zoomFactor
                             + 68.0f * zoomFactor,
                             viewCenter.y - viewSize.y / 2 + 8.0f * zoomFactor
                             + 22.0f * zoomFactor));

        RenderWindow::draw(selectedTextureThumbnail);

        // Draw canvas
        sf::Sprite spriteCanvas(sceneTextures[tilesetOffset].texture);
            spriteCanvas.setPosition(viewCenter.x + viewSize.x / 2 - 328.0f * zoomFactor,
                                     viewCenter.y - viewSize.y / 2 + 84.0f * zoomFactor);
        float canvasScale = 1.0f;
        if (textureSize.x > 320 || textureSize.y > 320)
        {
            float xScale = 320.0f / textureSize.x;
            float yScale = 320.0f / textureSize.y;
            canvasScale = xScale < yScale ? xScale : yScale;

            spriteCanvas.setScale(canvasScale * zoomFactor, canvasScale * zoomFactor);
        }
        else
            spriteCanvas.setScale(zoomFactor, zoomFactor);

        RenderWindow::draw(spriteCanvas);

        // Draw currently selected tile
        sf::RectangleShape selectedTile(
                sf::Vector2f(gridSize * canvasScale * zoomFactor + canvasScale * zoomFactor,
                             gridSize * canvasScale * zoomFactor + canvasScale * zoomFactor));
            selectedTile.setPosition(viewCenter.x + viewSize.x / 2 - 328.0f * zoomFactor -
                                     canvasScale * zoomFactor,
                                     viewCenter.y - viewSize.y / 2 + 84.0f * zoomFactor -
                                     canvasScale * zoomFactor);
            selectedTile.setOutlineColor(sf::Color(255, 0, 0, 160));
            selectedTile.setOutlineThickness(2.0f * zoomFactor);
            selectedTile.setFillColor(sf::Color::Transparent);
        RenderWindow::draw(selectedTile);
    }

protected:
    void OnInit()
    {
        CreateCheckerBoardBackground();

        QFile fontFile(":/fonts/Titillium-RegularUpright.otf");
        if (!fontFile.open(QIODevice::ReadOnly))
            qDebug() << "Cannot open embedded font.";
        else
        {
            fontBuffer = fontFile.readAll();
            if (!uiFont.loadFromMemory(fontBuffer, fontFile.size()))
                qDebug() << "Cannot load UI font.";
        }

        CameraCenter();
    }

    void OnUpdate()
    {
        // It's very very important we call our base
        QSFMLCanvas::OnUpdate();

        if (hasFocus())
        {
            sf::View view = RenderWindow::getView();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.move(0.0f, sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ? -1.0f : -4.0f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
                     !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                view.move(0.0f, sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ? 1.0f : 4.0f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.move(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ? -1.0f : -4.0f, 0.0f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.move(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ? 1.0f : 4.0f, 0.0f);

            RenderWindow::setView(view);
        }
    }

    void OnDraw()
    {
        sf::Vector2f viewCenter = RenderWindow::getView().getCenter();
        sf::Vector2f viewSize = RenderWindow::getView().getSize();

        // Clear that shit, yo
        RenderWindow::clear(sf::Color(192, 192, 192));

        // Draw checkers
        sf::Sprite checkersBgSprite(bgGrid.getTexture());
        checkersBgSprite.setPosition(sf::Vector2f(viewCenter.x - viewSize.x / 2.0f,
                                                  viewCenter.y - viewSize.y / 2.0f));
        checkersBgSprite.setTextureRect(sf::IntRect(0, 0, viewSize.x / zoomFactor, viewSize.y / zoomFactor));
        checkersBgSprite.setScale(zoomFactor, zoomFactor);
        RenderWindow::draw(checkersBgSprite);

        // Draw scene background
        sf::RectangleShape sceneBgColor(sf::Vector2f(sceneData->configuration.size));
        sceneBgColor.setFillColor(sceneData->background.color);
        sceneBgColor.setPosition(0.0f, 0.0f);
        RenderWindow::draw(sceneBgColor);

        if (sceneData->background.textureName != "")
        {
            sf::Texture backgroundTexture;
            bool foundTexture = false;
            for(SceneTexture& sceneTexture : sceneTextures)
            {
                if (sceneData->background.textureName == sceneTexture.name)
                {
                    backgroundTexture = sceneTexture.texture;
                    foundTexture = true;
                    break;
                }
            }

            if (foundTexture)
            {
                sf::Vector2u textureSize = backgroundTexture.getSize();
                sf::Sprite backgroundTextureSprite(backgroundTexture);
                backgroundTextureSprite.setPosition(0.0f, 0.0f);
                backgroundTextureSprite.setScale((float)sceneData->configuration.size.x / textureSize.x,
                                                 (float)sceneData->configuration.size.y / textureSize.y);
                RenderWindow::draw(backgroundTextureSprite);
            }
        }

        // Draw tile maps
        for(auto& tileMap : sceneData->tiles.maps)
        {
            sf::Texture tileMapTexture;
            bool foundTexture = false;
            for(SceneTexture& sceneTexture : sceneTextures)
            {
                if (tileMap.textureName == sceneTexture.name)
                {
                    tileMapTexture = sceneTexture.texture;
                    foundTexture = true;
                    break;
                }
            }

            for(auto& tile : tileMap.tiles)
            {
                if (foundTexture)
                {
                    sf::Sprite spriteTile(tileMapTexture);
                    spriteTile.setPosition(tile.position.x * tileMap.tileSize.x,
                                           tile.position.y * tileMap.tileSize.y);
                    spriteTile.setTextureRect(sf::IntRect(tile.tilePosition.x * tileMap.tileSize.x,
                                                          tile.tilePosition.y * tileMap.tileSize.y,
                                                          tileMap.tileSize.x, tileMap.tileSize.y));
                    RenderWindow::draw(spriteTile);
                }
                else
                {
                    sf::RectangleShape shape(sf::Vector2f(tileMap.tileSize));
                    shape.setPosition(sf::Vector2f(tile.position.x * tileMap.tileSize.x,
                                                   tile.position.y * tileMap.tileSize.y));
                    shape.setFillColor(sf::Color::Green);
                    RenderWindow::draw(shape);
                }
            }
        }

        // Draw entities
        for(auto& entity : sceneData->entities.entities)
        {
            sf::Texture entityTexture;
            bool foundTexture = false;
            for(SceneEntity& sceneEntity : sceneEntities)
            {
                if (entity.name == sceneEntity.name)
                {
                    entityTexture = sceneEntity.texture;
                    foundTexture = true;
                    break;
                }
            }

            if (foundTexture)
            {
                sf::Sprite entitySprite(entityTexture);
                entitySprite.setPosition(entity.x, entity.y);
                RenderWindow::draw(entitySprite);
            }
            else
            {
                sf::RectangleShape shape(sf::Vector2f(24.0f, 24.0f));
                shape.setPosition(entity.x, entity.y);
                shape.setFillColor(sf::Color::Magenta);
                RenderWindow::draw(shape);
            }
        }

        // Draw grid
        if (showGrid && zoomFactor < 2.0f)
        {
            unsigned int yCount = sceneData->configuration.size.y / gridSize +
                                    ((sceneData->configuration.size.y % gridSize > 0) ? 1 : 0);
            unsigned int xCount = sceneData->configuration.size.x / gridSize +
                                    ((sceneData->configuration.size.x % gridSize > 0) ? 1 : 0);

            for(unsigned int y = 1; y < yCount; ++y)
            {
                sf::RectangleShape line(sf::Vector2f(sceneData->configuration.size.x, zoomFactor));
                line.setPosition(0.0f, y * gridSize);
                line.setFillColor(sf::Color(0, 0, 0, 160));
                RenderWindow::draw(line);
            }

            for(unsigned int x = 1; x < xCount; ++x)
            {
                sf::RectangleShape line(sf::Vector2f(zoomFactor, sceneData->configuration.size.y));
                line.setPosition(x * gridSize, 0.0f);
                line.setFillColor(sf::Color(0, 0, 0, 160));
                RenderWindow::draw(line);
            }
        }

        // Draw overlaying scene boundary
        sf::RectangleShape sceneBoundaryOverlay(sf::Vector2f(sceneData->configuration.size.x - zoomFactor * 2.0f,
                                                             sceneData->configuration.size.y - zoomFactor * 2.0f));
        sceneBoundaryOverlay.setFillColor(sf::Color::Transparent);
        sceneBoundaryOverlay.setOutlineColor(sf::Color::Black);
        sceneBoundaryOverlay.setOutlineThickness(zoomFactor);
        sceneBoundaryOverlay.setPosition(zoomFactor, zoomFactor);
        RenderWindow::draw(sceneBoundaryOverlay);

        // Draw UI
        DrawUI();
    }

    void EventMouseButtonPressed(const sf::Event::MouseButtonEvent &args)
    {
        this->focusWidget();
        switch(args.button)
        {
            case sf::Mouse::Left:
                if (mode == Mode::Entity)
                {
                    sf::Vector2f mousePos = GetMousePosition();
                    SceneEntity sceneEntity = sceneEntities[entityOffset];

                    SceneData::Entity entity;
                    entity.name = sceneEntity.name;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
                    {
                        entity.x = mousePos.x;
                        entity.y = mousePos.y;
                    }
                    else
                    {
                        entity.x = SnapValue(mousePos.x, gridSize);
                        entity.y = SnapValue(mousePos.y, gridSize);
                    }

                    sceneData->entities.entities.push_back(entity);

                    SceneEditor* sceneEditor = (SceneEditor*)parent();
                    QTabWidget* tabWidget = (QTabWidget*)parent()->parent()->parent();
                    tabWidget->setTabText(tabWidget->indexOf(sceneEditor), sceneEditor->scene->name + "*");
                }
                else if (mode == Mode::Tile)
                {
                    sf::Vector2f mousePos = GetMousePosition();
                    unsigned int tX = mousePos.x / gridSize;
                    unsigned int tY = mousePos.y / gridSize;
                    if (tX > sceneData->configuration.size.x / gridSize ||
                        tY > sceneData->configuration.size.y / gridSize)
                        return;

                    SceneData::Tiles::TileMap::Tile tile;
                    tile.position.x = tX;
                    tile.position.y = tY;
                    tile.tilePosition.x = 0;
                    tile.tilePosition.y = 0;

                    sceneData->tiles.maps[0].tiles.push_back(tile);

                    SceneEditor* sceneEditor = (SceneEditor*)parent();
                    QTabWidget* tabWidget = (QTabWidget*)parent()->parent()->parent();
                    tabWidget->setTabText(tabWidget->indexOf(sceneEditor), sceneEditor->scene->name + "*");
                }
                break;

            case sf::Mouse::Right:
                if (mode == Mode::Entity)
                {
                    for(size_t i = 0; i < sceneData->entities.size(); ++i)
                    {
                        auto& entity = sceneData->entities[i];
                        sf::Texture entityTexture;
                        bool foundTexture = false;
                        for(SceneEntity& sceneEntity : sceneEntities)
                        {
                            if (entity.name == sceneEntity.name)
                            {
                                entityTexture = sceneEntity.texture;
                                foundTexture = true;
                                break;
                            }
                        }

                        if (!foundTexture)
                            continue;

                        sf::Vector2f mousePos = GetMousePosition();
                        sf::IntRect rect(entity.x, entity.y, entityTexture.getSize().x,
                                         entityTexture.getSize().y);
                        if (rect.contains(sf::Vector2i(mousePos)))
                        {
                            sceneData->entities.entities.removeAt(i);
                            break;
                        }
                    }

                    SceneEditor* sceneEditor = (SceneEditor*)parent();
                    QTabWidget* tabWidget = (QTabWidget*)parent()->parent()->parent();
                    tabWidget->setTabText(tabWidget->indexOf(sceneEditor), sceneEditor->scene->name + "*");
                }
                else if (mode == Mode::Tile)
                {
                    sf::Vector2f mousePos = GetMousePosition();
                    unsigned int tX = mousePos.x / gridSize;
                    unsigned int tY = mousePos.y / gridSize;
                    if (tX > sceneData->configuration.size.x / gridSize ||
                        tY > sceneData->configuration.size.y / gridSize)
                        return;

                    for(int i = 0; i < sceneData->tiles.maps[0].tiles.size(); ++i)
                    {
                        auto tile = sceneData->tiles.maps[0].tiles[i];
                        if (tile.position.x == tX && tile.position.y == tY)
                        {
                            sceneData->tiles.maps[0].tiles.removeAt(i);
                            break;
                        }
                    }

                    SceneEditor* sceneEditor = (SceneEditor*)parent();
                    QTabWidget* tabWidget = (QTabWidget*)parent()->parent()->parent();
                    tabWidget->setTabText(tabWidget->indexOf(sceneEditor), sceneEditor->scene->name + "*");
                }
                break;

            case sf::Mouse::Middle:
                mouseDelta = sf::Vector2i(args.x, args.y);
                dragging = true;
                break;
        }
    }

    void EventMouseButtonReleased(const sf::Event::MouseButtonEvent &args)
    {
        switch(args.button)
        {
            case sf::Mouse::Button::Middle:
                dragging = false;
                break;
        }
    }

    void EventMouseMove(const sf::Event::MouseMoveEvent &args)
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Middle) || !dragging)
            return;

        sf::View view = RenderWindow::getView();
        view.move((mouseDelta.x - args.x) * zoomFactor, (mouseDelta.y - args.y) * zoomFactor);
        RenderWindow::setView(view);

        mouseDelta = sf::Vector2i(args.x, args.y);
    }

    void EventMouseWheelMoved(const sf::Event::MouseWheelEvent &args)
    {
        sf::View view = RenderWindow::getView();

        if (args.delta < 0 && zoomFactor < 3.0f)
        {
            view.zoom(1.1f);
            zoomFactor *= 1.1f;
        }
        else if (args.delta > 0 && zoomFactor > 0.25f)
        {
            view.zoom(0.9f);
            zoomFactor *= 0.9f;
        }
        else
            return;

        RenderWindow::setView(view);
    }

    void EventKeyPressed(const sf::Event::KeyEvent &args)
    {
        switch(args.code)
        {
            case sf::Keyboard::G:
                showGrid = !showGrid;
                break;

            case sf::Keyboard::C:
                CameraCenter();
                break;

            case sf::Keyboard::R:
                mode = Mode::View;
                break;

            case sf::Keyboard::P:
            {
                ScenePropertiesDialog* scenePropertiesDialog =
                        new ScenePropertiesDialog(sceneData, (int*)&gridSize, (QWidget*)parent());
                scenePropertiesDialog->setFocus();
                scenePropertiesDialog->show();

                SceneEditor* sceneEditor = (SceneEditor*)parent();
                QTabWidget* tabWidget = (QTabWidget*)parent()->parent()->parent();
                tabWidget->setTabText(tabWidget->indexOf(sceneEditor), sceneEditor->scene->name + "*");
                break;
            }

            case sf::Keyboard::E:
                mode = Mode::Entity;
                break;

            case sf::Keyboard::Q:
                // Disable tile mode for now
                //mode = Mode::Tile;
                break;

            case sf::Keyboard::Z:
                if (mode == Mode::Entity)
                {
                    if (entityOffset > 0)
                        --entityOffset;
                    else
                        entityOffset = sceneEntities.size() - 1;
                }
                else if (mode == Mode::Tile)
                {
                    if (tilesetOffset > 0)
                        --tilesetOffset;
                    else
                        tilesetOffset = sceneTextures.size() - 1;
                }
                break;

            case sf::Keyboard::X:
                if (mode == Mode::Entity)
                {
                    if (entityOffset < sceneEntities.size() - 1)
                        ++entityOffset;
                    else
                        entityOffset = 0;
                }
                else
                {
                    if (tilesetOffset < sceneTextures.size() - 1)
                        ++tilesetOffset;
                    else
                        tilesetOffset = 0;
                }
                break;
        }
    }

public:
    SceneEditorView(SceneData::SceneData* sceneData, QWidget* parent)
        : QSFMLCanvas(parent, QPoint(0, 0), QSize(640, 480), 1000 / 60),
          sceneData(sceneData),
          sceneEntities(),
          bgGrid(),
          fontBuffer(),
          uiFont(),
          mode(Mode::View),
          entityOffset(0),
          tilesetOffset(0),
          mouseDelta(),
          dragging(false),
          showGrid(true),
          gridSize(24)
    {
        for(auto& texture : Project.textures->textures)
            sceneTextures.push_back(SceneTexture(texture.name));

        for(Script& script : Project.scripts)
        {
			SceneEntity entity(script.GetPath());
            if (entity.name != "")
                sceneEntities.push_back(entity);
        }
    }
};

SceneEditor::SceneEditor(AssetLink* assetLink, QWidget* parent)
    : AssetForm(assetLink, parent),
      editorView(nullptr),
      frameTarget(nullptr),
      gridLayoutTarget(nullptr),
      scene(dynamic_cast<Scene*>(GetAssetLink()->asset))
{
    // Reserve asset
    GetAssetLink()->opened = true;
    SetupUi();
}

SceneEditor::~SceneEditor()
{
    delete editorView;
    delete frameTarget;
    delete gridLayoutTarget;
}

void SceneEditor::SetupUi()
{
    frameTarget = new QFrame(this);
        frameTarget->setFrameShape(QFrame::StyledPanel);
        frameTarget->setFrameShadow(QFrame::Raised);
    gridLayoutTarget = new QGridLayout(frameTarget);
        gridLayoutTarget->setSpacing(0);
        gridLayoutTarget->setContentsMargins(0, 0, 0, 0);

    setLayout(gridLayoutTarget);

    editorView = new SceneEditorView(&scene->sceneData, frameTarget);
    editorView->show();
    gridLayoutTarget->addWidget(editorView);
    editorView->setFocus(Qt::OtherFocusReason);
}

void SceneEditor::Save()
{
    // Save scene
    scene->sceneData.Save(scene->sceneData.filename);

    // We're no longer dirty
    SetModified(false);
}
