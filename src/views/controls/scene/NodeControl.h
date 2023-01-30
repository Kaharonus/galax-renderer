//
// Created by tomas on 5.9.22.
//


#include "renderer/interfaces/IRenderNode.h"
#include <memory>
#include <QWidget>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class NodeControl; }
QT_END_NAMESPACE
using namespace Galax::Renderer;
class NodeControl : public QWidget {

Q_OBJECT

public:
    explicit NodeControl(QWidget *parent = nullptr);
    NodeControl(std::shared_ptr<IRenderNode> node, QWidget *parent = nullptr);

    ~NodeControl() override;

protected slots:
    void update();

private:

    std::shared_ptr<IRenderNode> node;
    QTimer* timer;

    Ui::NodeControl *ui;
};

