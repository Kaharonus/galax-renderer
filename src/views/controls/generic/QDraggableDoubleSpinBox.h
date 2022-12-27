//
// Created by tomas on 8.11.22.
//


#pragma once


#include <QDoubleSpinBox>
#include <QApplication>
#include <QMouseEvent>
#include <iostream>

class QDraggableDoubleSpinBox : public QDoubleSpinBox {
Q_OBJECT
public:
    explicit QDraggableDoubleSpinBox(QWidget *parent = nullptr) {
        this->setMinimum(-1000000);
        this->setMaximum(1000000);
        this->setSingleStep(0.01);
    };
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override {
        if (!(event->buttons() & Qt::MiddleButton)){
            return QDoubleSpinBox::mouseMoveEvent(event);
        }

        if (!IsDragging) {
            StartDragPos = QCursor::pos();
            IsDragging = true;
            QApplication::setOverrideCursor(Qt::BlankCursor);
        } else {
            int dragDist = QCursor::pos().y() - StartDragPos.y();
            if (dragDist == 0){
                return;
            }

            double dragMultiplier = .05 * this->singleStep();
            if (!(event->modifiers() & Qt::ControlModifier))
                dragMultiplier *= 5.0;
            auto diff = (dragMultiplier * dragDist);

            this->setValue(this->value() - diff);
            QCursor::setPos(StartDragPos);
        }


    };
    virtual void mouseReleaseEvent(QMouseEvent *event) override{
        if (!IsDragging || event->button() != Qt::MiddleButton)
            return QDoubleSpinBox::mouseReleaseEvent(event);

        IsDragging = false;
        QApplication::restoreOverrideCursor();
    };



private:
    bool IsDragging = false;
    QPoint StartDragPos;

};
