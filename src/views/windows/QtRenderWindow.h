
#pragma once
#include "views/controls/generic/QRenderer.h"
#include <views/windows/MainWindow.h>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <physics/PhysicsEngine.h>
#include <orbital/Planet.h>
#include <physics/interfaces/IRigidBody.h>

#include <assets/SceneLoader.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class QtRenderWindow;
}
QT_END_NAMESPACE

using namespace Galax::Physics;

class QtRenderWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit QtRenderWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    ~QtRenderWindow() override;

private:
	bool pause = true;
    void setupRenderer(const QSurfaceFormat &format);
    void loadScene(const std::string& path);

	std::shared_ptr<Galax::Assets::SceneLoader> sceneLoader;

    MainWindow* renderOptionsWindow;
    Galax::Renderer::QRenderer* renderer;
    Galax::Physics::PhysicsEngine* physicsEngine;
    std::shared_ptr<InputHandler> inputHandler;
	QTimer* engineUpdate;

	QTimer* refreshTimer;
	std::shared_ptr<Galax::Orbital::Planet> selectedPlanet;

    Ui::QtRenderWindow* ui;

    void setupPhysics();
};
