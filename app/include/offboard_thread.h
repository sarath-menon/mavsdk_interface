#pragma once
#include "include_helper.h"

#include <QThread>

class fastdds_thread : public QThread {
  Q_OBJECT

public:
  explicit fastdds_thread(std::unique_ptr<mavsdk::Offboard> offboard,
                          QObject *parent = nullptr);
  ~fastdds_thread();

  void run();

  std::unique_ptr<mavsdk::Offboard> return_offboard_obj();

  // fastdds objects
private:
  // Create doamin participant
  std::unique_ptr<DefaultParticipant> dp;

  // Motion capture data subscriber
  DDSSubscriber<idl_msg::QuadPositionCmdPubSubType, cpp_msg::QuadPositionCmd>
      *cmd_sub;

  // mavsdk
private:
  std::unique_ptr<mavsdk::Offboard> offboard_;
  mavsdk::Offboard::PositionNedYaw pos_msg{};

  // Local position offsets
  constexpr static float x_offset = 0.0;
  constexpr static float y_offset = 0.0;

  // All offboard modes
  enum class offboard_mode { circle, lemniscate, external };
  offboard_mode offb_mode;

  void circle_forever();
  void lemniscate_forever();
};