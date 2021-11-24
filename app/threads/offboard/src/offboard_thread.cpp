#include "offboard_thread.h"
#include <qglobal.h>
#include <qthread.h>

OffboardThread::OffboardThread(DefaultParticipant *dp,
                               mavsdk::Offboard *offboard,
                               mavsdk::Telemetry *telemetry, QObject *parent)
    : QThread(parent) {

  // set thread flag to false
  threadflags::pos_pub = false;

  // Fastdds
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
  dp_ = dp;
  // Create fastdds objects

  // Create  subscriber
  cmd_sub = new DDSSubscriber(idl_msg::QuadPositionCmdPubSubType(),
                              &sub::pos_cmd, "pos_cmd", dp_->participant());

  // Create  subscriber
  pos_pub = new DDSPublisher(idl_msg::MocapPubSubType(), "mocap_pose",
                             dp_->participant());

  // initialize
  cmd_sub->init();
  pos_pub->init();
  // mavsdk ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  offboard_ = offboard;
  telemetry_ = telemetry;

  // Set intial position
  sub::pos_cmd.position.x = 0.0;
  sub::pos_cmd.position.y = 0.0;
  sub::pos_cmd.position.z = 1.5;
}

OffboardThread::~OffboardThread() { // Fastdds
  delete cmd_sub;
  delete pos_pub;
}

// publish position
void publish_position(mavsdk::Telemetry *telemetry) {
  while (!threadflags::pos_pub) {
    std::cout << "Attitude euler: " << telemetry->position_velocity_ned()
              << std::endl;

    // Publish at 100 Hz
    QThread::msleep(10);
  }
};

void OffboardThread::run() { // Blocks until new data is available

  auto pos_publisher =
      std::async(std::launch::async, &publish_position, telemetry_);

  forever {

    cmd_sub->listener->wait_for_data_for_ms(100);
    // Check if program close has been requeated. if so, leave loop
    if (QThread::currentThread()->isInterruptionRequested()) {

      threadflags::pos_pub = true;
      // wait
      QThread::msleep(10);

      return;
    }

    pos_msg.north_m = sub::pos_cmd.position.x + x_offset;
    pos_msg.east_m = sub::pos_cmd.position.y + y_offset;
    // To account for px4 -z coordinate system (North-East-Down)
    pos_msg.down_m = -sub::pos_cmd.position.z;

    offboard_->set_position_ned(pos_msg);

    // publish_position();

    // qDebug() << "X Position:" << sub::pos_cmd.position.x;
  }
}
