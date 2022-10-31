//
// Created by tomas on 5.9.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NodeControl.h" resolved

#include "NodeControl.h"
#include "ui_NodeControl.h"


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
    auto x = QString::number(this->node->getPosition().x);
    auto y = QString::number(this->node->getPosition().y);
    auto z = QString::number(this->node->getPosition().z);
    if (!ui->x->hasFocus()) {
        ui->x->setText(x);
    }
    if (!ui->y->hasFocus()) {
        ui->y->setText(y);
    }
    if (!ui->z->hasFocus()) {
        ui->z->setText(z);
    }
    node->setPosition(glm::vec3(ui->x->text().toFloat(), ui->y->text().toFloat(), ui->z->text().toFloat()));

    x = QString::number(this->node->getRotation().x);
    y = QString::number(this->node->getRotation().y);
    z = QString::number(this->node->getRotation().z);

    if (!ui->xRotation->hasFocus()) {
        ui->xRotation->setText(x);
    }
    if (!ui->yRotation->hasFocus()) {
        ui->yRotation->setText(y);
    }
    if (!ui->zRotation->hasFocus()) {
        ui->zRotation->setText(z);
    }
    node->setRotation(glm::vec3(ui->xRotation->text().toFloat(), ui->yRotation->text().toFloat(), ui->zRotation->text().toFloat()));

    x = QString::number(this->node->getScale().x);
    y = QString::number(this->node->getScale().y);
    z = QString::number(this->node->getScale().z);

    if (!ui->xScale->hasFocus()) {
        ui->xScale->setText(x);
    }
    if (!ui->yScale->hasFocus()) {
        ui->yScale->setText(y);
    }
    if (!ui->zScale->hasFocus()) {
        ui->zScale->setText(z);
    }
    node->setScale(glm::vec3(ui->xScale->text().toFloat(), ui->yScale->text().toFloat(), ui->zScale->text().toFloat()));

    node->drawAsWireframe(ui->wireframe->isChecked());

}

NodeControl::~NodeControl() {
    delete ui;
}
