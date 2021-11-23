#pragma once

// type of px4 flightmode
enum class FlightMode { takeoff, land, offboard };

// px4 connection ports
struct Port {
  QString mac_xbee{};
  QString linux_xbee{};
  QString px4_sim{};
} inline disp_name, address;

//