#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "safety_checks.h"
#include <yaml-cpp/yaml.h>

void MainWindow::set_disp_options() {
  // Add options to port selector combobox
  ui->port_selector->addItem(disp_name.mac_xbee);
  ui->port_selector->addItem(disp_name.linux_xbee);
  ui->port_selector->addItem(disp_name.px4_sim);

  // Add options to offboard mode selector combobox
  ui->mode_selector->addItem(circle_mode);
  ui->mode_selector->addItem(lemniscate_mode);
  ui->mode_selector->addItem(external_pos_control_mode);
}