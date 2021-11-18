#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_arm_btn_clicked();

  void on_disarm_btn_clicked();

  void on_takeoff_btn_clicked();

  void on_land_btn_clicked();

private:
  Ui::MainWindow *ui;

  // Pointer to mavsdk objects
  std::unique_ptr<Mavsdk> mavsdk;
  std::shared_ptr<System> system;
  std::unique_ptr<Telemetry> telemetry;
  std::unique_ptr<Action> action;

  // List of ports to connect to px4
  const QString xbee_mac = "serial:///dev/tty.usbserial-D309S1F2";
  const QString xbee_ubuntu = "serial:///dev/ttyUSB0";
  const QString px4_simulator = "udp://:14540";
};
#endif // MAINWINDOW_H
