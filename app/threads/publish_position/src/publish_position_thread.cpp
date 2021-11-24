#include "publish_position_thread.h"
#include <qglobal.h>
#include <qthread.h>

PositionPublisher::PositionPublisher(
    DefaultParticipant *dp, std::unique_ptr<mavsdk::Telemetry> telemetry,
    QObject *parent)
    : QThread(parent) {

  // Fastdds ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
  // Create  subscriber
  pos_pub = new DDSPublisher(idl_msg::MocapPubSubType(), "mocap_pose",
                             dp_->participant());

  // initialize  subscriberDefaultParticipant
  pos_pub->init();
  // mavsdk ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  telemetry_ = std::move(telemetry);
}

PositionPublisher::~PositionPublisher() { // Fastdds
  delete pos_pub;
}

void PositionPublisher::run() { // Blocks until new data is available

  forever {

    // Check if program close has been requeated. if so, leave loop
    if (QThread::currentThread()->isInterruptionRequested()) {
      return;
    }

    pos_msg.pose.position.x =
        telemetry_->position_velocity_ned().position.north_m;
    pos_msg.pose.position.y =
        -telemetry_->position_velocity_ned().position.east_m;
    pos_msg.pose.position.z =
        -telemetry_->position_velocity_ned().position.down_m;

    std::cout << "Position: " << pos_msg.pose.position.x << '\t'
              << pos_msg.pose.position.y << '\t' << pos_msg.pose.position.z
              << std::endl;

    pos_pub->publish(pos_msg);

    // // Publish at 100Hz
    // QThread::msleep(10);

    // qDebug() << "X Position:" << sub::pos_cmd.position.x;
  }
}
