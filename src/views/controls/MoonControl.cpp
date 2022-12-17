//
// Created by tomas on 16.12.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MoonControl.h" resolved

#include "MoonControl.h"
#include "ui_MoonControl.h"


MoonControl::MoonControl(QWidget *parent) : QWidget(parent), ui(new Ui::MoonControl) {
	ui->setupUi(this);
}

MoonControl::~MoonControl() {
	delete ui;
}

MoonControl::MoonControl(const std::shared_ptr<Galax::Orbital::Planet> moon, int number,
						 QWidget *parent) : QWidget(parent), ui(new Ui::MoonControl) {
	ui->setupUi(this);
	this->moon = moon;
	ui->nameText->setText(("Moon - " + std::to_string(number)).c_str());
}

void MoonControl::update() {
	if(!moon){
		return;
	}

	if(!ui->moonScaleInput->hasFocus()){
		ui->moonScaleInput->setValue(moon->getSize());
	}
	moon->setSize(ui->moonScaleInput->value());

	if(!ui->moonSeedInput->hasFocus()){
		ui->moonSeedInput->setValue(moon->getSeed());
	}
	if (abs(moon->getSeed() - this->ui->moonSeedInput->value()) > .5) {
		moon->setSeed(ui->moonSeedInput->value());
	}

	if(!ui->moonSpeedInput->hasFocus()){
		ui->moonSpeedInput->setValue(moon->getSpeed());
	}
	moon->setSpeed(ui->moonSpeedInput->value());

}
