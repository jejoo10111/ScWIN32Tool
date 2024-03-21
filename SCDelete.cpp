#include <stdio.h>
#include <Windows.h>
#include <winsvc.h> //scm database handle .... 


LPCWSTR NameOfService;
SC_HANDLE hSCManager;
SC_HANDLE hService;

void __stdcall DeleteSvc();

//https://learn.microsoft.com/en-us/windows/win32/services/svcconfig-cpp
void __stdcall DeleteSvc()
{
    
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


int main(int argc, char* argv[])
{
    int i;
    wchar_t wtext[100];//Name of Service is a LPCWSTR so need to convert the string 
    size_t outSize;

    if (strcmp(argv[1], "scdelete") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        DeleteSvc();
        return 0;
    }

    return 0;
}
