#include <windows.h>
#include <iostream>

int main() {
    HANDLE hDevice = CreateFile(
        R"(\\.\PhysicalDrive0)",      // Targeting the first physical drive
        GENERIC_WRITE,                // We only need write access
        FILE_SHARE_READ | FILE_SHARE_WRITE, // Share mode
        NULL,                         // Default security
        OPEN_EXISTING,               // Open existing device
        0,                           // No flags or attributes
        NULL                         // No template file
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open physical drive. Error: " << GetLastError() << std::endl;
        return 1;
    }

    // Set the offset (sector 34 * 512 = 17408 bytes)
    LARGE_INTEGER offset;
    offset.QuadPart = 17408;

    if (SetFilePointerEx(hDevice, offset, NULL, FILE_BEGIN) == 0) {
        std::cerr << "Failed to set file pointer. Error: " << GetLastError() << std::endl;
        CloseHandle(hDevice);
        return 1;
    }

    const char* data = "this is just a test to make sure wh3th3r this C++ shit works or not. Let's hope the system doesn't crash.";
    DWORD bytesWritten;

    if (!WriteFile(hDevice, data, strlen(data), &bytesWritten, NULL)) {
        std::cerr << "Failed to write to the drive. Error: " << GetLastError() << std::endl;
        CloseHandle(hDevice);
        return 1;
    }

    std::cout << "Successfully wrote " << bytesWritten << " bytes to PhysicalDrive0 at sector 34." << std::endl;

    CloseHandle(hDevice);
    return 0;
}
