#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <mavsdk/log_callback.h>

std::shared_ptr<System> get_system(Mavsdk &mavsdk) {
  qDebug() << "Waiting to discover system...\n";
  auto prom = std::promise<std::shared_ptr<System>>{};
  auto fut = prom.get_future();

  // We wait for new systems to be discovered, once we find one that has an
  // autopilot, we decide to use it.
  mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
    auto system = mavsdk.systems().back();

    if (system->has_autopilot()) {
      qDebug() << "Discovered autopilot\n";

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

  QObject::connect(this, &MainWindow::msg_changed, this,
                   &MainWindow::console_log);

  // Custom logger that logs to app console
  mavsdk::log::subscribe(
      [this](mavsdk::log::Level level,   // message severity level
             const std::string &message, // message text
             const std::string
                 &file,  // source file from which the message was sent
             int line) { // line number in the source file
        // process the log message in a way you like
        qDebug() << QString::fromStdString(message);

        // log msg to console
        emit msg_changed(QString::fromStdString(message));

        // returning true from the callback disables printing the message to
        // stdout
        return level < mavsdk::log::Level::Warn;
      });

  // Add ports to combobox
  ui->port_selector->addItem(xbee_mac);
  ui->port_selector->addItem(xbee_ubuntu);
  ui->port_selector->addItem(px4_simulator);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_arm_btn_clicked() {
  qDebug() << "Arming...\n";
  const Action::Result arm_result = action->arm();
  if (arm_result != Action::Result::Success) {
    qDebug() << "Arming failed: ";
  }
}

void MainWindow::on_disarm_btn_clicked() {
  qDebug() << "Taking off...\n";
  const Action::Result arm_result = action->disarm();
}

void MainWindow::on_takeoff_btn_clicked() {
  const Action::Result takeoff_result = action->takeoff();
  if (takeoff_result != Action::Result::Success) {
    qDebug() << "Takeoff failed";
  }
}

void MainWindow::on_land_btn_clicked() {
  qDebug() << "Landing...\n";

  const Action::Result land_result = action->land();
  if (land_result != Action::Result::Success) {
    qDebug() << "Land failed";
  }
}

// logs string to console
void MainWindow::console_log(const QString msg) { ui->console->append(msg); }

// Connect to mavsd instance selected in dropdown
void MainWindow::on_initialize_btn_clicked() {
  // Create mavsdk object
  mavsdk = std::make_unique<Mavsdk>();

  QString mavsdk_port = ui->port_selector->currentText();
  ConnectionResult connection_result =
      mavsdk->add_any_connection(mavsdk_port.toStdString());

  // Get pointer to system from mavsdk obj
  system = get_system(*mavsdk);

  // Error checking
  if (!system) {
    std::cerr << "Couldn't get system" << std::endl;
  }

  // Create telemetry object
  telemetry = std::make_unique<Telemetry>(system);
  action = std::make_unique<Action>(system);
}