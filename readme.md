# Vulnerable Chat Server (vchat)

The vulnerable chat server (vchat) serves multile purposes
- It is a working chat server with a chat client written in Python. We use the chat server to send messages betweeen each other. For example, base64 encoded encrypted data can be sent over the chat server.
- It is a server with various vulnerabilities that can be exploited on Windows 10/11. For example, we developed the buffer overflow, DoS and DDoS attack modules for Armitage and Metasploit. A few clicks can hack into a victim Windows running vchat.

## Table of Content

* [Server](Server): Revised [vulnserver](http://thegreycorner.com/vulnserver.html) as the chat server
  - The whole repository shall be downloaded to a Windows computer to use the vulnerable chat server, *vchat.exe*.
* [Client](Client): A Python chat client
  - The repository can be downloaded to either a Linux computer such as Kali or a Windows computer to use the chat client, *client.py*.
* [Attack](Attack): Attacks against the chat server
  - The attack code here is in Python and can be used on Linux or Windows.

## Downloading

- Click Code -> Download ZIP -> Unzip to a folder
- If <a href="https://git-scm.com/">*git*</a> is intalled on your computer, the repository can be cloned to your computer. To install with git, use the command
```
git clone https://github.com/xinwenfu/vchat.git
```
