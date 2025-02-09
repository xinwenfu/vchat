## DLL Configuration and Building
1. Download Visual Studio
2. Open Visual Studio 
3. Click Create a new Project

    <img src="Images/I3.png">

4. Select Dynamic-Link Library (DLL)

    <img src="Images/I4.png">

5. Select a location to store the project

    <img src="Images/I5.png">

6. There may be a few pre-generated files as shown below, you can remove the `dllmain.cpp`, `pch.cpp`. and `pch.h`

    <img src="Images/I6.png">

7. We can disable the use of precompiled headers in the following manner
   1. Open the Project Properties window: Project -> Properties

        <img src="Images/I7.png">

   2. Navigate to the Precompiled Headers options: C/C++ -> Precompiled Headers 

        <img src="Images/I8.png">

   3. Select "Not Using Precompiled Headers

        <img src="Images/I9.png">

8. Now we should specify that this project will be compiled to x86, not x86-64 as may be the default
   1. Click on *Properties* in the Solution Explorer, or in the *Project* dropdown as shown previously

        <img src="Images/I10.png">

   2. Select *Platform* and *Win32*

        <img src="Images/I11.png">

9. Now we can configure the *Linker*
   1.  Open the Linker *All Options* table as shown below

        <img src="Images/I12.png">

   2. Set the preferred base address

        <img src="Images/I13.png">

   3. Disable DEP

        <img src="Images/I14.png">

   4. Set Incremental Linking to NO - It does not appear we did this with the GCC version

        <img src="Images/I15.png">

   5. Set SAFE-SEH to No

        <img src="Images/I16.png">

   6. Modify the Import Library Name

        <img src="Images/I17.png">

   7. Disable randomized base addresses

        <img src="Images/I18.png">

10. Now set the C++ compiler options 
    1. Set the compiler to not validate stack frames 

        <img src="Images/I26.png">

    2. Set the compiler to use SEH exceptions 

        <img src="Images/I19.png">

    3. Disable Security Checks 

        <img src="Images/I20.png">

    4. Disable Control Flow Guards 

        <img src="Images/I21.png">

    5. Disable SDL checks

        <img src="Images/I23.png">

    6.  Set the compiler to generate C code 

        <img src="Images/I22.png">

    7. Set the compiler to not remove unreferenced code

        <img src="Images/I27.png">

    8. Disable Support Just My Code Debugging

        <img src="Images/I28.png">

    9. Set to use the Multi-Threaded DLL (No longer the DEBUG version) for runtime linking 

        <img src="Images/I29.png">

11. Build the project!
    1.  Select the *Build* tab and click on **Build Solution** or **Build Essfun**

        <img src="Images/I24.png">

    2. Open the output folder you can see where this is by looking in the properties window and configuration manager

        <img src="Images/I25.png">
