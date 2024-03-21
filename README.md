# ScWIN32Tool Reproduction

### Description:

This project was created to understand how to code in C++ and understand the research and process in replicating binaries by using WIN32 APIs. 

This is a C++ build of a native Windows Tool, sc.exe (service control) , using win32 APIs. This build will attempt to replicate switches: start, stop, failure, create, config, query, qdescription, and delete. Due to the difficulties in implementing a remote connection to a remote server, this build will not have Remote server commands and should run on your local computer. 

A note will be written here: this build has a few difficulties connecting to certain services due to access issues and have been noted in the execution of the process. Additionally, when this project was started, it was tackled one switch at a time. As a result, one large source.cpp script was created combining all the switches together. However, in the process of combining my scquery.cpp switch was not transferred accordingly. Therefore, a separate query.exe is published onto this GitHub to be executed separately.

### Updates for the Future
There are a few updates that need to be made to this tool for it to function appropriately and efficiently: 

1. This tool needs to update proper configuration into one large tool script.
2. Query
3. Create
4. Config
   

### How To Install

This project was written and tested on a Windows 11 Home.

This project can be used by simply downloading the executables of the source.cpp and the query.cpp.

### How to Start

Once the executables are downloaded, you simply need to know the location of the executable. 

Open an Administrative Command Prompt and either navigate to the location of the executable or drag the location of the executable to your command prompt. To execute the switches properly scroll to the FEATURES section of this README.

### Features


If your project has a lot of features, consider adding a "Features" section and listing them here.





5. How to Use the Project
Provide instructions and examples so users/contributors can use the project. This will make it easy for them in case they encounter a problem – they will always have a place to reference what is expected.

You can also make use of visual aids by including materials like screenshots to show examples of the running project and also the structure and design principles used in your project.

Also if your project will require authentication like passwords or usernames, this is a good section to include the credentials.

6. Include Credits
If you worked on the project as a team or an organization, list your collaborators/team members. You should also include links to their GitHub profiles and social media too.

Also, if you followed tutorials or referenced a certain material that might help the user to build that particular project, include links to those here as well.

This is just a way to show your appreciation and also to help others get a first hand copy of the project.

7. Add a License
For most README files, this is usually considered the last part. It lets other developers know what they can and cannot do with your project.

We have different types of licenses depending on the kind of project you are working on. Depending on the one you will choose it will determine the contributions your project gets.

The most common one is the GPL License which allows other to make modification to your code and use it for commercial purposes. If you need help choosing a license, use check out this link: https://choosealicense.com/

Up to this point what we have covered are the minimum requirements for a good README. But you might also want to consider adding the following sections to make it more eye catching and interactive.
