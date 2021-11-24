#pragma once
#pragma once

#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QThread>
#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <memory>
#include <thread>

// Fastdds
#include "MocapPubSubTypes.h"
#include "default_participant.h"
#include "default_publisher.h"
#include "sensor_msgs/msgs/Mocap.h"

#include <QThread>

class pos_pub_thread : public QThread {
  Q_OBJECT

public:
  explicit pos_pub_thread(DefaultParticipant *dp,
                          std::shared_ptr<mavsdk::Telemetry> telemetry,
                          QObject *parent = nullptr);
  ~pos_pub_thread();

  void run();

  // fastdds objects
private:
  DefaultParticipant *dp_;

  // Motion capture data subscriber
  DDSPublisher *pos_pub;

  cpp_msg::Mocap pos_msg{};

  // mavsdk
private:
  std::shared_ptr<mavsdk::Telemetry> telemetry_;

  // Local position offsets
  constexpr static float x_offset = 0.0;
  constexpr static float y_offset = 0.0;

  // All offboard modes
  enum class offboard_mode { circle, lemniscate, external };
  offboard_mode offb_mode;
};