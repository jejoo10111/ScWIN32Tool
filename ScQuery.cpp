
#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <winsvc.h> //scm database handle .... create a service
#include <strsafe.h>
#include <tchar.h>
#include <vector>

//https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/dd228922(v=ws.11)


//query commands come with 
//servername, servicename, type= {driver | service | all}, type= {own | share | interact | kernel | filesys | rec | adapt}, 
//state= {active | inactive | all}, bufsize= <BufferSize>, ri= <ResumeIndex>, group= <GroupName>



//query for all when you only do <sc query>
//query for remte server... if asked if there is remote server .... actually if the secnd syntax is not wuery then it is a server
//if second is query then it is local computer https://github.com/nixonion/sc/blob/master/sc/Source.cpp#L1559

//global variables
LPCWSTR NameOfService;
SC_HANDLE hSCManager;
SC_HANDLE hService;


CHAR type[256];
CHAR state[256];
CHAR group[256] = "";

void __stdcall scqueryService();
void __stdcall scqueryENUMERATE();
void QueryCall();

void QueryCall()
{
    printf("\QUERY and QUERY OPTIONS:\n\tIf the query command is followed by a service name, the status for that service is returned. Further options do not apply in this case.  If the query command is followed by nothing or one of the options listed below, the services are enumerated. type=    Type of services to enumerate (driver, service, userservice, all)\n");
    printf("\ttype=     Type of services to enumerate (driver, service, userservice, all)\n\t\t(default = services)\n");
    printf("\tstart=    State of services to enumerate (inactive, all)\n\t\t(default = active)\n");
    printf("\tgroup=    Service group to enumerate\n\t\t(default = all groups)\n");
    printf("\tbufsize=      Service group to enumerate\n\t\(tdefault = 4096\n");

}

