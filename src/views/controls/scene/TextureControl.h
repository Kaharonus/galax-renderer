//
// Created by tomas on 5.9.22.
//
#include <QWidget>
#include <QTimer>
#include <QGraphicsScene>

#include "renderer/impl/Texture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextureControl; }
QT_END_NAMESPACE

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Renderer;
class TextureControl : public QWidget {
Q_OBJECT

public:
    explicit TextureControl(QWidget *parent = nullptr);
    TextureControl(std::shared_ptr<ITexture> texture, QWidget *parent = nullptr);

    ~TextureControl() override;

protected slots:
    void update();

private:
    std::shared_ptr<ITexture> texture;

    QTimer* timer;
    QGraphicsScene* imageScene;

    std::vector<unsigned char> data;

    Ui::TextureControl *ui;

    void init();


    std::vector<unsigned char>
    convertData(std::vector<unsigned char> data, Texture::DataType type, Texture::Format format);
    bool gotData = false;
};
