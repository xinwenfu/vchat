# EXE Configuration and Building
0. Download Visual Studio
0. Open Visual Studio 
1. Create a Empty `C++` Console project

    <img src="Images/I1.png">

2. Set the project to be a 32-bit x86 executable
   1. Open the configuration manager from the **Build** tab

        <img src="Images/I2.png">

    2. Select Win32 for a `X86` program in both the platform and Active Solution Platform 

        <img src="Images/I3.png">

3. Configure the Project's C++ compiler options (It compiles the C too!)
   1. Disable Control Flow Guard

        <img src="Images/I4.png">

    2. Set the Excepton Hander used to be SEH

        <img src="Images/I5.png">

    3. Set EH Continuation Metadata to No

        <img src="Images/I6.png">

    4. Disable Signed Returns 

        <img src="Images/I7.png">

    5. Disable  *Remove Unreferenced Code and Data*

        <img src="Images/I8.png">
    
    6. Disable SDL checks 

        <img src="Images/I9.png">
    
    7. Disable Security Checks 

        <img src="Images/I10.png">

    8. Disable the Debug Information 

        <img src="Images/I16.png">

    9. Disable Support Just My Code Debugging 

        <img src="Images/I17.png">

    10. Make sure we do not validate stack frames 

        <img src="Images/I18.png">

    11. Change the Runtime Library to Multi-Threaded DLL (No longer Debug)

        <img src="Images/I19.png">

4. Modify the Linker settings in the properties page
   1. Disable Randomized Base Address 

        <img src="Images/I11.png">
   
   2. Set References to `/OPT:NOREF`

        <img src="Images/I12.png">
   
   3. Ensure the target machine is x86!

        <img src="Images/I13.png">

   4. Disable DEP

        <img src="Images/I14.png">
   
   5. Disable SAFE SEH

        <img src="Images/I15.png">

5. Once you have generated the DLL configure the project to look for the `.a` file at a specified path.
   1. In `Linker -> Input -> Additional Dependencies` add *libessfunc.a* as one of the additional dependencies.

        <img src="Images/I20.png">

   2. In `Linker -> General -> Additional Library Directories` add the paths we can search for the *libessfunc.a* file in.

        <img src="Images/I21.png">

6. Build the project!
7. For the project to run you will need to copy the DLL over to the same directory as the EXE. If you make modifications to the DLL you will need to copy it over too.