# Vulnerable Chat Server (vchat)

This is the Vulnerable Chat (VChat) server which serves multiple purposes.
- It is a working chat server with a chat client written in Python. We use the chat server to send messages between multiple uses on the Chat client. For example, base64 encoded encrypted data can be sent over the chat server between users, in addition to plain text messages.
- It is a server with various vulnerabilities that can be exploited on Windows 10/11. For example, we developed the <a href="https://github.com/xinwenfu/Malware-Analysis/tree/main/MetasploitNewModule">buffer overflow</a>, <a href="https://github.com/DaintyJet/TA-GenCyber-UML-2022/tree/main/Vulnserver-Modules">DoS and DDoS</a> attack modules for Armitage and Metasploit. A few clicks and a person can hack into a victim Windows machine running VChat.

## Table of Content

* [Server](./Server): Revised [vulnserver](http://thegreycorner.com/vulnserver.html) as the chat server
  - The whole repository shall be downloaded to a Windows computer to use the vulnerable chat server, *vchat.exe*.
* [Client](./Client): A Python chat client
  - The repository can be downloaded to either a Linux computer such as Kali or a Windows computer to use the chat client, *client.py*.
* [Attack](./Attack): Attacks against the chat server
  - The attack code here is in Python and can be used on Linux or Windows.
  - Refer to  <a href="https://github.com/xinwenfu/Malware-Analysis/tree/main/MetasploitNewModule">buffer overflow<a/>, <a href="https://github.com/DaintyJet/TA-GenCyber-UML-2022/tree/main/Vulnserver-Modules">DoS and DDoS</a> attack modules for Armitage and Metasploit.
* [GUI](GUI)
## Downloading

- Click Code -> Download ZIP -> Unzip to a folder
- If <a href="https://git-scm.com/">*git*</a> is intalled on your computer, the repository can be cloned to your computer. To install with git, use the command
```
git clone https://github.com/xinwenfu/vchat.git
```
