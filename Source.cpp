#include <stdio.h>
#include <Windows.h>
#include <winsvc.h> //scm database handle ....
#include <iostream>
#include <string>
#include <tchar.h>
//#include <vector>


//global variables
TCHAR CREATENameOfService[256]; //ichanged this from TCHAR to LPCWSTR if something stops working!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
TCHAR displayName[256];
TCHAR BinaryPathName[256];
LPCWSTR NameOfService;
SC_HANDLE hSCManager;
SC_HANDLE hService;

//global variables for argument/options types for create/config
CHAR start[100];
CHAR error[100];
LPCWSTR DisplayName = NULL;
LPCWSTR password = NULL;
LPCWSTR group = NULL;
LPCWSTR obj = NULL;
LPCWSTR depend = NULL;
LPCWSTR binpath = NULL;

//query global variables
CHAR type[256];
CHAR state[256];
CHAR groupQ[256] = "";

//variables for arguments for create/config
DWORD CreateType = SERVICE_WIN32_OWN_PROCESS;
DWORD StartType = SERVICE_DEMAND_START;
DWORD Error = SERVICE_ERROR_NORMAL;
DWORD ServiceType = SERVICE_WIN32_OWN_PROCESS;

//failure variables
CHAR reset[100] = "";
CHAR reboot[100] = "";
CHAR action[100] = "";
CHAR command[100] = "";


//function calls
void FailureCall();
void ConfigCall();
void CreateCall();
void QueryCall();

void __stdcall sccreate();
void __stdcall scqueryService();
void __stdcall scqueryENUMERATE();
void __stdcall DeleteSvc();
void __stdcall Qdes();
void __stdcall scconfig(); 
void __stdcall DoStartSvc();
void __stdcall StopSvc();
BOOL __stdcall StopDependentServices();
void __stdcall scFAIL();


//description calls
void FailureCall()
{
    printf("\nDESCRIPTION:\n\tChanges the actions upon failure.\n");
    printf("\n\tUSAGE:\n\tsc <server> failure [service name] <option1> <option2>...\n");
    printf("\n\tOPTIONS:\n");
    printf("\treset=  <Length of period of no failures (in seconds) after which to reset the failure count to 0 (may be INFINITE) > (Must be used in conjunction with actions = )\n");
    printf("\t reboot=  <Message broadcast before rebooting on failure>\n");
    printf("\tcommand= <Command line to be run on failure>\n");
    printf("\tactions=  <Failure actions and their delay time (in milliseconds), separated by / (forward slash) --e.g., run / 5000 / reboot / 800 Valid actions are <run | restart | reboot> > (Must be used in conjunction with the reset = option)\n");
}
void ConfigCall()
{
    printf("\nDESCRIPTION:\n\tCreates a service entry in the registry and Service Database.\n");
    printf("\n\tUSAGE:\n\tsc <server> create [service name] [binPath= ] <option1> <option2>...\n");
    printf("\n\tOPTIONS:\n\t\tNOTE: The option name includes the equal sign.\n\tA space is required between the equal sign and the value.\n");
    printf("\ttype= <own | share | interact | kernel | filesys type= {own | share}>\n\t\t(default = own)\n");
    printf("\tstart= <boot | auto | demand | disabled | delayed - auto>\n\t\t(default = demand)\n");
    printf("\terror= <normal | severe | critical | ignore>\n\t\t(default = normal)\n");
    printf("\tbinPath= <BinaryPathName to the.exe file>\n\t\tgroup= <LoadOrderGroup>\n");
    printf("\ttag= <yes | no>\n");
    printf("\tdepend= <Dependencies(separated by / (forward slash))>\n");
    printf("\tobj= <AccountName | ObjectName>\n\t\t(default = LocalSystem)\n");
    printf("\tDisplayName= <display name>\n");
    printf("\tpassword= <password>;\n");
}
void CreateCall()
{
    printf("\nDESCRIPTION:\n\tCreates a service entry in the registry and Service Database.\n");
    printf("\n\tUSAGE:\n\tsc <server> create [service name] [binPath= ] <option1> <option2>...\n");
    printf("\n\tOPTIONS:\n\t\tNOTE: The option name includes the equal sign.\n\tA space is required between the equal sign and the value.\n");
    printf("\ttype= <own | share | interact | kernel | filesys | rec type= {own | share}>\n\t\t(default = own)\n");
    printf("\tstart= <boot | system | auto | demand | disabled | delayed - auto>\n\t\t(default = demand)\n");
    printf("\terror= <normal | severe | critical | ignore>\n\t\t(default = normal)\n");
    printf("\tbinPath= <BinaryPathName to the.exe file>\n\t\tgroup= <LoadOrderGroup>\n");
    printf("\ttag= <yes | no>\n");
    printf("\tdepend= <Dependencies(separated by / (forward slash))>\n");
    printf("\tobj= <AccountName | ObjectName>\n\t\t(default = LocalSystem)\n");
    printf("\tDisplayName= <display name>\n");
    printf("\tpassword= <password>;\n");
}
void QueryCall()
{
    printf("\QUERY and QUERY OPTIONS:\n\tIf the query command is followed by a service name, the status for that service is returned. Further options do not apply in this case.  If the query command is followed by nothing or one of the options listed below, the services are enumerated. type=    Type of services to enumerate (driver, service, userservice, all)\n");
    printf("\ttype=     Type of services to enumerate (driver, service, userservice, all)\n\t\t(default = services)\n");
    printf("\tstart=    State of services to enumerate (inactive, all)\n\t\t(default = active)\n");
    printf("\tgroup=    Service group to enumerate\n\t\t(default = all groups)\n");
    printf("\tbufsize=      Service group to enumerate\n\t\(tdefault = 4096\n");

}


