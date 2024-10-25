### Qraft - A Raft Consensus Implementation on Raspberry Pi Pico 2 Boards

This project implements a simplified version of the Raft consensus algorithm across two Raspberry Pi Pico 2 boards. Each node in the system consists of two boards: one running an ARM Cortex-M33 core and the other running a RISC-V core. The ARM board handles computationally intensive tasks, such as leader election and log replication, while the RISC-V board manages communication and monitoring tasks, such as heartbeat detection and message relaying.

### Project Structure

The project uses two separate source files for each type of board, and one CMake file to handle the build process for both configurations.

### Directory Structure

```bash
qraft/
├── CMakeLists.txt         # CMake build configuration for both ARM and RISC-V
├── src/
│   ├── qraft-arm.c        # Raft logic for the ARM Cortex-M33 board
│   └── qraft-risc.c       # Communication and heartbeat logic for the RISC-V board
└── pico_sdk_import.cmake  # Import for the Pico SDK (custom path defined in CMakeLists)
```

### Getting Started

This project requires the Raspberry Pi Pico SDK. Make sure it is properly installed and the path is configured before proceeding.

### Prerequisites

**CMake** (version 3.12 or higher)

**Raspberry Pi Pico SDK**

**ARM Cortex-M33 and RISC-V Hazard 3-based Raspberry Pi Pico 2 boards**


### Setting Up the Pico SDK

1. Clone the Pico SDK to your system:

```bash
git clone https://github.com/raspberrypi/pico-sdk.git /opt/pico-sdk
```

2. Set the environment variable in CMakeLists.txt:

```bash
set(PICO_SDK_PATH "/opt/pico-sdk/")
```


### Building the Project

You can build for either the ARM or RISC-V board by setting the BUILD_TYPE parameter during the build process.

### ARM Build

```bash
mkdir build-arm
cd build-arm
cmake .. -DBUILD_TYPE=ARM
make
```

This will compile the qraft-arm.c source and create an executable for the ARM Cortex-M33-based board.

### RISC-V Build

```bash
mkdir build-risc
cd build-risc
cmake .. -DBUILD_TYPE=RISC-V
make
```

This will compile the qraft-risc.c source and create an executable for the RISC-V-based board.

### Flashing the Firmware

Once the build completes, you can flash the binaries to the respective boards using picotool or any other Pico-compatible flashing tool:

```bash
picotool load -f qraft-arm.uf2
picotool load -f qraft-risc.uf2
```

### How It Works

The system is designed around the Raft consensus algorithm, where nodes communicate over UART. Each node consists of two boards: one handles intensive consensus logic (ARM) while the other handles I/O tasks (RISC-V).

ARM Board (qraft-arm.c)

- **Leader Election**: If no leader is detected, the ARM board starts a leader election process.

- **Log Replication**: Once a leader is elected, the leader appends and replicates log entries (blockchain transactions) across all nodes.

- **Heartbeat Management**: The leader sends periodic heartbeat signals to ensure the network's stability.


### RISC-V Board (qraft-risc.c)

- **Heartbeat Monitoring**: The RISC-V board monitors the leader’s heartbeat. If no heartbeat is received within a specified timeout, it triggers the ARM board to start a new leader election.

Message Relaying: The RISC-V board relays Raft messages (e.g., vote requests, log replication commands) between nodes via UART and communicates with the ARM board.


### Inter-Board Communication

- **UART Protocol**: The ARM and RISC-V boards communicate using UART for message exchange, log replication, and heartbeat notifications.

- **Message Format**: Messages are sent as simple strings for vote requests, log entries, and heartbeat notifications.


### Configuration

You can modify various parameters such as the heartbeat timeout and election timeout in the source files to adjust the system's behavior:

HEARTBEAT_INTERVAL: The interval (in milliseconds) at which the leader sends heartbeat messages to followers.

ELECTION_TIMEOUT: The time (in milliseconds) a follower waits before starting an election if no heartbeat is received.


These parameters can be found in both qraft-arm.c and qraft-risc.c.

License

This project is licensed under the MIT License - see the LICENSE file for details.

### Acknowledgments

Raft Consensus Algorithm: The Raft algorithm is a consensus protocol designed for managing a replicated log in distributed systems. Special thanks to raft.github.io for providing detailed resources on Raft's implementation.

Raspberry Pi Pico SDK: For enabling us to use the Raspberry Pi Pico boards effectively in this project.


### Contact

For any questions or issues, feel free to open an issue on the GitHub repository or contact the project maintainers.



