#include <iostream>
#include "sceneeditorview.h"

SceneEditorView::SceneEditorView(QWidget* parent, const QPoint& position, const QSize& size)
    : QSFMLCanvas(parent, position, size, 1000 / 60)
{

}

void SceneEditorView::OnInit()
{
    std::cout << "Initialize SceneEditorView()" << std::endl;
}

void SceneEditorView::OnUpdate()
{
    RenderWindow::clear(sf::Color(100, 149, 237));

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text("Check out my sick SFML canvas.", font);
    text.move(sf::Vector2f(64.0f, 64.0f));
    RenderWindow::draw(text);
}
