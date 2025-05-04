# DirectShow Example

A C-based example project demonstrating video capture using Microsoft's DirectShow framework on Windows. This project provides a simple implementation for enumerating video input devices, initializing a capture graph, selecting a device, and rendering a live video preview stream.

## Features
- Enumerates system video input devices using COM and DirectShow.
- Allows user selection of a video device via a console interface.
- Sets up a DirectShow filter graph for live video preview.
- Includes Doxygen-compatible documentation for key functions.

## Prerequisites
- Windows OS (DirectShow is Windows-specific).
- Microsoft Visual Studio (2022 recommended) with C/C++ development tools.
- Windows SDK with DirectShow headers and libraries.

## Getting Started
1. Clone the repository: `git clone https://github.com/yourusername/directshow-example.git`
2. Open the project in Visual Studio.
3. Ensure the Windows SDK is installed and configured.
4. Build and run the project to enumerate and preview video from connected devices.

## Documentation
Function-level documentation is provided using Doxygen. Generate HTML documentation by running:
```bash
doxygen Doxyfile
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue to discuss improvements or bug fixes.