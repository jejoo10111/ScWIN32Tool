# ScWIN32Tool Reproduction

## Description:

This project was created to understand how to code in C++ and understand the research and process in replicating binaries by using WIN32 APIs. 

This is a C++ build of a native Windows Tool, sc.exe (service control) , using win32 APIs. This build will attempt to replicate switches: start, stop, failure, create, config, query, qdescription, and delete. Due to the difficulties in implementing a remote connection to a remote server, this build will not have Remote server commands and should run on your local computer. 

A note will be written here: this build has a few difficulties connecting to certain services due to access issues and have been noted in the execution of the process. Additionally, when this project was started, it was tackled one switch at a time. As a result, one large source.cpp script was created combining all the switches together. However, in the process of combining my **scquery.cpp** switch was not transferred accordingly. Therefore, a separate **query.exe** is published onto this GitHub to be executed separately.

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

The features in this tool are discussed below. An example to utilize the command for these switches are also provided.
commands can be written as : [Name of Executable] [command]

### Start

The start command starts a service. The command can be written as 

scWin32.exe scstart [Service Name]

### Stop

The stop command sends a STOP control request to a service; stops the service from running.

scWin32.exe scstop [Service Name]

### Failure

The failure command specifies one or more actions to be taken if a particular service fails.

scWin32.exe scfailure [Service Name] <option1><option2><option3>

### Create

The create command creates a subkey and entries for the service in the registry and in the service control manager database.

scWin32.exe sccreate [Service Name] binpath= "" type= "" start= "" ...


### Config

The config command modifies the value of a service's entries in the registry and in the service control manager database.

scWin32.exe scconfig [Service Name] binpath= "" type= "" start= "" ...

### Query

The query command obtains and displays information about the specified service, driver, type of service, or type of driver.

scWin32.exe scquery [Service Name] 
scWin32.exe scquery

### Qdescription

The qdescription command displays a service's description string.

scWin32.exe scqdescription [Service Name] 

### Delete

The delete command deletes a service subkey from the registry.

scWin32.exe scdelete [Service Name] 

6. Include Credits
If you worked on the project as a team or an organization, list your collaborators/team members. You should also include links to their GitHub profiles and social media too.

Also, if you followed tutorials or referenced a certain material that might help the user to build that particular project, include links to those here as well.

This is just a way to show your appreciation and also to help others get a first hand copy of the project.

