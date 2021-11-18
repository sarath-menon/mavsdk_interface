#include "fastdds_thread.h"
#include <qglobal.h>
#include <qthread.h>

fastdds_thread::fastdds_thread(QObject *parent) : QThread(parent) {

  // Fastdds ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

  // Create fastdds objects
  // Create domain participant
  dp = std::make_unique<DefaultParticipant>(0, "mavsdk_gui_interface");

  // Create  subscriber
  cmd_sub = new DDSSubscriber(idl_msg::QuadPositionCmdPubSubType(),
                              &sub::pos_cmd, "pos_cmd", dp->participant());

  // initialize  subscriberDefaultParticipant
  cmd_sub->init();
  // Qt ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

    cmd_sub->listener->wait_for_data_for_ms(100);
  };
}
