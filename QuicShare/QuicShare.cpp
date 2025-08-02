#include "QuicShare.h"
#include "Log/Log.h"

QuicShare::QuicShare(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    Log::Init("logs/log.txt", ui.textEditLog);

    LOG_INFO("Welcome to spdlog!")
    LOG_ERROR("Some error message with arg: {}", 1)
}

QuicShare::~QuicShare()
{}
