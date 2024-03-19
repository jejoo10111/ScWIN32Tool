#pragma once
#include <stdio.h>
#include <Windows.h>
#include <winsvc.h> //scm database handle ....
#include <wchar.h>
#include <tchar.h>

//https://learn.microsoft.com/en-us/windows/win32/services/svcconfig-cpp
//https://learn.microsoft.com/en-us/windows/win32/services/querying-a-service-s-configuration


LPCWSTR NameOfService;
SC_HANDLE hSCManager;
SC_HANDLE hService;

void __stdcall Qdes();

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

int main(int argc, char* argv[])
{
    int i;
    wchar_t wtext[100];//Name of Service is a LPCWSTR so need to convert the string
    size_t outSize;

    if (strcmp(argv[1], "scqdescription") == 0)
    {
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        NameOfService = wtext;
        Qdes();
        return 0;
    }

    return 0;
}

