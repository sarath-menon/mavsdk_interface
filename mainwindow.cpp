#include "mainwindow.h"
#include "./ui_mainwindow.h"

std::shared_ptr<System> get_system(Mavsdk &mavsdk) {
  std::cout << "Waiting to discover system...\n";
  auto prom = std::promise<std::shared_ptr<System>>{};
  auto fut = prom.get_future();

  // We wait for new systems to be discovered, once we find one that has an
  // autopilot, we decide to use it.
  mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
    auto system = mavsdk.systems().back();

    if (system->has_autopilot()) {
      std::cout << "Discovered autopilot\n";

      // Unsubscribe again as we only want to find one system.
      mavsdk.subscribe_on_new_system(nullptr);
      prom.set_value(system);
    }
  });

  // We usually receive heartbeats at 1Hz, therefore we should find a
  // system after around 3 seconds max, surely.
  if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
    std::cerr << "No autopilot found.\n";
    return {};
  }

  // Get discovered system now.
  return fut.get();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  mavsdk = std::make_unique<Mavsdk>();

  ConnectionResult connection_result =
      mavsdk->add_any_connection("udp://:14540");
}

MainWindow::~MainWindow() { delete ui; }
