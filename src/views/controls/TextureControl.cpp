//
// Created by tomas on 5.9.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TextureControl.h" resolved

#include "TextureControl.h"
#include "ui_TextureControl.h"


TextureControl::TextureControl(QWidget *parent) : QWidget(parent), ui(new Ui::TextureControl) {
    ui->setupUi(this);
    init();
}

TextureControl::TextureControl(std::shared_ptr<Texture> texture, QWidget *parent) : QWidget(parent), ui(new Ui::TextureControl) {
    ui->setupUi(this);
    this->texture = texture;
    init();
}

void TextureControl::init(){
    this->timer = new QTimer(this);
    timer->setInterval(1000/15);
    connect(timer, &QTimer::timeout, this, &TextureControl::update);
    timer->start();
}

TextureControl::~TextureControl() {
    delete ui;
}

void TextureControl::update() {


}
