#pragma once

#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QTextStream>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

// Fastdds
#include "default_participant.h"
#include "default_subscriber.h"

// Threads
#include "offboard_thread.h"
#include "publish_position_thread.h"

// MAVSDK
#include <mavsdk/log_callback.h>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

// others
#include "custom_types.h"
