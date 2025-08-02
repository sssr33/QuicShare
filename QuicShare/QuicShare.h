#pragma once
#include "ui_QuicShare.h"

#include <QtWidgets/QMainWindow>

class QuicShare : public QMainWindow {
    Q_OBJECT
public:
    QuicShare(QWidget *parent = nullptr);
    ~QuicShare();

private:
    Ui::QuicShareClass ui;
};
