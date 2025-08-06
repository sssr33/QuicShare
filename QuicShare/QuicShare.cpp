#include "QuicShare.h"
#include "Log/Log.h"

QuicShare::QuicShare(QWidget *parent)
    : QMainWindow(parent)
    , ioContextThread(&QuicShare::IoContextThreadMain, this)
{
    ui.setupUi(this);

    Log::Init("logs/log.txt", ui.textEditLog);

    LOG_INFO("Welcome to spdlog!");
    LOG_ERROR("Some error message with arg: {}", 1);

    connect(ui.startButton, &QPushButton::clicked, this, &QuicShare::OnStartClicked);

    {
        boost::asio::ip::udp::resolver resolver(ioContext);
        auto it = resolver.resolve(boost::asio::ip::host_name(), "", boost::asio::ip::resolver_base::flags::passive);

        for (const auto& e : it) {
            auto addr = e.endpoint().address();
            auto str = addr.to_string();
            auto isv4 = addr.is_v4();
            auto isLoopback = addr.is_loopback();

            if (!addr.is_loopback()) {
                auto lnd = std::make_unique<LocalNetworkDiscovery>(ioContext, addr);
                localNetworkDiscovery.push_back(std::move(lnd));
            }
        }
    }
}

QuicShare::~QuicShare()
{}

void QuicShare::OnStartClicked() {
    LOG_INFO("start clicked");

    for (auto& lnd : localNetworkDiscovery) {
        lnd->Test();
    }
}

void QuicShare::IoContextThreadMain() {
    auto work_guard = boost::asio::make_work_guard(ioContext);
    ioContext.run();
    int stop = 234;
}
