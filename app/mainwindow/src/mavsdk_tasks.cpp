#include "./ui_mainwindow.h"
#include "mainwindow.h"

// arming
void MainWindow::arm() {
  console_log("Arming...");
  const Action::Result arm_result = action->arm();
  if (arm_result != Action::Result::Success) {
    console_log("Arming failed: ");
  }
};

// disarming
void MainWindow::disarm() {
  console_log("Taking off..");
  const Action::Result arm_result = action->disarm();
};

// takeoff
void MainWindow::takeoff() {
  const Action::Result takeoff_result = action->takeoff();
  if (takeoff_result != Action::Result::Success) {
    console_log("Takeoff failed");
  }
};

// landing
void MainWindow::land() {
  console_log("Landing...");

  const Action::Result land_result = action->land();
  if (land_result != Action::Result::Success) {
    console_log("Land failed");
  }
};

// enable offboard mode
void MainWindow::offboard_enable() {
  // check if offboard already enabled
  if (!offb_enabled) {
    // Set flag to indicate offboard mode is activated
    offb_enabled = true;

    auto offboard = mavsdk::Offboard{system};

    // Send it once before starting offboard, otherwise it will be rejected.
    const Offboard::VelocityNedYaw stay{};
    // Drone stays in place waiting for commands
    offboard.set_velocity_ned(stay);

    Offboard::Result offboard_result = offboard.start();
    if (offboard_result != Offboard::Result::Success) {
      console_log("Offboard start failed");
    } else {
      console_log("Offboard started");

      // Start offboard thread
      // Start fastdds thread
      fastdds_obj =
          std::make_unique<fastdds_thread>(std::make_unique<Offboard>(system));
      fastdds_obj->start();
    }
  } else {
    console_log("Offboard is already enabled");
  }
};

// disable offboard mode
void MainWindow::offboard_disable() {
  // check if offboard is enabled before stopping
  if (offb_enabled) { // Set flag to indicate offboard mode is activated

    auto offboard = mavsdk::Offboard{system};

    // Stop offboard thread
    fastdds_obj->quit();
    fastdds_obj->requestInterruption();
    fastdds_obj->wait();

    Offboard::Result offboard_result = offboard_result = offboard.stop();
    if (offboard_result != Offboard::Result::Success) {
      console_log("Offboard stop failed");
    } else {
      console_log("Offboard stopped");
    }

    // reset offboard enabled status
    offb_enabled = false;

  } else {
    console_log("Offboard is already disabled");
  }
};

void MainWindow::connect() {
  // check if already initialized
  if (!initialized) {
    // Create mavsdk object
    mavsdk = std::make_unique<Mavsdk>();

    QString mavsdk_port = ui->port_selector->currentText();
    ConnectionResult connection_result =
        mavsdk->add_any_connection(mavsdk_port.toStdString());

    // Error checking
    if (connection_result != ConnectionResult::Success) {
      console_log("Connection failed:");
    }

    else {

      // Get pointer to system from mavsdk obj
      system = get_system(*mavsdk);

      // set intitialized status
      initialized = true;

      // Error checking
      if (!system) {
        console_log("Couldn't get system");
      }

      else {
        // Create telemetry object
        telemetry = std::make_unique<Telemetry>(system);
        action = std::make_unique<Action>(system);
      }
    }
  }

  else {
    console_log("Already Initialized");
  }
}