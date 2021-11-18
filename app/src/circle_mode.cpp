#include "fastdds_thread.h"
#include <qglobal.h>
#include <qthread.h>

void fastdds_thread::circle_forever() { // Wait ill subscriber receives

  // Step size in degrees
  constexpr static float step_size = 0.1;
  // center angle in degrees
  float theta = 0;
  // Radius of circle to be tracked (in meters)
  constexpr static float radius = 1.5;
  // Local center
  constexpr static float center_x = 0;
  constexpr static float center_y = 0;
  // constant altitude
  constexpr static float const_altitude = 1.5;

  // Go to circle start point
  pos_msg.down_m = -const_altitude;
  pos_msg.north_m = 1;
  offboard_->set_position_ned(pos_msg);

  // Delay for quad to catch up
  std::this_thread::sleep_for(std::chrono::seconds(5));

  // Start circle
  for (int i = 0; i < 360; i++) {

    // COmpute x,y coordinates
    // std::cout << "Timestep:" << i << '\n';
    pos_msg.north_m = center_x + radius * cos(theta);
    pos_msg.east_m = center_y + radius * sin(theta);

    theta += step_size;

    // Delay for quad to catch up
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    offboard_->set_position_ned(pos_msg);
  }
}