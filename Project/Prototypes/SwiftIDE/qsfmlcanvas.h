#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
    //Q_OBJECT

private:
    QTimer timer;
    bool initialized;

protected:
    virtual void OnInit();
    virtual void OnUpdate();

public:
    explicit QSFMLCanvas(QWidget *parent, const QPoint& position,
                         const QSize& size, unsigned int frameTime = 0);
    virtual ~QSFMLCanvas();

    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);

    virtual QPaintEngine* paintEngine() const;

signals:

public slots:

};

#endif // QSFMLCANVAS_H
