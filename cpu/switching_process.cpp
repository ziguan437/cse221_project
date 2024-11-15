#include <iostream>
#include <windows.h>

int main() {
    // Use specific event names so both parent and child can access them
    HANDLE event1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event1");
    HANDLE event2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event2");

    if (!event1) {
        std::cerr << "Child: Failed to open event1. Error: " << GetLastError() << std::endl;
        return 1;
    }
    if (!event2) {
        std::cerr << "Child: Failed to open event2. Error: " << GetLastError() << std::endl;
        CloseHandle(event1);
        return 1;
    }

    std::cout << "Child: Successfully opened events.\n";


    std::cout << "Child: Waiting for event1 signal from parent..." << std::endl;
    DWORD waitResult = WaitForSingleObject(event1, INFINITE);
    if (waitResult == WAIT_FAILED) {
        std::cerr << "Child: WaitForSingleObject failed for event1. Error: " << GetLastError() << std::endl;
        CloseHandle(event1);
        CloseHandle(event2);
        return 1;
    }
    std::cout << "Child: Received event1 signal from parent." << std::endl;
    std::cout << "Child: Signaling event2 back to parent." << std::endl;
    if (!SetEvent(event2)) {
        std::cerr << "Child: Failed to signal event2. Error: " << GetLastError() << std::endl;
        CloseHandle(event1);
        CloseHandle(event2);
        return 1;
    }

    std::cout << "Child: Successfully signaled event2.\n";

    CloseHandle(event1);
    CloseHandle(event2);

    return 0;
}

