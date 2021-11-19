#include "fastdds_thread.h"
#include <qglobal.h>
#include <qthread.h>

void fastdds_thread::lemniscate_forever() { // Wait ill subscriber receives

  // angle in radians
  float theta = 0;
  float theta_dot = 0.5;
  constexpr static float dt = 0.1;
  constexpr static int dt_ms = dt * 1000;
  constexpr static float a = 1.5;

  // constant altitude
  constexpr static float const_altitude = 1.5;

  // Go to lemniscate start point
  pos_msg.down_m = -const_altitude;
  pos_msg.north_m = a;
  offboard_->set_position_ned(pos_msg);

  // Delay for quad to catch up
  std::this_thread::sleep_for(std::chrono::seconds(5));

  // Start circle
  for (;;) {

    // COmpute x,y coordinates
    // std::cout << "Timestep:" << i << '\n';
    pos_msg.north_m = (a * cos(theta)) / (1.0 + pow(sin(theta), 2));
    pos_msg.east_m = (a * cos(theta) * sin(theta)) / (1.0 + pow(sin(theta), 2));

    // incrementing theta
    theta += theta_dot * dt;

    // Delay for quad to catch up
    std::this_thread::sleep_for(std::chrono::milliseconds(dt_ms));

    offboard_->set_position_ned(pos_msg);
  }
}