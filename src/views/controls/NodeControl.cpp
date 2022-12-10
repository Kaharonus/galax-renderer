//
// Created by tomas on 5.9.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NodeControl.h" resolved

#include <views/controls/NodeControl.h>
#include "ui_NodeControl.h"
#include <views/controls/QDraggableDoubleSpinBox.h>


NodeControl::NodeControl(QWidget *parent) : QWidget(parent), ui(new Ui::NodeControl) {
    ui->setupUi(this);
}

NodeControl::NodeControl(std::shared_ptr<IRenderNode> node, QWidget *parent) : QWidget(parent), ui(new Ui::NodeControl) {
    ui->setupUi(this);
    this->node = node;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NodeControl::update);
    timer->start(1000/60);
}


void NodeControl::update(){
    auto x = this->node->getRelativePosition().x;
    auto y = this->node->getRelativePosition().y;
    auto z = this->node->getRelativePosition().z;
    if (!ui->x->hasFocus()) {
        ui->x->setValue(x);
    }
    if (!ui->y->hasFocus()) {
        ui->y->setValue(y);
    }
    if (!ui->z->hasFocus()) {
        ui->z->setValue(z);
    }
    node->setPosition(glm::vec3(ui->x->value(), ui->y->value(), ui->z->value()));

    x = this->node->getRelativeRotation().x;
    y = this->node->getRelativeRotation().y;
    z = this->node->getRelativeRotation().z;

    if (!ui->xRotation->hasFocus()) {
        ui->xRotation->setValue(x);
    }
    if (!ui->yRotation->hasFocus()) {
        ui->yRotation->setValue(y);
    }
    if (!ui->zRotation->hasFocus()) {
        ui->zRotation->setValue(z);
    }
    node->setRotation(glm::vec3(ui->xRotation->value(), ui->yRotation->value(), ui->zRotation->value()));

    x = this->node->getRelativeScale().x;
    y = this->node->getRelativeScale().y;
    z = this->node->getRelativeScale().z;


    if (!ui->xScale->hasFocus()) {
        ui->xScale->setValue(x);
    }
    if (!ui->yScale->hasFocus()) {
        ui->yScale->setValue(y);
    }
    if (!ui->zScale->hasFocus()) {
        ui->zScale->setValue(z);
    }
    node->setScale(glm::vec3(ui->xScale->value(), ui->yScale->value(), ui->zScale->value()));

    node->drawAsWireframe(ui->wireframe->isChecked());

}

NodeControl::~NodeControl() {
    delete ui;
}
