
#pragma once
#include <renderer/QRenderer.h>
#include <views/windows/RenderOptions.h>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <physics/PhysicsEngine.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

using namespace Galax::Physics;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const QSurfaceFormat &format, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    ~MainWindow() override;

private:
    void setupRenderer(const QSurfaceFormat &format);
    void loadScene();

    RenderOptions* renderOptionsWindow;
    Galax::Renderer::QRenderer* renderer;
    Galax::Physics::PhysicsEngine* physicsEngine;


    Ui::MainWindow* ui;

    void setupPhysics();
};
