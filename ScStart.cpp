#include <stdio.h>
#include <Windows.h>
#include <winsvc.h> //scm database handle .... 

//https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/cc742126(v=ws.11)

LPCWSTR NameOfService;

VOID __stdcall DoStartSvc();

VOID __stdcall DoStartSvc()
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;

    // Get a handle to the SCM database. 

    hSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == hSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    hService = OpenService(
        hSCManager,         // SCM database 
        NameOfService,            // name of service 
        SERVICE_ALL_ACCESS);  // full access 

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Check the status in case the service is not stopped. 

    if (!QueryServiceStatusEx(
        hService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // information level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // size needed if buffer is too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return;
    }

    // Check if the service is already running. It would be possible 
    // to stop the service here, but for simplicity this example just returns. 

    if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
        printf("Cannot start the service because it is already running\n");
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return;
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    // Wait for the service to stop before attempting to start it.

    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status until the service is no longer stop pending. 

        if (!QueryServiceStatusEx(
            hService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // information level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))              // size needed if buffer is too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            return;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
            {
                printf("Timeout waiting for service to stop\n");
                CloseServiceHandle(hService);
                CloseServiceHandle(hSCManager);
                return;
            }
        }
    }

    // Attempt to start the service.

    if (!StartService(
        hService,  // handle to service 
        0,           // number of arguments 
        NULL))      // no arguments 
    {
        printf("StartService failed (%d)\n", GetLastError());
        printf("The service did not respond to the start or control request in a timely fashion.\n");
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return;
    }
    else printf("Service start pending...\n");

    // Check the status until the service is no longer start pending. 

    if (!QueryServiceStatusEx(
        hService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // info level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // if buffer too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return;
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth the wait hint, but no less than 1 second and no 
        // more than 10 seconds. 

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status again. 

        if (!QueryServiceStatusEx(
            hService,             // handle to service 
            SC_STATUS_PROCESS_INFO, // info level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))              // if buffer too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            break;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
            {
                // No progress made within the wait hint.
                break;
            }
        }
    }

    // Determine whether the service is running.

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        printf("Service started successfully.\n");
    }
    else
    {
        printf("Service not started. \n");
        printf("  Current State: %d\n", ssStatus.dwCurrentState);
        printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
        printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
        printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

int main(int argc, char* argv[])
{
    int i;
    wchar_t wtext[100];//Name of Service is a LPCWSTR so need to convert the string 
    size_t outSize;

    if (strcmp(argv[1], "scstart") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        DoStartSvc();
        return 0;
    }

    return 0;
}
