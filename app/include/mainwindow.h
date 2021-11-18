#pragma once
#include "include_helper.h"

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

  void on_initialize_btn_clicked();

  void on_offboard_start_btn_clicked();

  void on_offboard_stop_btn_clicked();

public:
  void console_log(const std::string &msg);

private:
  Ui::MainWindow *ui;

  // Pointer to mavsdk objects
  std::unique_ptr<Mavsdk> mavsdk;
  std::shared_ptr<System> system;
  std::unique_ptr<Telemetry> telemetry;
  std::unique_ptr<Action> action;
  std::unique_ptr<Offboard> offboard;

  // List of ports to connect to px4
  const QString xbee_mac = "serial:///dev/tty.usbserial-D309S1F2";
  const QString xbee_ubuntu = "serial:///dev/ttyUSB0";
  const QString px4_simulator = "udp://:14540";

  // List of offboard modes
  const QString circle_mode = "fly circles forever";
  const QString lemniscate_mode = "fly a lemniscate forever";
  const QString external_pos_control_mode = "external position control";

  // fastdds objects
private:
  // Create doamin participant
  std::unique_ptr<DefaultParticipant> dp;

  // Motion capture data subscriber
  DDSSubscriber<idl_msg::QuadPositionCmdPubSubType, cpp_msg::QuadPositionCmd>
      *cmd_sub;

private:
  std::shared_ptr<System> get_system(Mavsdk &mavsdk);
};
