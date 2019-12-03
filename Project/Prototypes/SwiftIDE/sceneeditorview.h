#ifndef SCENEEDITORVIEW_H
#define SCENEEDITORVIEW_H

#include <SFML/Graphics.hpp>
#include "qsfmlcanvas.h"

class SceneEditorView : public QSFMLCanvas
{
private:

protected:
    void OnInit();
    void OnUpdate();

public:
    SceneEditorView(QWidget* parent, const QPoint& position, const QSize& size);
};

#endif // SCENEEDITORVIEW_H
