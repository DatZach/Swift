#include <iostream>
#include "qsfmlcanvas.h"

QSFMLCanvas::QSFMLCanvas(QWidget *parent, const QPoint& position,
                         const QSize& size, unsigned int frameTime)
    : QWidget(parent),
      timer(),
      initialized(false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be recieved
    setFocusPolicy(Qt::StrongFocus);

    // Set up widget geometry
    move(position);
    resize(size);

    // Initialize timer
    timer.setInterval(frameTime);
}

QSFMLCanvas::~QSFMLCanvas()
{

}

void QSFMLCanvas::OnInit()
{

}

void QSFMLCanvas::OnUpdate()
{

}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (initialized)
        return;

    // Create the SFML window with the widget handle
    RenderWindow::create((sf::WindowHandle)winId());

    // Let the derived class do its specific initialization
    OnInit();

    // Setup the timer to trigger at the specified framerate
    connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer.start();

    initialized = true;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    OnUpdate();
    RenderWindow::display();
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}
