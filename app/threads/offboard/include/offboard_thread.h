#pragma once
#pragma once

#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QThread>
#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <memory>
#include <sub_variables.h>
#include <thread>

// Fastdds
#include "MocapPubSubTypes.h"
#include "default_participant.h"
#include "default_publisher.h"
#include "default_subscriber.h"
#include "sensor_msgs/msgs/Mocap.h"

#include <QThread>

namespace threadflags {
inline std::atomic_bool pos_pub{};
}

class OffboardThread : public QThread {
  Q_OBJECT

public:
  explicit OffboardThread(DefaultParticipant *dp, mavsdk::Offboard *offboard,
                          mavsdk::Telemetry *telemetry,
                          QObject *parent = nullptr);
  ~OffboardThread();

  void run();

  // fastdds objects
private:
  DefaultParticipant *dp_;

  // Motion capture data subscriber
  DDSSubscriber<idl_msg::QuadPositionCmdPubSubType, cpp_msg::QuadPositionCmd>
      *cmd_sub;

  // Motion capture data subscriber
  DDSPublisher *pos_pub;

  // mavsdk
private:
  mavsdk::Offboard *offboard_;
  mavsdk::Telemetry *telemetry_;

  mavsdk::Offboard::PositionNedYaw pos_msg{};

  // Local position offsets
  constexpr static float x_offset = 0.0;
  constexpr static float y_offset = 0.0;

  // All offboard modes
  enum class offboard_mode { circle, lemniscate, external };
  offboard_mode offb_mode;

  // void publish_position();
};