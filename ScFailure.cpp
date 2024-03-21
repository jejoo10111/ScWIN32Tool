#include <stdio.h>
#include <Windows.h>
#include <winsvc.h> //scm database handle ....

LPCWSTR NameOfService;
SC_HANDLE hSCManager;
SC_HANDLE hService;

CHAR reset[100] = "";
CHAR reboot[100] = "";
CHAR action[100] = "";
CHAR command[100] = "";

void __stdcall scFAIL();

//sc [<ServerName>] failure [<ServiceName>] [reset= <ErrorFreePeriod>] [reboot= <BroadcastMessage>] [command= <CommandLine>] [actions= {"" | {[run/<MS>] | [restart/<MS>] | [reboot/<MS>]}[/...]]

//https://microsoft.github.io/windows-docs-rs/doc/windows/Win32/System/Services/struct.SERVICE_FAILURE_ACTIONSW.html

void FailureCall()
{
    printf("\nDESCRIPTION:\n\tChanges the actions upon failure.\n");
    printf("\n\tUSAGE:\n\tsc failure [service name] <option1> <option2>...\n");
    printf("\n\tOPTIONS:\n");
    printf("\treset=  <Length of period of no failures (in seconds) after which to reset the failure count to 0 (may be INFINITE) > (Must be used in conjunction with actions = )\n");
    printf("\t reboot=  <Message broadcast before rebooting on failure>\n");
    printf("\tcommand= <Command line to be run on failure>\n");
    printf("\tactions=  <Failure actions and their delay time (in milliseconds), separated by / (forward slash) --e.g., run / 5000 / reboot / 800 Valid actions are <run | restart | reboot> > (Must be used in conjunction with the reset = option)\n");
}

//https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_failure_actionsa

/*typedef struct _SERVICE_FAILURE_ACTIONSA {
  DWORD     dwResetPeriod;
  LPSTR     lpRebootMsg;
  LPSTR     lpCommand;
  DWORD     cActions;
  SC_ACTION *lpsaActions;
} SERVICE_FAILURE_ACTIONSA, *LPSERVICE_FAILURE_ACTIONSA;*/

void __stdcall scFAIL()
{
    // Get a handle to the SCM database. 
    //declares a pointer to a SERVICE_FAILURE_ACTIONS structure and initializes it to NULL. 
    // The SERVICE_FAILURE_ACTIONS structure is used to specify the actions the Service Control Manager (SCM) should take if a service fails to start.
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
    //QueryServiceConfig2 retrieve the optional configuration information stored in the Service Control Manager (SCM) database for the specified service. 
    //query the size of the buffer needed to hold the service configuration information.
    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_FAILURE_ACTIONS,
        NULL,
        0,
        &dwBytesNeeded))
    {   
        //If GetLastError() returns ERROR_INSUFFICIENT_BUFFER, the code proceeds to allocate a buffer of the
        //required size (dwBytesNeeded) to hold the service failure actions configuration 
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_FAILURE_ACTIONS)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {   //If any other error is encountered, it prints an error message indicating that
            //QueryServiceConfig2 failed, along with the error code
            printf("QueryServiceConfig failed (%d)", dwError);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
        }
    }


    // The SERVICE_CONFIG_FAILURE_ACTIONS parameter indicates that the function should retrieve the failure actions configuration of the service.
    if (!QueryServiceConfig2(
        hService,
        SERVICE_CONFIG_FAILURE_ACTIONS, //failure actions config
        (LPBYTE)lpsd, //pointer to a buffer that receives failure actions
        cbBufSize, //size of pointer
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig failed (%d)", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
    }


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

    CloseServiceHandle(hService); //done qquerying

    /*Further Actions: typical next steps would involve inspecting the
    lpsd structure to read or modify the service's failure actions (like restart on failure, run a program, etc.),
    and possibly calling ChangeServiceConfig2 if modifications are needed.*/

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
    //Command line to be run on failure
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
        SC_ACTION_TYPE actions[6]; // Maximum of 3 actions/... aone failure cmes with a delay actions= [action1]/[delay1]/[action2]/[delay2]/[action3]/[delay3]
        DWORD delayt[6];
        const int maxActions = sizeof(actions) / sizeof(actions[0]);
        char* Iter_Action; //iterate through each action item
        // used in a loop to iteratively extract all actions from the original string. 
        // After the first call, strtok_s should be called with the first argument set to action to signal that next command should continue on the same string:
        char actionCopy[256]; // Ensure this is large enough for your input string
        strncpy_s(actionCopy, sizeof(actionCopy), action, _TRUNCATE);

        //called once to find the first actin type
        //based on forward slash 
        char* CommACTION = strtok_s(actionCopy, "/", &Iter_Action);
        int actionCount = 0;

        //loop continues as more actions are proccessed but less than the max allowed 
        while (CommACTION != NULL && actionCount < maxActions)
        {
            //prints current action 
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

            //mve to delay value 
            //NULL signals to continue iteration through the string and read the delay 
            CommACTION = strtok_s(NULL, "/", &Iter_Action); // Move to delay value
            //if command actin found, continue
            if (CommACTION != NULL)
            {   
                //prints the delay value
                printf(" %s\n", CommACTION);
                //convert delay valuye frm a string to an integer using ati and store in delay by action count
                delayt[actionCount] = atoi(CommACTION);
                //mve on to the next action 
                CommACTION = strtok_s(NULL, "/", &Iter_Action);

                //then count until the end of the action 
                actionCount = actionCount + 1;
            }
            //CommACTION = strtok_s(NULL, "/", &Iter_Action); // Move to the next action type
        }



        // At this point, `actions` array is populated with up to 6 actions and their delays.
        // You can now use this information to configure service failure actions as needed.
        // Example of printing the actions for verification:

        //are there statements that have been parsed and stred 
        if (actionCount != 0)
        {
            //assign number of actions
            FActions = actionCount;
            SC_ACTION failActions[6];//declare array for type and delay 
            for (int j = 0; j < actionCount; j++)
            {   //for each action, set the type and delay field of failactions array based n parsed data stored in actins and delayt
                failActions[j].Type = actions[j];
                failActions[j].Delay = delayt[j];
            }
            //assign the address to the structure sfa
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

int main(int argc, char* argv[])
{
    wchar_t wtext[100];//Name of Service is a LPCWSTR so need to convert the string 
    size_t outSize;
    int i = 0;

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

    return 0;
}