int main(int argc, char* argv[])
{

 //variables
    size_t outSize;
    wchar_t wtext[100];
    size_t outSize1;
    wchar_t wtext1[100];
    wchar_t wtext2[100];
    size_t outSize2;

    //variables for the argument/switches
    size_t outSize3;
    wchar_t wtext3[100];

    size_t outSize4;
    wchar_t wtext4[100];

    size_t outSize5;
    wchar_t wtext5[100];

    size_t outSize6;
    wchar_t wtext6[100];

    size_t outSize7;
    wchar_t wtext7[100];

    size_t outSize8;
    wchar_t wtext8[100];

    int i;

    //sccreate 
    if (strcmp(argv[1], "sccreate") == 0) //compare the first argument passed to see if it is sccreate
    {
        //create has 4 required argmuents 
        //sc <server> create [service name] [binPath= ] <option1> <option2>...

        // if less than 4 pull up help screen
        if (argc < 4)
        {
            CreateCall();
            return 1;
        }

        // if 4 arguments create default service
        else if (argc == 5)
        {
            //convert the command-line arguments to TCHAR*.... command-line arguments need to be converted to TCHAR* before assigning them to global variables 
            mbstowcs_s(&outSize, CREATENameOfService, strlen(argv[2]) + 1, argv[2], _TRUNCATE);
            mbstowcs_s(&outSize2, BinaryPathName, strlen(argv[4]) + 1, argv[4], _TRUNCATE);
            sccreate();
            return 0;
        }
        // has 10 options 
        //if greater than 3 
        else
        {
            for (i = 5; i < argc; i = i + 2)
            {
                mbstowcs_s(&outSize, CREATENameOfService, strlen(argv[2]) + 1, argv[2], _TRUNCATE); //name of service
                mbstowcs_s(&outSize2, BinaryPathName, strlen(argv[4]) + 1, argv[4], _TRUNCATE); //binPath = <BinaryPathName to the.exe file>

                //options  A space is required between the equal sign and the value.
                //type = <own | share | interact | kernel | filesys | rec | userown | usershare> (default = own)

                if (strcmp(argv[i], "type=") == 0)
                {
                    //copy the string+1 if it equals "type="
                    strncpy_s(type, argv[i + 1], sizeof(type));
                }

                // start = <boot | system | auto | demand | disabled | delayed - auto> (default = demand)
                else if (strcmp(argv[i], "start=") == 0)
                {
                    //copy the string+1 if it equals "start="
                    strncpy_s(start, argv[i + 1], sizeof(start));
                }

                else if (strcmp(argv[i], "DisplayName=") == 0)
                {
                    //copy the string+1 if it equals "start="
                    mbstowcs_s(&outSize1, displayName, strlen(argv[i]) + 1, argv[i + 1], _TRUNCATE);
                }

                //error = <normal | severe | critical | ignore> (default = normal)
                else if (strcmp(argv[i], "error=") == 0)
                {
                    //copy the string+1 if it equals "error="
                    strncpy_s(error, argv[i + 1], sizeof(error));
                }

                //password = <password>
                else if (_strcmpi(argv[i], "password=") == 0)
                {
                    //password is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize3, wtext3, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    password = wtext3;
                }

                //group = <LoadOrderGroup>
                else if (_strcmpi(argv[i], "group=") == 0)
                {
                    // is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize4, wtext4, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    group = wtext4;
                }

                //depend = <Dependencies(separated by / (forward slash))>
                else if (_strcmpi(argv[i], "depend=") == 0)
                {
                    // is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize6, wtext6, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    depend = wtext6;
                }
                //obj = <AccountName | ObjectName> (default = LocalSystem)
                else if (_strcmpi(argv[i], "obj=") == 0)
                {
                    // is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize7, wtext7, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    obj = wtext7;
                }
            }
            sccreate();
            return 0;
        }
    }

    //scquery
    //https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/dd228922(v=ws.11)
    //query commands come with 
    //servername, servicename, type= {driver | service | all}, type= {own | share | interact | kernel | filesys | rec | adapt}, 
    //state= {active | inactive | all}, bufsize= <BufferSize>, ri= <ResumeIndex>, group= <GroupName>
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
                    strcpy_s(groupQ, sizeof(groupQ), argv[i + 1]);
                }
            }

            scqueryENUMERATE();

        }
    }

    //scdelete
    if (strcmp(argv[1], "scdelete") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        DeleteSvc();
        return 0;
    }

    //scqdescription
    //https://learn.microsoft.com/en-us/windows/win32/services/svcconfig-cpp
    //https://learn.microsoft.com/en-us/windows/win32/services/querying-a-service-s-configuration
    if (strcmp(argv[1], "scqdescription") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        Qdes();
        return 0;
    }

    //scconfig
    if (strcmp(argv[1], "scconfig") == 0) //compare the first argument passed to see if it is sccreate
    {
        //create has 4 required argmuents
        //sc <server> create [service name] [binPath= ] <option1> <option2>...

        // if less than 4 pull up help screen
        if (argc < 3)
        {
            ConfigCall();
            return 1;
        }


        else
        {
            mbstowcs_s(&outSize, wtext1, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            NameOfService = wtext1;

            for (i = 3; i < argc; i = i + 2)
            {
                printf("FOR LOOP");
                //options  A space is required between the equal sign and the value. argv[i+1]
                //type = <own | share | interact | kernel | filesys | rec | userown | usershare> (default = own)

                if (strcmp(argv[i], "type=") == 0)
                {
                    //copy the string+1 if it equals "type="
                    strcpy_s(type, sizeof(type), argv[i + 1]);
                }

                // start = <boot | system | auto | demand | disabled | delayed - auto> (default = demand)
                else if (strcmp(argv[i], "start=") == 0)
                {
                    //copy the string+1 if it equals "start="
                    strcpy_s(start, sizeof(start), argv[i + 1]);
                }

                else if (strcmp(argv[i], "DisplayName=") == 0)
                {
                    //copy the string+1 if it equals "start="
                    mbstowcs_s(&outSize1, wtext2, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    DisplayName = wtext2;
                }

                //error = <normal | severe | critical | ignore> (default = normal)
                else if (strcmp(argv[i], "error=") == 0)
                {
                    //copy the string+1 if it equals "error="
                    strcpy_s(error, sizeof(error), argv[i + 1]);
                }

                //password = <password>
                else if (_strcmpi(argv[i], "password=") == 0)
                {
                    //password is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize3, wtext3, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    password = wtext3;
                }

                //group = <LoadOrderGroup>
                else if (_strcmpi(argv[i], "group=") == 0)
                {
                    // is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize4, wtext4, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    group = wtext4;
                }

                //depend = <Dependencies(separated by / (forward slash))>
                else if (_strcmpi(argv[i], "depend=") == 0)
                {
                    // is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize6, wtext6, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    depend = wtext6;
                }
                //obj = <AccountName | ObjectName> (default = LocalSystem)
                else if (_strcmpi(argv[i], "obj=") == 0)
                {
                    // is a LCWSTR and incompatible with char had to convert the ascii and then store
                    mbstowcs_s(&outSize7, wtext7, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    obj = wtext7;
                }
                else if (_strcmpi(argv[i], "binPath=") == 0)
                {
                    mbstowcs_s(&outSize8, wtext8, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    binpath = wtext8;
                }
            }
            scconfig();
            return 0;
        }
    }

    //scstart
    if (strcmp(argv[1], "scstart") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        DoStartSvc();
        return 0;
    }

    //scstop
    if (strcmp(argv[1], "scstop") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        StopSvc();
        return 0;
    }

    //scfailure
    if (strcmp(argv[1], "scfailure") == 0)
    {

        if (argc <= 3)
        {
            //return if insufficient parameters
            FailureCall();
            return 1;
        }

        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;

        //set different flags
        for (i = 3; i < argc; i = i + 2)
        {
            if (strcmp(argv[i], "reset=") == 0)
            {
                strcpy_s(reset, sizeof(reset), argv[i + 1]);
            }
            else if (strcmp(argv[i], "reboot=") == 0)
            {
                strcpy_s(reboot, sizeof(reboot), argv[i + 1]);
            }
            else if (strcmp(argv[i], "command=") == 0)
            {
                strcpy_s(command, sizeof(command), argv[i + 1]);
            }
            else if (strcmp(argv[i], "actions=") == 0)
            {
                strcpy_s(action, sizeof(action), argv[i + 1]);
            }
        }
        scFAIL();


        return 0;
    }

    // Body of the program
    return 0; // Indicates successful execution
}


void __stdcall sccreate()
{
    //https://forums.codeguru.com/showthread.php?504620-OpenService-returns-Null-but-GetLastError-also-returns-Null
    //https://github.com/nixonion/sc/blob/master/sc/Source.cpp#L46
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicea
    //https://learn.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicew
    //https://learn.microsoft.com/en-us/windows/win32/services/installing-a-service
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagera

    SC_HANDLE hSCManager;
    SC_HANDLE hService;

    //openSCManager handle to the service control manager (SCM) database... connect to the SCM datatbase
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);//accesss with full accesss rights on local computer
    if (NULL == hSCManager) //check to see if handle was successfullly opened. 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    //10 arguments, but only need type, start, error
    // don't need others because should be able to plug it in bc it is string

        //type = <own | share | interact | kernel | filesys | rec type= {own | share}> (default = own)

    if (strcmp(type, "own") == 0)
    {
        CreateType = SERVICE_WIN32_OWN_PROCESS;
    }
    else if (strcmp(type, "share") == 0)
    {
        CreateType = SERVICE_WIN32_SHARE_PROCESS;
    }
    else if (strcmp(type, "kernel") == 0)
    {
        CreateType = SERVICE_KERNEL_DRIVER;
    }
    else if (strcmp(type, "filesys") == 0)
    {
        CreateType = SERVICE_FILE_SYSTEM_DRIVER;
    }
    else if (strcmp(type, "rec") == 0)
    {
        CreateType = SERVICE_RECOGNIZER_DRIVER;
    }

    // start = <boot | system | auto | demand | disabled | delayed - auto> (default = demand)
    // these were the only parameters provided for the start in service creation 
    if (strcmp(start, "demand") == 0)
    {
        StartType = SERVICE_DEMAND_START;
    }
    else if (strcmp(start, "boot") == 0)
    {
        StartType = SERVICE_BOOT_START;
    }
    else if (strcmp(start, "system") == 0)
    {
        StartType = SERVICE_SYSTEM_START;
    }
    else if (strcmp(start, "auto") == 0)
    {
        StartType = SERVICE_AUTO_START;
    }
    else if (strcmp(start, "disabled") == 0)
    {
        StartType = SERVICE_DISABLED;
    }

    //error = <normal | severe | critical | ignore> (default = normal)

    if (strcmp(error, "normal") == 0)
    {
        Error = SERVICE_ERROR_NORMAL;
    }
    else if (strcmp(error, "severe") == 0)
    {
        Error = SERVICE_ERROR_SEVERE;
    }
    else if (strcmp(error, "critical") == 0)
    {
        Error = SERVICE_ERROR_CRITICAL;
    }
    else if (strcmp(error, "ignore") == 0)
    {
        Error = SERVICE_ERROR_IGNORE;
    }

    //create a new service or OpenService to open a handle to an existing service //did not use createservicea or w but it defaults to createservicew unicode
    hService = CreateService(      //hService = CreateServiceA(
        hSCManager,                 //SC_HANDLE hSCManager,
        CREATENameOfService,              //Name of Service 
        displayName,                //displayname   
        SERVICE_ALL_ACCESS,         //desired access                          
        CreateType,                 //service type... service that runs in its own process.... there is share //but this is default
        StartType,                  //start type  //this is default
        Error,                      //error control type //this is default
        BinaryPathName,             //binary path name
        group,                      //no load ordering group
        NULL,                        //no tag identifier... not widely used and not documented... only for specific applications
        depend,                     // no dependencies
        NULL,                       //localsystem account
        password                    //no password
    );


    //check to see if createservice function call was successful by verifying the handle is NULL. if NULL then service reation failed
    if (hService == NULL)
    {
        DWORD error = GetLastError();
        if (error == ERROR_SERVICE_EXISTS)
        {
            printf("The service already exists.");
        }
        else
        {
            printf("(%d)", error);
            printf("CreateService failed (%d)\n", GetLastError());
        }

        CloseServiceHandle(hSCManager);
        return;
    }

    printf("Service created successfully\n");

    //close service handle when you're done to avoid resource leaks
    CloseServiceHandle(hService);
    //close the SCM database handle
    CloseServiceHandle(hSCManager);
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
            mbstowcs_s(&convertedChars, wGroup, sizeof(wGroup) / sizeof(wGroup[0]), groupQ, _TRUNCATE);
            // Ensure null-termination
            wGroup[sizeof(wGroup) / sizeof(wGroup[0]) - 1] = L'\0';
        }
        else {
            mbstowcs_s(&convertedChars, wGroup, sizeof(wGroup) / sizeof(wGroup[0]), groupQ, _TRUNCATE); // Convert group to wide string

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
void __stdcall DeleteSvc()
{
    ////https://learn.microsoft.com/en-us/windows/win32/services/svcconfig-cpp
    SERVICE_STATUS ssStatus;

    // Get a handle to the SCM database. 

    hSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == hSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    hService = OpenService(
        hSCManager,       // SCM database 
        NameOfService,          // name of service 
        DELETE);            // need delete access 

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Delete the service.

    if (!DeleteService(hService))
    {
        printf("DeleteService failed (%d)\n", GetLastError());
    }
    else printf("Service deleted successfully\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
void __stdcall Qdes()
{
    LPQUERY_SERVICE_CONFIG lpsc;
    LPSERVICE_DESCRIPTION lpsd;
    lpsc = NULL;
    lpsd = NULL;
    DWORD dwBytesNeeded, cbBufSize, dwError;

    // Get a handle to the SCM database.

    hSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database
        SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == hSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    hService = OpenService(
        hSCManager,          // SCM database
        NameOfService,             // name of service
        SERVICE_QUERY_CONFIG); // need query config access

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Get the configuration information.

    if (!QueryServiceConfig(
        hService,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig failed (%d)", dwError);
            goto cleanup;
        }
    }

    if (!QueryServiceConfig(
        hService,
        lpsc,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig failed (%d)", GetLastError());
        goto cleanup;
    }

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_DESCRIPTION,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_DESCRIPTION)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig2 failed (%d)", dwError);
            goto cleanup;
        }
    }

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_DESCRIPTION,
        (LPBYTE)lpsd,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig2 failed (%d)", GetLastError());
        goto cleanup;
    }

    // Print the configuration information.

    _tprintf(TEXT("SERVICE NAME: %s\n"), NameOfService);


    if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
        _tprintf(TEXT("  Description: %s\n"), lpsd->lpDescription);

    LocalFree(lpsc);
    LocalFree(lpsd);

cleanup:
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
void __stdcall scconfig()
{
    //https://forums.codeguru.com/showthread.php?504620-OpenService-returns-Null-but-GetLastError-also-returns-Null
    //https://github.com/nixonion/sc/blob/master/sc/Source.cpp#L46
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicea
    //https://learn.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicew
    //https://learn.microsoft.com/en-us/windows/win32/services/installing-a-service
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagera

    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    LPQUERY_SERVICE_CONFIG lpsc = NULL;
    LPSERVICE_DESCRIPTION lpsd = NULL;
    DWORD dwBytesNeeded, cbBufSize, dwError;



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
        SERVICE_QUERY_CONFIG); // need change config access 

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Get the configuration information.

    if (!QueryServiceConfig(
        hService,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig failed (%d)", dwError);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
        }
    }

    if (!QueryServiceConfig(
        hService,
        lpsc,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig failed (%d)", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
    }

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_DESCRIPTION,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_DESCRIPTION)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig2 failed (%d)", dwError);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
        }
    }

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_DESCRIPTION,
        (LPBYTE)lpsd,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig2 failed (%d)", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
    }

    CloseServiceHandle(hService);


    //10 arguments, but only need type, start, error
    // don't need others because should be able to plug it in bc it is string
    DWORD     ServiceType = lpsc->dwServiceType;
    DWORD     StartType = lpsc->dwStartType;
    DWORD     Error = lpsc->dwErrorControl;
    LPCWSTR   BinaryPath = lpsc->lpBinaryPathName;
    LPCWSTR   PassW = NULL;
    LPCWSTR   Display = lpsc->lpDisplayName;
    //type = <own | share | interact | kernel | filesys | rec type= {own | share}> (default = own)
