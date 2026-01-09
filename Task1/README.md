# Employee Salary Management System

The Employee Salary Management System is a C++ console application designed to demonstrate multi-process architecture using the Windows API. The system consists of a main orchestrating process and two utility processes: one for creating binary data files containing employee records and another for generating text-based salary reports based on that data.

## Features

*   **Multi-process Architecture:** Utilizes Windows API (`CreateProcess`, `WaitForSingleObject`) to manage independent utilities.
*   **Binary Data Management:** Stores employee information (ID, Name, Hours Worked) in a structured binary format.
*   **Automated Reporting:** Reads binary data to calculate wages based on an hourly rate and exports the results to a readable text file.
*   **Process Synchronization:** Ensures data integrity by waiting for child processes to complete before proceeding to the next execution step.

## Installation

### Prerequisites

*   **Operating System:** Microsoft Windows (Required due to dependencies on `<windows.h>`).
*   **Compiler:** A C++ compiler supporting C++14 or higher (e.g., MSVC, MinGW).
*   **Build System:** CMake (Version 3.10 or higher recommended).

### Build Instructions

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/quanted129/OS-projects/SalaryCalculatorProcesses
    cd ToDoManager
    ```
2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
3.  Generate the build files using CMake:
    ```bash
    cmake ..
    ```
4.  Compile the project:
    ```bash
    cmake --build .
    ```
    This will generate three executables in the build directory: `MainProcess.exe`, `Creator.exe`, and `Reporter.exe`.

## Usage

1.  Navigate to the directory where the executables were built.
2.  Launch the main application:
    ```bash
    ./MainProcess.exe
    ```
3.  **Binary File Creation:**
    *   Enter the desired name for the binary file (e.g., `employees.bin`).
    *   Enter the number of employee records you wish to create.
    *   The `Creator` utility will start. For each record, input the employee number, name (max 9 characters), and hours worked.
4.  **Verification:**
    *   Once data entry is complete, the main program will display the contents of the created binary file on the console.
5.  **Report Generation:**
    *   Enter the desired name for the report file (e.g., `report.txt`).
    *   Enter the hourly payment rate.
    *   The `Reporter` utility will start, process the binary file, and calculate salaries.
6.  **Final Output:**
    *   The main program will display the generated report on the console and then terminate.
