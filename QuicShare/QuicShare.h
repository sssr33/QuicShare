#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QuicShare.h"

class QuicShare : public QMainWindow
{
    Q_OBJECT

public:
    QuicShare(QWidget *parent = nullptr);
    ~QuicShare();

private:
    Ui::QuicShareClass ui;
};