//set binary path
    if (binpath != NULL)
    {
        BinaryPath = binpath;
    }

    //set password
    if (password != NULL)
    {
        PassW = password;
    }

    //set displayname
    if (DisplayName != NULL)
    {
        Display = DisplayName;
    }
    if (strcmp(type, "own") == 0)
    {
        ServiceType = SERVICE_WIN32_OWN_PROCESS;
    }
    else if (strcmp(type, "share") == 0)
    {
        ServiceType = SERVICE_WIN32_SHARE_PROCESS;
    }
    else if (strcmp(type, "kernel") == 0)
    {
        ServiceType = SERVICE_KERNEL_DRIVER;
    }
    else if (strcmp(type, "filesys") == 0)
    {
        ServiceType = SERVICE_FILE_SYSTEM_DRIVER;
    }
    /*else if (strcmp(type, "rec") == 0)
    {
        ServiceType = SERVICE_RECOGNIZER_DRIVER;
    }*/

    // start = <boot | system | auto | demand | disabled | delayed - auto> (default = demand)
    // these were the only parameters provided for the start in service creation
    if (strcmp(start, "demand") == 0)
    {
        StartType = SERVICE_DEMAND_START;
    }
    else if (strcmp(start, "boot") == 0)
    {
        StartType = SERVICE_BOOT_START;
    }
    /*else if (strcmp(start, "system") == 0)
    {
        StartType = SERVICE_SYSTEM_START;
    }*/
    else if (strcmp(start, "auto") == 0)
    {
        StartType = SERVICE_AUTO_START;
    }
    else if (strcmp(start, "disabled") == 0)
    {
        StartType = SERVICE_DISABLED;
    }

    //error = <normal | severe | critical | ignore> (default = normal)

    if (strcmp(error, "normal") == 0)
    {
        Error = SERVICE_ERROR_NORMAL;
    }
    else if (strcmp(error, "severe") == 0)
    {
        Error = SERVICE_ERROR_SEVERE;
    }
    else if (strcmp(error, "critical") == 0)
    {
        Error = SERVICE_ERROR_CRITICAL;
    }
    else if (strcmp(error, "ignore") == 0)
    {
        Error = SERVICE_ERROR_IGNORE;
    }

    // Get a handle to the service.

    hService = OpenService(
        hSCManager,            // SCM database 
        NameOfService,               // name of service 
        SERVICE_CHANGE_CONFIG);  // need change config access 

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    //Modify the start type of service
    if (!ChangeServiceConfig(hService, ServiceType, StartType, Error, BinaryPath, NULL, NULL, NULL, NULL, PassW, Display)) {
        printf("ChangeServiceConfig failed (%d)\n", GetLastError());
    }
    //BOOL result = ChangeServiceConfig(hService, ServiceType, StartType, Error, BinaryPathName, NULL, NULL, NULL, NULL, password, DisplayName);
    else {

        printf("Service configuration changed successfully.\n");
    }

    //if (!result)
    //{
    //    cout << "Error: Failed to modify the service configuration." << endl;
    //    printf("ChangeServiceConfig failed (%d)\n", GetLastError());
    //    CloseServiceHandle(hService);
    //    CloseServiceHandle(hSCManager);
   // }

