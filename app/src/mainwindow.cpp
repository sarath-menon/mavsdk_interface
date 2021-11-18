#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <mavsdk/log_callback.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Custom logger that logs to app console
  mavsdk::log::subscribe(
      [this](mavsdk::log::Level level,   // message severity level
             const std::string &message, // message text
             const std::string
                 &file,  // source file from which the message was sent
             int line) { // line number in the source file
        // process the log message in a way you like
        console_log(message);

        // returning true from the callback disables printing the message to
        // stdout
        return level < mavsdk::log::Level::Warn;
      });

  // Add options to port selector combobox
  ui->port_selector->addItem(xbee_mac);
  ui->port_selector->addItem(xbee_ubuntu);
  ui->port_selector->addItem(px4_simulator);

  // Add options to offboard mode selector combobox
  ui->mode_selector->addItem(circle_mode);
  ui->mode_selector->addItem(lemniscate_mode);
  ui->mode_selector->addItem(external_pos_control_mode);
}

MainWindow::~MainWindow() { delete ui; }

std::shared_ptr<System> MainWindow::get_system(Mavsdk &mavsdk) {
  console_log("Waiting to discover system...");
  auto prom = std::promise<std::shared_ptr<System>>{};
  auto fut = prom.get_future();

  // We wait for new systems to be discovered, once we find one that has an
  // autopilot, we decide to use it.
  mavsdk.subscribe_on_new_system([this, &mavsdk, &prom]() {
    auto system = mavsdk.systems().back();

    if (system->has_autopilot()) {
      console_log("Discovered autopilot");

      // Unsubscribe again as we only want to find one system.
      mavsdk.subscribe_on_new_system(nullptr);
      prom.set_value(system);
    }
  });

  // We usually receive heartbeats at 1Hz, therefore we should find a
  // system after around 3 seconds max, surely.
  if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
    console_log("No autopilot found.");
    return {};
  }

  // Get discovered system now.
  return fut.get();
}

void MainWindow::on_arm_btn_clicked() {
  console_log("Arming...");
  const Action::Result arm_result = action->arm();
  if (arm_result != Action::Result::Success) {
    console_log("Arming failed: ");
  }
}

void MainWindow::on_disarm_btn_clicked() {
  console_log("Taking off..");
  const Action::Result arm_result = action->disarm();
}

void MainWindow::on_takeoff_btn_clicked() {
  const Action::Result takeoff_result = action->takeoff();
  if (takeoff_result != Action::Result::Success) {
    console_log("Takeoff failed");
  }
}

void MainWindow::on_land_btn_clicked() {
  console_log("Landing...");

  const Action::Result land_result = action->land();
  if (land_result != Action::Result::Success) {
    console_log("Land failed");
  }
}

// logs string to console

void MainWindow::console_log(const std::string &msg) {
  auto msg_qt = QString::fromStdString(msg);
  ui->console->append(msg_qt);
}

// Connect to mavsd instance selected in dropdown
void MainWindow::on_initialize_btn_clicked() {
  // Create mavsdk object
  mavsdk = std::make_unique<Mavsdk>();

  QString mavsdk_port = ui->port_selector->currentText();
  ConnectionResult connection_result =
      mavsdk->add_any_connection(mavsdk_port.toStdString());

  // Error checking
  if (connection_result != ConnectionResult::Success) {
    console_log("Connection failed:");
  }

  // Get pointer to system from mavsdk obj
  system = get_system(*mavsdk);

  // Error checking
  if (!system) {
    console_log("Couldn't get system");
  }

  // Create telemetry object
  telemetry = std::make_unique<Telemetry>(system);
  action = std::make_unique<Action>(system);
  offboard = std::make_unique<Offboard>(system);
}

void MainWindow::on_offboard_start_btn_clicked() {

  // Send it once before starting offboard, otherwise it will be rejected.
  const Offboard::VelocityNedYaw stay{};
  // Drone stays in place waiting for commands
  offboard->set_velocity_ned(stay);

  Offboard::Result offboard_result = offboard->start();
  if (offboard_result != Offboard::Result::Success) {
    console_log("Offboard start failed");
  } else {
    console_log("Offboard started");
  }
}

void MainWindow::on_offboard_stop_btn_clicked() {
  Offboard::Result offboard_result = offboard_result = offboard->stop();
  if (offboard_result != Offboard::Result::Success) {
    console_log("Offboard stop failed");
  } else {
    console_log("Offboard stopped");
  }
}
