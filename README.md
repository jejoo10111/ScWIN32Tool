# ScWIN32Tool Reproduction

## Description:

This project was created to understand how to code in C++ and understand the research and process in replicating binaries by using WIN32 APIs. 

This is a C++ build of a native Windows Tool, sc.exe (service control) , using win32 APIs. This build will attempt to replicate switches: start, stop, failure, create, config, query, qdescription, and delete. Due to the difficulties in implementing a remote connection to a remote server, this build will not have Remote server commands and should run on your local computer. 

A note will be written here: this build has a few difficulties connecting to certain services due to access issues and have been noted in the execution of the process. Additionally, when this project was started, it was tackled one switch at a time. As a result, one large source.cpp script was created combining all the switches together. However, in the process of combining my **scquery.cpp** switch was not transferred accordingly. Therefore, a separate **query.exe** is published onto this GitHub to be executed separately.

## Sysmon monitoring 
The SYSMON.xml is a sysmon configuration file to monitor the commands for the executables query.exe and scWin32.exe. If this configuration file is going to be used for your own monitoring, change the location of you the executable for the processes and the commands!!

## Updates for the Future
There are a few updates that need to be made to this tool for it to function appropriately and efficiently: 

1. This tool needs to update proper configuration into one large tool script.
2. Query: When querying services, for example services that have groups, some service accesses are denied even with SERVICES_ALL_ACCESS.
3. Create: The switch **Start**, is not fully functionable. Most of the traits that come with the start switch does work, but not all.
4. Config: The **type** and **start** switch are not fully functionable as well. The **type** switch <rec> SERVICE_RECOGNIZER_DRIVER does not configure properly. The **start** switch <system> SERVICE_SYSTEM_START does not configure as well.
   

## How To Install

This project was written and tested on a Windows 11 Home.

This project can be used by simply downloading the executables of the **source.cpp** and the **query.cpp**.

## How to Start

Once the executables are downloaded, you simply need to know the location of the executable. 

Open an Administrative Command Prompt and either navigate to the location of the executable or drag the location of the executable to your command prompt. To execute the switches properly scroll to the **FEATURES** section of this README.

## Features

The features in this tool are discussed below. An example to utilize the command for these switches are also provided. Whitespace is needed after each switch after the "=" sign.
commands can be written as : [Name of Executable] [command]

### Start

The start command starts a service. The command can be written as 

scWin32.exe scstart [Service Name]

scWin32.exe scstart GamesAppService

### Stop

The stop command sends a STOP control request to a service; stops the service from running.

scWin32.exe scstop [Service Name]

scWin32.exe scstop GamesAppService

### Failure

The failure command specifies one or more actions to be taken if a particular service fails.

scWin32.exe scfailure [Service Name] <option1><option2><option3>
[reset= <ErrorFreePeriod>] [reboot= <BroadcastMessage>] [command= <CommandLine>] [actions= {"" | {[run/<MS>] | [restart/<MS>] | [reboot/<MS>]}[/...]]

scWin32.exe scfailure SimpleService reset= 3600 reboot= "MyService crashed -- rebooting machine" actions= reboot/30000

### Create

The create command creates a subkey and entries for the service in the registry and in the service control manager database.

type= <own | share | interact | kernel | filesys | rec type= {own | share}> (default = own)
start= <boot | system | auto | demand | disabled | delayed - auto> (default = demand)
error= <normal | severe | critical | ignore> (default = normal)
binPath= <BinaryPathName to the.exe file> group= <LoadOrderGroup>
tag= <yes | no>
depend= <Dependencies(separated by / (forward slash))>
obj= <AccountName | ObjectName> (default = LocalSystem)
DisplayName= <display name>
password= <password>

scWin32.exe sccreate [Service Name] binpath= "" type= "" start= "" ...

scWin32.exe sccreate SimpleService binpath= "C:\path\to\your\service.exe"

### Config

The config command modifies the value of a service's entries in the registry and in the service control manager database.

type= <own | share | interact | kernel | filesys | rec type= {own | share}> (default = own)
start= <boot | system | auto | demand | disabled | delayed - auto> (default = demand)
error= <normal | severe | critical | ignore> (default = normal)
binPath= <BinaryPathName to the.exe file> group= <LoadOrderGroup>
tag= <yes | no>
depend= <Dependencies(separated by / (forward slash))>
obj= <AccountName | ObjectName> (default = LocalSystem)
DisplayName= <display name>
password= <password>

scWin32.exe scconfig [Service Name] binpath= "" type= "" start= "" ...

scWin32.exe scconfig SimpleService type= share

### Query

The query command obtains and displays information about the specified service, driver, type of service, or type of driver.

scWin32.exe scquery [Service Name] 

scWin32.exe scquery SimpleService
scWin32.exe scquery 

### Qdescription

The qdescription command displays a service's description string.

scWin32.exe scqdescription [Service Name] 

scWin32.exe scqdescription SimpleService

### Delete

The delete command deletes a service subkey from the registry.

scWin32.exe scdelete [Service Name] 

scWin32.exe scdelete SimpleService

## Credits

This project was assigned to me by Gordon Long at George Mason University for my Digital Forensics class. 

### References
Here are a list of resources that I have used that have assisted me in the creation of this project:
1. https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/cc742126(v=ws.11)
2. https://learn.microsoft.com/en-us/windows/win32/services/starting-a-service
3. https://github.com/MicrosoftDocs/win32/blob/docs/desktop-src/Services/stopping-a-service.md
4. https://microsoft.github.io/windows-docs-rs/doc/windows/Win32/System/Services/struct.SERVICE_FAILURE_ACTIONSW.html
5. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_failure_actionsa
6. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfig2a
7. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-changeserviceconfig2a
8. https://forums.codeguru.com/showthread.php?504620-OpenService-returns-Null-but-GetLastError-also-returns-Null
9. https://github.com/nixonion/sc/blob/master/sc/Source.cpp#L46
10. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicea
11. https://learn.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights
12. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicew
13. https://learn.microsoft.com/en-us/windows/win32/services/installing-a-service
14. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagera
15. https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-170
16. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-enumservicesstatusa
17. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_status
18. https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryservicestatusex
19. https://learn.microsoft.com/en-us/windows/win32/services/querying-a-service-s-configuration
20. https://learn.microsoft.com/en-us/windows/win32/services/svcconfig-cpp


