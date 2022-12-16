
#pragma once
#include <renderer/QRenderer.h>
#include <views/windows/RenderOptions.h>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <physics/PhysicsEngine.h>
#include <orbital/Planet.h>
#include <physics/interfaces/IRigidBody.h>

#include <assets/SceneLoader.h>
#include <QTimer>
#include <views/controls/MoonControl.h>

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
    void loadScene(const std::string& path);

	std::shared_ptr<Galax::Assets::SceneLoader> sceneLoader;

    RenderOptions* renderOptionsWindow;
    Galax::Renderer::QRenderer* renderer;
    Galax::Physics::PhysicsEngine* physicsEngine;
    std::shared_ptr<InputHandler> inputHandler;


	QTimer* refreshTimer;
	std::shared_ptr<Galax::Orbital::Planet> selectedPlanet;
	std::vector<MoonControl*> moonControls;

    Ui::MainWindow* ui;

    void setupPhysics();

	std::vector<std::shared_ptr<IRigidBody>> getPhysical(const std::shared_ptr<IRenderNode> &node);
	std::vector<std::shared_ptr<IRigidBody>> getPhysical(std::shared_ptr<Scene> scene);

	void uiRefresh();
};