cleanup:
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
void __stdcall DoStartSvc()
{
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
void __stdcall StopSvc()
{

    SERVICE_STATUS_PROCESS ssp;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    DWORD dwTimeout = 30000; // 30-second time-out
    DWORD dwWaitTime;

    // Get a handle to the SCM database. 

    hSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
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
        SERVICE_STOP |
        SERVICE_QUERY_STATUS |
        SERVICE_ENUMERATE_DEPENDENTS);

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Make sure the service is not already stopped.

    if (!QueryServiceStatusEx(
        hService,
        SC_STATUS_PROCESS_INFO,
        (LPBYTE)&ssp,
        sizeof(SERVICE_STATUS_PROCESS),
        &dwBytesNeeded))
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        goto stop_cleanup;
    }

    if (ssp.dwCurrentState == SERVICE_STOPPED)
    {
        printf("Service is already stopped.\n");
        goto stop_cleanup;
    }

    // If a stop is pending, wait for it.

    while (ssp.dwCurrentState == SERVICE_STOP_PENDING)
    {
        printf("Service stop pending...\n");

        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 

        dwWaitTime = ssp.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        if (!QueryServiceStatusEx(
            hService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            goto stop_cleanup;
        }

        if (ssp.dwCurrentState == SERVICE_STOPPED)
        {
            printf("Service stopped successfully.\n");
            goto stop_cleanup;
        }

        if (GetTickCount() - dwStartTime > dwTimeout)
        {
            printf("Service stop timed out.\n");
            goto stop_cleanup;
        }
    }

    // If the service is running, dependencies must be stopped first.

    StopDependentServices();

    // Send a stop code to the service.

    if (!ControlService(
        hService,
        SERVICE_CONTROL_STOP,
        (LPSERVICE_STATUS)&ssp))
    {
        printf("ControlService failed (%d)\n", GetLastError());
        goto stop_cleanup;
    }

    // Wait for the service to stop.

    while (ssp.dwCurrentState != SERVICE_STOPPED)
    {
        Sleep(ssp.dwWaitHint);
        if (!QueryServiceStatusEx(
            hService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            goto stop_cleanup;
        }

        if (ssp.dwCurrentState == SERVICE_STOPPED)
            break;

        if (GetTickCount() - dwStartTime > dwTimeout)
        {
            printf("Wait timed out\n");
            goto stop_cleanup;
        }
    }
    printf("Service stopped successfully\n");

stop_cleanup:
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
BOOL __stdcall StopDependentServices()
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if (EnumDependentServices(hService, SERVICE_ACTIVE,
        lpDependencies, 0, &dwBytesNeeded, &dwCount))
    {
        // If the Enum call succeeds, then there are no dependent
        // services, so do nothing.
        return TRUE;
    }
    else
    {
        if (GetLastError() != ERROR_MORE_DATA)
            return FALSE; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

        if (!lpDependencies)
            return FALSE;

        __try {
            // Enumerate the dependencies.
            if (!EnumDependentServices(hService, SERVICE_ACTIVE,
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount))
                return FALSE;

            for (i = 0; i < dwCount; i++)
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService(hSCManager,
                    ess.lpServiceName,
                    SERVICE_STOP | SERVICE_QUERY_STATUS);

                if (!hDepService)
                    return FALSE;

                __try {
                    // Send a stop code.
                    if (!ControlService(hDepService,
                        SERVICE_CONTROL_STOP,
                        (LPSERVICE_STATUS)&ssp))
                        return FALSE;

                    // Wait for the service to stop.
                    while (ssp.dwCurrentState != SERVICE_STOPPED)
                    {
                        Sleep(ssp.dwWaitHint);
                        if (!QueryServiceStatusEx(
                            hDepService,
                            SC_STATUS_PROCESS_INFO,
                            (LPBYTE)&ssp,
                            sizeof(SERVICE_STATUS_PROCESS),
                            &dwBytesNeeded))
                            return FALSE;

                        if (ssp.dwCurrentState == SERVICE_STOPPED)
                            break;

                        if (GetTickCount() - dwStartTime > dwTimeout)
                            return FALSE;
                    }
                }
                __finally
                {
                    // Always release the service handle.
                    CloseServiceHandle(hDepService);
                }
            }
        }
        __finally
        {
            // Always free the enumeration buffer.
            HeapFree(GetProcessHeap(), 0, lpDependencies);
        }
    }
    return TRUE;
}
void __stdcall scFAIL()
{
    // Get a handle to the SCM database. 

    LPSERVICE_FAILURE_ACTIONS lpsd = NULL;
    DWORD dwBytesNeeded, cbBufSize, dwError;

    hSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == hSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    //open a servuce
    hService = OpenService(
        hSCManager,          // SCM database 
        NameOfService,             // name of service 
        SERVICE_QUERY_CONFIG); // need query config access 

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Get the configuration information.
    /*This function is called initially with NULL and 0 for the lpBuffer and cbBufSize parameters, respectively.
    This is a common technique to query the size of the buffer needed to hold the service configuration information.
    Since the buffer size (cbBufSize) is zero, the function is expected to fail with ERROR_INSUFFICIENT_BUFFER,
    and GetLastError() should return this value, indicating how much memory needs to be allocated.

    If GetLastError() returns ERROR_INSUFFICIENT_BUFFER, the code proceeds to allocate a buffer of the
    required size (dwBytesNeeded) to hold the service failure actions configuration. This buffer is
    pointed to by lpsd, which is cast to a LPSERVICE_FAILURE_ACTIONS pointer.

    If any other error is encountered, it prints an error message indicating that
    QueryServiceConfig2 failed, along with the error code. It then cleans up by closing
    the open service and service control manager handles and presumably exits the error
    path of the function or block.
    */

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_FAILURE_ACTIONS,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_FAILURE_ACTIONS)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig failed (%d)", dwError);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
        }
    }

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_FAILURE_ACTIONS,
        (LPBYTE)lpsd,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig failed (%d)", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
    }

    //memory is now properly allocated and this is expected to work now 
   /* if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_DESCRIPTION,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_FAILURE_ACTIONS)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig2 failed (%d)", dwError);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
        }
    }

    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_DESCRIPTION,
        (LPBYTE)lpsd,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig2 failed (%d)", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
    }
*/
/*Further Actions: typical next steps would involve inspecting the
lpsd structure to read or modify the service's failure actions (like restart on failure, run a program, etc.),
and possibly calling ChangeServiceConfig2 if modifications are needed. Finally, it would involve cleaning
up allocated resources and closing any open handles.*/

