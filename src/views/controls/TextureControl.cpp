//
// Created by tomas on 5.9.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TextureControl.h" resolved

#include "TextureControl.h"
#include "ui_TextureControl.h"
#include "../../Extensions.h"
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>


TextureControl::TextureControl(QWidget *parent) : QWidget(parent), ui(new Ui::TextureControl) {
    ui->setupUi(this);
    init();
}

TextureControl::TextureControl(std::shared_ptr<Texture> texture, QWidget *parent) : QWidget(parent),
                                                                                    ui(new Ui::TextureControl) {
    ui->setupUi(this);
    this->texture = texture;
    init();
}

void TextureControl::init() {
    this->timer = new QTimer(this);
    timer->setInterval(1000 / 15);
    connect(timer, &QTimer::timeout, this, &TextureControl::update);
    timer->start();

    imageScene = new QGraphicsScene(this);
    ui->imageView->setScene(imageScene);
    ui->imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

TextureControl::~TextureControl() {
    delete ui;
}

void TextureControl::update() {
    auto dataType = texture->getDataType();
    auto format = texture->getFormat();
    auto textureType = texture->getType();

    auto supportedTypes = {Texture::TYPE_2D};
    auto supportedFormats = {Texture::RGB, Texture::RGBA};
    auto supportedDataTypes = {Texture::UNSIGNED_BYTE, Texture::FLOAT, Texture::BYTE};

    if (!contains(supportedTypes, textureType) || !contains(supportedFormats, format) ||
        !contains(supportedDataTypes, dataType)) {
        return;
    }


    texture->requestData([&](const std::vector<unsigned char> &data) {
        this->data = data;
    });

    auto qFormat = format == Texture::RGBA ? QImage::Format_RGBA8888 : QImage::Format_RGB888;
    imageScene->clear();
    auto [w, h, d] = texture->getDimensions();
    if (data.empty()) {
        return;
    }


    auto converted = convertData(data, dataType, format);
    auto image = QImage(converted.data(), w, h, qFormat);
    if (image.isNull()) {
        return;
    }
    image = image.scaled(ui->imageView->width(), ui->imageView->height(), Qt::KeepAspectRatio);
    auto item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    imageScene->addItem(item);
}


std::vector<unsigned char>
TextureControl::convertData(std::vector<unsigned char> data, Texture::DataType type, Texture::Format format) {
    if (type == Texture::UNSIGNED_BYTE || type == Texture::BYTE) {
        return data;
    }
    auto [w, h, d] = texture->getDimensions();
    auto size = w * h * texture->getDataSize() * texture->getFormatSize();
    auto result = std::vector<unsigned char>(size);
    if(type == Texture::FLOAT){
        auto fResult = reinterpret_cast<float*>(data.data());
        for(int i = 0; i < size; i++){
            auto tmp = (unsigned char)(fResult[i/texture->getDataSize()] * 255);
            result[i] = tmp;
        }
    }
    return result;
}
