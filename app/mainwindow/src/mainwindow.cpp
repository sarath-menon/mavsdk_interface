#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

// logs string to console
void MainWindow::console_log(const std::string &msg) {
  auto msg_qt = QString::fromStdString(msg);
  ui->console->append(msg_qt);
}

void MainWindow::on_arm_btn_clicked() { this->arm(); }

void MainWindow::on_disarm_btn_clicked() { this->disarm(); }

void MainWindow::on_takeoff_btn_clicked() {
  this->arm();
  this->takeoff();
}

void MainWindow::on_land_btn_clicked() {
  this->offboard_disable();
  this->land();
}

// Connect to mavsd instance selected in dropdown
void MainWindow::on_connect_btn_clicked() { this->connect(); }

void MainWindow::on_offboard_start_btn_clicked() { this->offboard_enable(); }

void MainWindow::on_offboard_stop_btn_clicked() { this->offboard_disable(); }

void MainWindow::on_mode_selector_currentIndexChanged(int index) {

  switch (index) {
  case 0:
    console_log("No offboard mode selected");
    break;
  case 1:
    console_log("Offboard mode: Circle forever selected");
    break;
  case 2:
    console_log("Offboard mode: Lemnicate forever selected");
    break;
  case 3:
    console_log("Offboard mode: External position control selected");
    break;
  default:
    exit(0);
  }
}
