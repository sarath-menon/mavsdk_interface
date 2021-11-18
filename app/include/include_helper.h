#pragma once

#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <memory>
#include <thread>

// Fastdds
#include "default_participant.h"
#include "default_subscriber.h"
#include "fastdds_sub_variables.h"
#include "fastdds_thread.h"