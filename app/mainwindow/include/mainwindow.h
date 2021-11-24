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

  void on_connect_btn_clicked();

  void on_offboard_start_btn_clicked();

  void on_offboard_stop_btn_clicked();

  void on_mode_selector_currentIndexChanged(int index);

private:
  void console_log(const std::string &msg);

private:
  Ui::MainWindow *ui;

  // Pointer to mavsdk objects
  std::unique_ptr<Mavsdk> mavsdk;
  std::shared_ptr<System> system;
  std::unique_ptr<Telemetry> telemetry;
  std::unique_ptr<Action> action;
  std::unique_ptr<Offboard> offboard;

  // List of offboard modes
  const QString circle_mode = "fly circles forever";
  const QString lemniscate_mode = "fly a lemniscate forever";
  const QString external_pos_control_mode = "external position control";

private:
  // maps port type enum to actual port
  std::map<QString, QString> px4_port{};

  // threads
private:
  std::unique_ptr<OffboardThread> offboard_thread;
  // std::unique_ptr<PositionPublisher> pos_pub_thread;

  // status flag
private:
  bool offb_enabled{};
  bool connected{};

  // mavsdk tasks
private:
  // connect to px4 and initialize all communication
  void connect();

  void arm();
  void disarm();

  void takeoff();
  void land();

  void offboard_enable();
  void offboard_disable();

  void setup_console_logging();
  void publish_position();
  std::shared_ptr<System> get_system(

      Mavsdk &mavsdk);

private:
  void set_options(const std::string path);
  void set_disp_options();
  void set_disp_names();

  // qprocess
private:
  QObject *parent;
  QString program = "/Users/sarathmenon/Desktop/eth_soft/code/Controllers/"
                    "reference_generator/build/apps/circle/circle";
  QStringList arguments;

  std::unique_ptr<QProcess> myProcess;
};
