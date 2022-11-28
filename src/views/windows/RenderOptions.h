
#pragma once

#include "../../renderer/Scene.h"
#include "../../renderer/LightingModel.h"
#include "../../renderer/impl/PostProcessEffect.h"

#include <QCodeEditor>
#include <QMainWindow>
#include <QOpenGLContext>
#include <memory>
#include <QGridLayout>
#include <QTreeWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RenderOptions;
}
QT_END_NAMESPACE
using namespace Galax::Renderer;
class RenderOptions : public QMainWindow {
    Q_OBJECT

public:
    explicit RenderOptions(QWidget* parent = nullptr);

    void setScene(std::shared_ptr<Galax::Renderer::Scene> scene,
                  std::shared_ptr<Galax::Renderer::LightingModel> lighting,
                  const std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>>& effects
                  );

    ~RenderOptions() override;

protected slots:

    void onItemClicked();

	void onTabCloseRequest(int index);

private:
    Ui::RenderOptions* ui;

    std::shared_ptr<Galax::Renderer::Scene> scene;
    std::shared_ptr<Galax::Renderer::LightingModel> lighting;
    std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>> effects;

    QTimer* fpsTimer;
    // Renderer* renderer;
    QLayout* baseGridLayout;
    QTreeWidget* nodeView;
	QTabWidget* dataView;
    std::shared_ptr<IShader> currentShader;

    void addLighting(std::shared_ptr<Galax::Renderer::LightingModel> sharedPtr);

    void addEffects(const std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>> &effects);
    void addNode(std::shared_ptr<IRenderNode> node, QTreeWidgetItem* parent);

    std::string demangle(const char* name);


    template <class T> std::string type(const T& t) {
        return demangle(typeid(t).name());
    }


	std::vector<std::tuple<std::shared_ptr<SceneObject>, QWidget*>> tabs;

	void addTab(QWidget *widget, std::shared_ptr<SceneObject> object);
};
