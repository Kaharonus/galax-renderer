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

NodeControl::NodeControl(std::shared_ptr<INode> node, QWidget *parent) : QWidget(parent), ui(new Ui::NodeControl) {
    ui->setupUi(this);
    this->node = node;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NodeControl::update);
    timer->start(1000/60);
}


void NodeControl::update(){
    auto x = this->node->getPosition().x;
    auto y = this->node->getPosition().y;
    auto z = this->node->getPosition().z;
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

    x = this->node->getRotation().x;
    y = this->node->getRotation().y;
    z = this->node->getRotation().z;

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

    x = this->node->getScale().x;
    y = this->node->getScale().y;
    z = this->node->getScale().z;


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
