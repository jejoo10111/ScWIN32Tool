#pragma once
#include <stdio.h>
#include <Windows.h>
#include <winsvc.h> //scm database handle .... create a service


//https://github.com/MicrosoftDocs/win32/blob/docs/desktop-src/Services/changing-a-service-configuration.md

//global variables
LPCWSTR NameOfService; //ichanged this from TCHAR to LPCWSTR if something stops working!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
TCHAR displayName[256];
TCHAR BinaryPathName[256];

//global variables for argument/options types
CHAR type[100];
CHAR start[100];
CHAR error[100];
LPCWSTR DisplayName = NULL;
LPCWSTR password = NULL;
LPCWSTR group = NULL;
LPCWSTR obj = NULL;
LPCWSTR depend = NULL;
LPCWSTR binpath = NULL;

//variables for arguments
DWORD ServiceType = SERVICE_WIN32_OWN_PROCESS;
DWORD StartType = SERVICE_DEMAND_START;
DWORD Error = SERVICE_ERROR_NORMAL;

void __stdcall scconfig(void); //sc config

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

int main(int argc, char* argv[]) //takes in command line input // argccontains count of arguments that follow in argv... argv representing command-line arguments entered by the user of the program
{
    //https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-170

     //create variable
    size_t outSize;
    size_t outSize1;
    wchar_t wtext1[100];
    size_t outSize2;
    wchar_t wtext2[100];
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

    // Body of the program
    return 0; // Indicates successful execution
}
