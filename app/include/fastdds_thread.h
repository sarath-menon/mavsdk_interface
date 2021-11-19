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
  constexpr static float x_offset = 0.5;
  constexpr static float y_offset = 0.5;

  // All offboard modes
  enum class offboard_mode { circle, lemniscate, external };
  offboard_mode offb_mode;

  void external_pos_control();
  void circle_forever();
  void lemniscate_forever();

public:
  // setter function
  void set_offboard_mode(const int index) {
    switch (index) {
    case 0:
      break;
    case 1:
      offb_mode = offboard_mode::circle;
      break;
    case 2:
      offb_mode = offboard_mode::lemniscate;
      break;
    case 3:
      offb_mode = offboard_mode::external;
      break;
    default:
      exit(0);
    }
  }
};