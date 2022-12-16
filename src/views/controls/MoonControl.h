//
// Created by tomas on 16.12.22.
//

#ifndef GALAX_RENDER_MOONCONTROL_H
#define GALAX_RENDER_MOONCONTROL_H

#include <QWidget>
#include <orbital/Planet.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MoonControl; }
QT_END_NAMESPACE

class MoonControl : public QWidget {
Q_OBJECT


public:

	MoonControl(const std::shared_ptr<Galax::Orbital::Planet> moon, int number ,QWidget *parent);
	void update();

	explicit MoonControl(QWidget *parent = nullptr);

	~MoonControl() override;

private:
	Ui::MoonControl *ui;

	std::shared_ptr<Galax::Orbital::Planet> moon;
};


#endif //GALAX_RENDER_MOONCONTROL_H
