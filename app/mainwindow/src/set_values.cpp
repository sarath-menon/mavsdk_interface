#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "safety_checks.h"
#include <yaml-cpp/yaml.h>

void MainWindow::set_options(const std::string path) {

  // Safety check, see if file exists
  safety_checks::yaml_file_check(path);

  // Load yaml file containing gains
  YAML::Node options_yaml = YAML::LoadFile(path);

  // load port addresses
  address.mac_xbee =
      QString::fromStdString(options_yaml["mac_xbee_port"].as<std::string>());
  address.linux_xbee =
      QString::fromStdString(options_yaml["linux_xbee_port"].as<std::string>());
  address.px4_sim =
      QString::fromStdString(options_yaml["px4_sim_port"].as<std::string>());
}
