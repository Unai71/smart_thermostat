# smart_thermostat

A smart thermostat firmware project based on ESP-IDF. This repository contains the application source, build scripts, and configuration to build and flash firmware to an ESP32-based device.

Status: work in progress — see the source in [main/main.c](main/main.c) and the project configuration in [CMakeLists.txt](CMakeLists.txt).

## Features
- Temperature sensing and basic control loop
- Wi‑Fi connectivity (configurable)
- OTA-ready build layout (if enabled via sdkconfig)
- Example entry point at [main/main.c](main/main.c)

## Hardware
Target: ESP32 family development boards. Adapt pin assignments and peripherals in the `main` component.

## Quick start

Prerequisites:
- ESP-IDF toolchain installed and activated.
- USB serial driver for your board.
- Project environment variables (use `idf.py` environment).

Build:
```sh
idf.py build
```

Flash (replace PORT with your device port):
```sh
idf.py -p /dev/ttyUSB0 flash monitor
```

To clean:
```sh
idf.py fullclean
```

## Configuration
Project defaults are stored in [sdkconfig](sdkconfig). Use:
```sh
idf.py menuconfig
```
to adjust Wi‑Fi, peripherals, and other options.

## Project layout
- [CMakeLists.txt](CMakeLists.txt) — top-level CMake configuration
- main/ — application component
  - [main/main.c](main/main.c) — example application entry
  - main/CMakeLists.txt — component build info
- build/ — build artifacts (ignored in VCS)
- sdkconfig — project configuration file
- LICENSE — project license ([LICENSE](LICENSE))

## Development notes
- Use the provided devcontainer for a reproducible development environment: [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json).
- VSCode settings and launch configs are available in [.vscode](.vscode).

## Testing
Add unit/integration tests under a tests/ folder and integrate with the CI/tooling you prefer. Local testing can use the ESP-IDF test frameworks or host-side mocks.

## Contributing
1. Create an issue describing the bug or feature.
2. Create a branch from `main`.
3. Open a PR with a clear description and testing steps.

## License
This project is released under the terms in [LICENSE](LICENSE).
If you need a different license, update the LICENSE
