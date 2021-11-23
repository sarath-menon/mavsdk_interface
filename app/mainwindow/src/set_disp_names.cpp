

#include "mainwindow.h"
#include "safety_checks.h"
#include <yaml-cpp/yaml.h>

void MainWindow::set_disp_names() {

  // set px4 port display names
  disp_name.mac_xbee = "xbee conected to mac";
  disp_name.linux_xbee = "xbee conected to linux";
  disp_name.px4_sim = "px4 native simulation (gazebo,jmavsim etc)";

  // map display name to actual port names
  px4_port[disp_name.mac_xbee] = address.mac_xbee;
  px4_port[disp_name.linux_xbee] = address.linux_xbee;
  px4_port[disp_name.px4_sim] = address.px4_sim;
}