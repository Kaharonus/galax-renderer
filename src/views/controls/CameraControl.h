
#pragma once

#include <QTimer>
#include <QWidget>
#include <memory>
#include <qwidget.h>
#include "../../renderer/impl/Camera.h"

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Renderer;
QT_BEGIN_NAMESPACE
namespace Ui {
    class CameraControl;
}
QT_END_NAMESPACE


class CameraControl : public QWidget {
    Q_OBJECT
public:
    CameraControl();
    CameraControl(std::shared_ptr<ICamera> camera, QWidget* parent);
    ~CameraControl();

protected slots:
    void update();

private:
    Ui::CameraControl* ui;

    std::shared_ptr<ICamera> camera;
    QTimer* timer;
};