// need to store findings from earlier into new ones

//10 arguments, but only need type, start, error
// don't need others because should be able to plug it in bc it is string
/*/*typedef struct _SERVICE_FAILURE_ACTIONSA {
DWORD     dwResetPeriod;
LPSTR     lpRebootMsg;
LPSTR     lpCommand;
DWORD     cActions;
SC_ACTION *lpsaActions;
} SERVICE_FAILURE_ACTIONSA, *LPSERVICE_FAILURE_ACTIONSA;*/

//restoring variables like config 

    DWORD ResetPeriod = lpsd->dwResetPeriod;
    LPWSTR RebootMsg = lpsd->lpRebootMsg;
    LPWSTR FAcommand = lpsd->lpCommand;
    DWORD FActions = lpsd->cActions;

    SERVICE_FAILURE_ACTIONS sfa;
    sfa.lpsaActions = lpsd->lpsaActions;

    CloseServiceHandle(hService);

    /*Further Actions: typical next steps would involve inspecting the
    lpsd structure to read or modify the service's failure actions (like restart on failure, run a program, etc.),
    and possibly calling ChangeServiceConfig2 if modifications are needed. Finally, it would involve cleaning
    up allocated resources and closing any open handles.*/

    wchar_t wtext[256];
    size_t outSize;

    wchar_t wtext1[256];
    size_t outSize1;

    //reset
    //length of period (in seconds) with no failures after which failure count reset to 0
    if (strlen(reset) != 0) // This checks if the string reset is not empty. strlen returns the length of the string excluding the null terminator. If the length is not zero, it means reset contains some characters.
    {
        sscanf_s(reset, "%d", &ResetPeriod, sizeof(ResetPeriod));
        //read an integer value from the string reset. 
    }
    //This assigns the value stored in ResetPeriod to the dwResetPeriod member of the sfa structure
    sfa.dwResetPeriod = ResetPeriod;

    //reboot
    //this is a message broadcast... whatever i type into command line is what gets submitted 
    if (strlen(reboot) != 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(reboot) + 1, reboot, strlen(reboot));
        RebootMsg = wtext;
    }
    sfa.lpRebootMsg = RebootMsg;

    //command
    if (strlen(command) != 0)
    {
        mbstowcs_s(&outSize1, wtext1, strlen(command) + 1, command, strlen(command));
        FAcommand = wtext1;
    }
    sfa.lpCommand = FAcommand;

    //actions
        //separate through forward slash
        // Valid actions are run, restart, and reboot.
        //Use actions= "" to take no action a service fails. Note that this parameter requires the reset= parameter.
        //the SC_ACTION_TYPE enumeration type.
        //SC_ACTION_NONE -No action.
        //SC_ACTION_REBOOT - Reboot the computer.
        //SC_ACTION_RESTART - Restart the service.
        //SC_ACTION_RUN_COMMAND - Run a command.

    if (strlen(action) != 0)
    {
        SC_ACTION_TYPE actions[6]; // Maximum of 6 actions
        DWORD delayt[6];
        const int maxActions = sizeof(actions) / sizeof(actions[0]);
        char* Iter_Action; //iterate through each action item
        // used in a loop to iteratively extract all tokens from the original string. 
        // After the first call, strtok_s should be called with the first argument set to action to signal that next command should continue on the same string:
        char actionCopy[256]; // Ensure this is large enough for your input string
        strncpy_s(actionCopy, sizeof(actionCopy), action, _TRUNCATE);

        char* CommACTION = strtok_s(actionCopy, "/", &Iter_Action);
        int actionCount = 0;

        while (CommACTION != NULL && actionCount < maxActions)
        {
            printf("%s =", CommACTION, &Iter_Action);
            if (strcmp(CommACTION, "run") == 0) {
                actions[actionCount] = SC_ACTION_RUN_COMMAND;
            }
            else if (strcmp(CommACTION, "restart") == 0) {
                actions[actionCount] = SC_ACTION_RESTART;
            }
            else if (strcmp(CommACTION, "reboot") == 0) {
                actions[actionCount] = SC_ACTION_REBOOT;
            }
            else if (strcmp(CommACTION, "none") == 0) {
                actions[actionCount] = SC_ACTION_NONE;
            }

            CommACTION = strtok_s(NULL, "/", &Iter_Action); // Move to delay value
            if (CommACTION != NULL)
            {
                printf(" %s\n", CommACTION);
                delayt[actionCount] = atoi(CommACTION);
                CommACTION = strtok_s(NULL, "/", &Iter_Action);

                actionCount = actionCount + 1;
            }
            //CommACTION = strtok_s(NULL, "/", &Iter_Action); // Move to the next action type
        }



        // At this point, `actions` array is populated with up to 6 actions and their delays.
        // You can now use this information to configure service failure actions as needed.
        // Example of printing the actions for verification:

        if (actionCount != 0)
        {
            FActions = actionCount;
            SC_ACTION failActions[3];
            for (int j = 0; j < actionCount; j++)
            {
                failActions[j].Type = actions[j];
                failActions[j].Delay = delayt[j];
            }
            sfa.lpsaActions = failActions;
        }

    }
    sfa.cActions = FActions;

    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfig2a
    //QueryServiceConfig2 has failure characters 
    //changeserviceConfig
    //https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-changeserviceconfig2a

// Get a handle to the service.

    hService = OpenService(
        hSCManager,            // SCM database 
        NameOfService,               // name of service 
        SERVICE_CHANGE_CONFIG);  // need change config access 

    if (hService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Update the failure configuration

    if (!ChangeServiceConfig2(
        hService,                 // handle to service
        SERVICE_CONFIG_FAILURE_ACTIONS, // change: failure
        &sfa))                      // new description
    {
        printf("ChangeServiceConfig2 failed\n");
    }
    else printf("Service description updated successfully.\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

}
