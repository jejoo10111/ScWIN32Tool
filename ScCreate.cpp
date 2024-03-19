#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <winsvc.h> //scm database handle .... create a service
#include <string>

//global variables
TCHAR NameOfService[256];
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

//variables for arguments
DWORD CreateType = SERVICE_WIN32_OWN_PROCESS;
DWORD StartType = SERVICE_DEMAND_START;
DWORD Error = SERVICE_ERROR_NORMAL;

void __stdcall sccreate(void); //sc create

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
       NameOfService,              //Name of Service 
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

int main(int argc, char* argv[]) //takes in command line input // argccontains count of arguments that follow in argv... argv representing command-line arguments entered by the user of the program
{
    //https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-170

     //create variable 
    size_t outSize;
    size_t outSize1;
    wchar_t wtext1[100];
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

    int i;


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
            mbstowcs_s(&outSize, NameOfService, strlen(argv[2]) + 1, argv[2], _TRUNCATE);
            mbstowcs_s(&outSize2, BinaryPathName, strlen(argv[4]) + 1, argv[4], _TRUNCATE);
            sccreate();
            return 0;
        }
        // has 10 options 
        //if greater than 3 
        else
        {
            for (i = 5; i < argc; i = i+2)
            {
                mbstowcs_s(&outSize, NameOfService, strlen(argv[2]) + 1, argv[2], _TRUNCATE); //name of service
                mbstowcs_s(&outSize2, BinaryPathName, strlen(argv[4]) + 1, argv[4], _TRUNCATE); //binPath = <BinaryPathName to the.exe file>

                //options  A space is required between the equal sign and the value.
                //type = <own | share | interact | kernel | filesys | rec | userown | usershare> (default = own)

                if (strcmp(argv[i], "type=") == 0)
                {
                    //copy the string+1 if it equals "type="
                    strncpy_s(type, argv[i+1], sizeof(type));
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
                    mbstowcs_s(&outSize1, displayName, strlen(argv[i]) + 1, argv[i+1], _TRUNCATE);
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

    // Body of the program
    return 0; // Indicates successful execution
}
