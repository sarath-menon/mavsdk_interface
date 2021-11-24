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

class PositionPublisher : public QThread {
  Q_OBJECT

public:
  explicit PositionPublisher(DefaultParticipant *dp,
                             std::unique_ptr<mavsdk::Telemetry> telemetry,
                             QObject *parent = nullptr);
  ~PositionPublisher();

  void run();

  // fastdds objects
private:
  DefaultParticipant *dp_;

  // Motion capture data subscriber
  DDSPublisher *pos_pub;

  cpp_msg::Mocap pos_msg{};

  // mavsdk
private:
  std::unique_ptr<mavsdk::Telemetry> telemetry_;
};