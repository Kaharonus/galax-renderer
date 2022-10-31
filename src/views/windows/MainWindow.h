
#pragma once
#include "../../renderer/QRenderer.h"
#include "RenderOptions.h"
#include <QMainWindow>
#include <QSurfaceFormat>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

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


    //std::shared_ptr<MapGenerator::Renderer::Scene> createScene();
    //void createSpheres(std::shared_ptr<MapGenerator::Renderer::SceneObjects::Node>& root, std::shared_ptr<MapGenerator::Renderer::SceneObjects::Camera>& camera);
    Ui::MainWindow* ui;
};
