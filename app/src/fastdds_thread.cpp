#include "fastdds_thread.h"
#include <qglobal.h>
#include <qthread.h>

fastdds_thread::fastdds_thread(std::unique_ptr<mavsdk::Offboard> offboard,
                               QObject *parent)
    : QThread(parent) {

  // Fastdds ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

  // Create fastdds objects
  // Create domain participant
  dp = std::make_unique<DefaultParticipant>(0, "mavsdk_gui_interface");

  // Create  subscriber
  cmd_sub = new DDSSubscriber(idl_msg::QuadPositionCmdPubSubType(),
                              &sub::pos_cmd, "pos_cmd", dp->participant());

  // initialize  subscriberDefaultParticipant
  cmd_sub->init();
  // mavsdk ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  offboard_ = std::move(offboard);

  // Set intial position
  sub::pos_cmd.position.x = 0.0;
  sub::pos_cmd.position.y = 0.0;
  sub::pos_cmd.position.z = 1.5;
}

fastdds_thread::~fastdds_thread() { // Fastdds
  delete cmd_sub;
}

void fastdds_thread::run() { // Blocks until new data is available

  forever {

    // Check if program close has been requeated. if so, leave loop
    if (QThread::currentThread()->isInterruptionRequested()) {
      return;
    }

    switch (offb_mode) {

    case offboard_mode::circle:
      circle_forever();
      break;

    case offboard_mode::lemniscate:
      lemniscate_forever();
      break;

    case offboard_mode::external:
      // Run external position control
      external_pos_control();
      break;

    default:
      exit(0);
    }

    // qDebug() << "X Position:" << sub::pos_cmd.position.x;
  }
}

std::unique_ptr<mavsdk::Offboard> fastdds_thread::return_offboard_obj() {
  return std::move(offboard_);
}

void fastdds_thread::external_pos_control() { // Wait ill subscriber receives
                                              // data
  cmd_sub->listener->wait_for_data_for_ms(100);
  pos_msg.north_m = sub::pos_cmd.position.x + x_offset;
  pos_msg.east_m = sub::pos_cmd.position.y + y_offset;
  // To account for px4 -z coordinate system (North-East-Down)
  pos_msg.down_m = -sub::pos_cmd.position.z;

  offboard_->set_position_ned(pos_msg);
}
