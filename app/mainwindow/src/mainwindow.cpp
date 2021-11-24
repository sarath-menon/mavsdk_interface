#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // set options list
  this->set_options("app/parameters/port_addresses.yaml");
  this->set_disp_names();
  this->set_disp_options();

  // setup console logging
  this->setup_console_logging();
}

MainWindow::~MainWindow() {
  // // Stop position publisher thread
  // pos_pub_thread->quit();
  // pos_pub_thread->requestInterruption();
  // pos_pub_thread->wait();

  delete ui;
}

// logs string to console
void MainWindow::console_log(const std::string &msg) {
  auto msg_qt = QString::fromStdString(msg);
  ui->console->append(msg_qt);
}

//////////////////////////////////////////////////////////////////
// GUI button click events
//////////////////////////////////////////////////////////////////

// Connect to mavsd instance selected in dropdown
void MainWindow::on_connect_btn_clicked() { this->connect(); }

void MainWindow::on_arm_btn_clicked() {
  if (connected) {
    this->arm();
  }
}

void MainWindow::on_disarm_btn_clicked() {
  if (connected) {
    this->disarm();
  }
}

void MainWindow::on_takeoff_btn_clicked() {
  if (connected) {
    this->arm();
    this->takeoff();
  }
}

void MainWindow::on_land_btn_clicked() {
  if (connected) {
    this->offboard_disable();
    this->land();
  }
}

void MainWindow::on_offboard_start_btn_clicked() {
  if (connected) {
    this->offboard_enable();
  }
}

void MainWindow::on_offboard_stop_btn_clicked() {
  if (connected) {
    this->offboard_disable();
  }
}

void MainWindow::on_mode_selector_currentIndexChanged(int index) {
  if (connected) {
    switch (index) {
    case 0:
      console_log("No offboard mode selected");
      myProcess->kill();
      break;
    case 1:
      console_log("Offboard mode: Circle forever selected");
      // start my process
      myProcess = std::make_unique<QProcess>(parent);
      myProcess->start(program, arguments);
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
}
