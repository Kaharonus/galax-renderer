//
// Created by tomas on 5.9.22.
//
#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>

#include "../../renderer/scene_objects/Texture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextureControl; }
QT_END_NAMESPACE

using namespace Galax::Renderer::SceneObjects;

class TextureControl : public QWidget {
Q_OBJECT

public:
    explicit TextureControl(QWidget *parent = nullptr);
    TextureControl(std::shared_ptr<Texture> texture, QWidget *parent = nullptr);

    ~TextureControl() override;

protected slots:
    void update();

private:
    std::shared_ptr<Texture> texture;

    QTimer* timer;
    QGraphicsScene* imageScene;

    std::vector<unsigned char> data;

    Ui::TextureControl *ui;

    void init();
};
