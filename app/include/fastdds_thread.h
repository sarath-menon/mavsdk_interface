#pragma once
#include "include_helper.h"

#include <QThread>

class fastdds_thread : public QThread {
  Q_OBJECT

public:
  explicit fastdds_thread(QObject *parent = nullptr);
  ~fastdds_thread();

  void run();

  // fastdds objects
private:
  // Create doamin participant
  std::unique_ptr<DefaultParticipant> dp;

  // Motion capture data subscriber
  DDSSubscriber<idl_msg::QuadPositionCmdPubSubType, cpp_msg::QuadPositionCmd>
      *cmd_sub;
};
