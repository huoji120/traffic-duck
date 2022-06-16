
#include <Windows.h>
#include <immintrin.h>
#include <intrin.h>

#include <fstream>
#include <iostream>

#define SLEEP_TIME 120000
#define INTERVAL_TIME SLEEP_TIME - 2000
void WriteFile(std::string pName, std::string pBody) {
    std::ofstream ofile(pName);
    ofile << pBody;
    ofile.close();
}
HANDLE EasyCreateThread(void* pFunctionAddress, void* pParams) {
    return CreateThread(
        NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(pFunctionAddress),
        static_cast<LPVOID>(pParams), NULL, NULL);
}
void ThreadRoute(HANDLE pEventHandle) {
    //其实不需要sleep的否则会被某些沙箱标记为"sleep"已经加速
    //我只是为了写文件才sleep的.正常情况直接WaitForSingleObjectEx等{{SLEEP_TIME}}就行
    Sleep(SLEEP_TIME);
    SetEvent(pEventHandle);
}
int main() {
    const auto eventHandle = CreateEventW(NULL, FALSE, FALSE, L"");
    if (eventHandle) {
        EasyCreateThread(static_cast<void*>(ThreadRoute),
                         static_cast<LPVOID>(eventHandle));
        const auto singleStatus =
            WaitForSingleObjectEx(eventHandle, INTERVAL_TIME, false);
        singleStatus != WAIT_TIMEOUT
            ? WriteFile("block.txt", "{64D8DECE-0F4C-4601-84E2-A6B154FDD155}")
            : WriteFile("pass.txt", "{E8415AFE-0126-428C-ACF9-EA035D6362C4}");
        CloseHandle(eventHandle);
    }
    return 0;
}
