#include <stdio.h> //input output 
#include <Windows.h> //windows API declarations
#include <winsvc.h> //scm database handle .... or manages services

//https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/cc742126(v=ws.11)
//https://learn.microsoft.com/en-us/windows/win32/services/starting-a-service

LPCWSTR NameOfService;
void __stdcall DoStartSvc();

void __stdcall DoStartSvc()
{
    //                  handle - reference to a sresource managed by the windows kernel
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;

    // Get a handle to the SCM database. 
    //starting a service requires you to first open a handle to the service control manager (SCM) which is used by the OpenSCManager function 

    //specify computer name (NULL for local computer)
    // database name (NULL for active database)
    // access rights SC_MANAGER_ALL_ACCESS for full access

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
    //then open a handle to the service using openservice function. 
    //this function requires the SCAM database handle,
    //the name of the service, 
    //and the access rights (SERVICE_ALLACCESS for full access)

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
    //then check the service status using the queryservicestatusex function to ensure the service is not already running or in a state that prevents it from being started
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

    // Check if the service is already running. 

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
    //prevent the program from attempting to start the service while it's still in the process of stopping. 

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
    //then start the service using the startservice function. this function requires the service handle and can optionally take arguments to pass to the service's main function 
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
    //then check the service status again to ensure it has started successfully
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
    //then close the service and SCM database handles using the CloseServiceHandleFunction to release the resurces
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

int main(int argc, char* argv[])
{

    wchar_t wtext[100];//Name of Service is a LPCWSTR so need to assign pointer to a wchar_t array to an LPCWSTR variable bc mbstowcs_s needs a wchar_t parameter
    size_t outSize;

    if (strcmp(argv[1], "scstart") == 0)
    {
        //second command line arggv[2] to a wide character string. the converted string is stored in the wtext arrawy
        // //multibyte string to a wide character string
        // Converts a multibyte character string from the array whose first element is pointed to by src to its wide character representation.
            //&utsize is a pointer to size_t variable that receives the number of wide characters converted 
            //wtext is the destination wide character array where converted string will be stored 
            // size of destination array which the length of the source string plus ne for the null terminator 
            //argv[2] is the source of multibyte character string to be converted
            //the maximum number of wide characters to be written to destination array 
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        //assign PLCWSTR pointer to the address of wtext array bc service name needs to be a wide character
        NameOfService = wtext;
        DoStartSvc();
        return 0;
    }

    return 0;
}
