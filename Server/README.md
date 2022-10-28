# Vulnerable chat server (vchat)

Our vulnerable chat server (vchat) is based on [vulnserver](http://thegreycorner.com/vulnserver.html). Here are the major changes
- Added the chat server functionality. The chat server receives messages from clients. Each message is forwarded to all other clients.
- CTRL+C to exit the server gracefully

Click vchat.exe to start the server.

How to perform chat via vchat.

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/fIcf5A0CCHU/0.jpg)](https://youtu.be/fIcf5A0CCHU)

## QuickEdit Mode and Copy and Paste in CMD

We have to disable the [QuickEdit Mode](https://stackoverflow.com/questions/48192465/accept-blocks-with-pending-connections-ctrlc-unblocks) and actually the Mark and Copy functionality of the Windows console too. Otherwise, the text selection mode halts the running program, e.g., our chat server. Ctrl+C can cancel the mode. However, it is really annoying. We now disable the QuickEdit Mode in the code. 

The *Mark* and *Copy* functionality of the console can still cause trouble if they are used by people.


## To-Do
1. Makefile for easy compilation during testing [ref](https://stackoverflow.com/questions/714100/os-detecting-makefile)
2. use preprocessor directive [ifdef](https://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch) to make file compile with differnt code depedning on system (automatic)
    1. Create stub program 
    2. Test windows compilation -- does it still work
    3. Add linux libraries to linux section (Equivalent to windows used)
    4. Replace Windows function calls
    5. Test  
