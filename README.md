# cpp-class-management
A lightweight C++ utility for managing VS Code configuration files (tasks.json and c_cpp_properties.json). It automatically updates source file lists and include paths using the nlohmann/json library, ensuring clean, duplicate-free project settings.
## Features
- Read and write JSON files with pretty-print formatting
- Append new `.cpp` source files to `tasks.json` build arguments
- Add new include paths to `c_cpp_properties.json`
- Prevent duplicate entries with a helper function (`controlJsonArg`)
- Maintain clean and consistent project configuration automatically

## Requirements
- C++17 or later
- MSVC or GCC/Clang toolchain
- nlohmann/json library 