void __stdcall scqueryService()
{
    //https://learn.microsoft.com/en-us/windows/win32/services/querying-a-service-s-configuration
    SERVICE_STATUS_PROCESS ssp;
    DWORD dwBytesNeeded, cbBufSize;

    //openSCManager handle to the service control manager (SCM) database... connect to the SCM datatbase
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);//accesss with full accesss rights on local computer
    if (NULL == hSCManager) //check to see if handle was successfullly opened. 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    hService = OpenService(
        hSCManager,          // SCM database 
        NameOfService,             // name of service 
        SC_MANAGER_ALL_ACCESS); // need query config access 

    if (hService == NULL)
    {
        printf("SERVICES");
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Get the configuration information.
    /*BOOL QueryServiceStatusEx(
  [in]            SC_HANDLE      hService,
  [in]            SC_STATUS_TYPE InfoLevel,
  [out, optional] LPBYTE         lpBuffer,
  [in]            DWORD          cbBufSize,
  [out]           LPDWORD        pcbBytesNeeded
);*/
//https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryservicestatusex
    if (!QueryServiceStatusEx(
        hService,
        SC_STATUS_PROCESS_INFO,
        (LPBYTE)&ssp,
        sizeof(SERVICE_STATUS_PROCESS),
        &dwBytesNeeded))
    {
        //error hanfling for queryservicestatusex...// `ERROR_INSUFFICIENT_BUFFER` is not expected for QueryServiceStatusEx with SC_STATUS_PROCESS_INFO
        printf("QueryServiceConfig failed (%d)", GetLastError());
        goto cleanup;
    }



    // Print the ther service information details.
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_status
    /*typedef struct _SERVICE_STATUS {
  DWORD dwServiceType;
  DWORD dwCurrentState;
  DWORD dwControlsAccepted;
  DWORD dwWin32ExitCode;
  DWORD dwServiceSpecificExitCode;
  DWORD dwCheckPoint;
  DWORD dwWaitHint;
} SERVICE_STATUS, *LPSERVICE_STATUS;*/
    _tprintf(TEXT("\nSERVICE_NAME: %s    \n"), NameOfService);
    printf("  TYPE: %x\n", ssp.dwServiceType);
    printf("  STATE: %d\n", ssp.dwCurrentState);
    //printf("   %x\n", ssp.dwControlsAccepted);
    printf("  WIN32_EXIT_CODE: %d\n", ssp.dwWin32ExitCode);
    printf("  SERVICE_EXIT_CODE: %d\n", ssp.dwServiceSpecificExitCode);
    printf("  ChHECKPOINT: %d\n", ssp.dwCheckPoint);
    printf("  WAIT_HINT: %d\n", ssp.dwWaitHint);

cleanup:
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void __stdcall scqueryENUMERATE()
{
    DWORD bytesNeeded = 0;
    DWORD numServices;
    DWORD bufferSize = 0;
    DWORD dwServiceType = SERVICE_WIN32;
    DWORD dwServiceState = SERVICE_ACTIVE;
    wchar_t wGroup[256] = { 0 };
    size_t convertedChars = 0;

    LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL;

    // //openSCManager handle to the service control manager (SCM) database... connect to the SCM datatbase
    hSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == hSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    //servername, servicename, 
    // type= {driver | service | all}, type= {own | share | interact | kernel | filesys}, //rec and adapt are no longer functions of the windows api and rec is the first type 
    //state= {active | inactive | all}, bufsize= <BufferSize>, ri= <ResumeIndex>, group= <GroupName>

    //as of april 2023 sc query no longer has the bufsize option , ri

    //set the service type   
    if (strcmp(type, "driver") == 0)
    {
        dwServiceType = SERVICE_DRIVER;
    }
    else if (strcmp(type, "service") == 0)
    {
        dwServiceType = SERVICE_WIN32;
    }
    else if (strcmp(type, "all") == 0)
    {
        dwServiceType = SERVICE_DRIVER | SERVICE_WIN32 | SERVICE_KERNEL_DRIVER;
    }
    //other service type 
    else if (strcmp(type, "own") == 0)
    {
        dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    }
    else if (strcmp(type, "share") == 0)
    {
        dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    }
    else if (strcmp(type, "kernel") == 0)
    {
        dwServiceType = SERVICE_KERNEL_DRIVER;
    }
    else if (strcmp(type, "filesys") == 0)
    {
        dwServiceType = SERVICE_FILE_SYSTEM_DRIVER;
    }

    //set the state of the service
    if (strcmp(state, "active") == 0)
    {
        dwServiceState = SERVICE_ACTIVE;
    }
    else if (strcmp(state, "inactive") == 0)
    {
        dwServiceState = SERVICE_INACTIVE;
    }
    else if (strcmp(state, "all") == 0)
    {
        dwServiceState = SERVICE_STATE_ALL;
    }
    //grouping
    else
    {

        // Assuming `group` is your input ANSI string for the group name
        if (group[0] != '\0') { // Check if the group string is not empty
            size_t convertedChars = 0;
            mbstowcs_s(&convertedChars, wGroup, sizeof(wGroup) / sizeof(wGroup[0]), group, _TRUNCATE);
            // Ensure null-termination
            wGroup[sizeof(wGroup) / sizeof(wGroup[0]) - 1] = L'\0';
        }
        else {
            mbstowcs_s(&convertedChars, wGroup, sizeof(wGroup) / sizeof(wGroup[0]), group, _TRUNCATE); // Convert group to wide string

        }
    }
    //enumerate all services
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-enumservicesstatusa
    /*BOOL EnumServicesStatusA(
  [in]                SC_HANDLE              hSCManager,
  [in]                DWORD                  dwServiceType,
  [in]                DWORD                  dwServiceState,
  [out, optional]     LPENUM_SERVICE_STATUSA lpServices,
  [in]                DWORD                  cbBufSize,
  [out]               LPDWORD                pcbBytesNeeded,
  [out]               LPDWORD                lpServicesReturned,
  [in, out, optional] LPDWORD                lpResumeHandle
);*/
    BOOL status = EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        NULL,
        0,
        &bytesNeeded,
        &numServices,
        NULL,
        wGroup
    );

    /*Specifies the service group to be enumerated. By default, all groups are enumerated. By default, all groups are enumerated (**group= **).*/

    //this set is used to determine correcr buffer size.... allocate memory and check for allocation failure
    PBYTE lpBytes = (PBYTE)malloc(bytesNeeded);
    if (lpBytes == NULL) {
        printf("Failed to allocate memory\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    status = EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        lpBytes,
        bytesNeeded,
        &bytesNeeded,
        &numServices,
        NULL,
        wGroup
    );

    //error handling
    ENUM_SERVICE_STATUS_PROCESS* services = (ENUM_SERVICE_STATUS_PROCESS*)lpBytes;

    //free(lpBytes);
    CloseServiceHandle(hSCManager);

    //enumerate through all the services for each service run it through scquery to get all the 
    //service information 

    printf("NUMBER OF SERVICES: (%d)", numServices);
    for (DWORD i = 0; i < numServices; i++)
    {
        NameOfService = services[i].lpServiceName;
        scqueryService();
    }


}

int main(int argc, char* argv[])
{
    wchar_t wtext[100];//Name of Service is a LPCWSTR so need to convert the string 
    size_t outSize;
    int i;

    if (strcmp(argv[1], "scquery") == 0)
    {
        if (argc == 2)
        {
            //query entire local database of services
            //NameOfService = wtext;
            //wprintf(L"Name of service: %ls\n", NameOfService);
            scqueryENUMERATE();
        }
        else if (argc == 3)
        {
            //query specific service
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            NameOfService = wtext;
            scqueryService();
        }

        else
        {
            // with switches 
            //servername, servicename, type= {driver | service | all}, type= {own | share | interact | kernel | filesys | rec | adapt}, 
            //state= {active | inactive | all}, bufsize= <BufferSize>, ri= <ResumeIndex>, group= <GroupName>

            // filter the query .... /// and rename the variable for global use 

            for (i = 2; i < argc; i = i + 2)
            {
                if (strcmp(argv[i], "state=") == 0)
                {
                    strcpy_s(state, sizeof(state), argv[i + 1]);
                }
                else if (strcmp(argv[i], "type=") == 0)
                {
                    strcpy_s(type, sizeof(type), argv[i + 1]);
                }
                else if (strcmp(argv[i], "group=") == 0)
                {
                    strcpy_s(group, sizeof(group), argv[i + 1]);
                }
            }

            scqueryENUMERATE();

        }
    }
    return 0;
}
