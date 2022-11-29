EE450 Project
Name : Xiaofeng Pan


Reference: I use blocks of code from Beej's guide to network programming, in cluding bind and accept of TCP socket and sendto and recvfrom of UDP sockets.

Data exchanging:

Client: 
string paramters plainu and plainp ask user to input username and password and then was convert to char* msg to send to the Main Server via tcp socket.

-->

ServerM:

in server, parameter buf got the username and password at once by using tcp socket, and then pass to a self-developed encryption function call "cipher" and return a char[] paramter c. Which will be pass to ServerC to do the authentication via UDP socket ut.

-->

ServerC:
get cipher code by using udp socket sockfd with char[] buf. Then the ServerC will read the input file and see if we can verify the cipher code we got. if so, send 0 to Main server to tell verify sucessfully. Otherwise, send 1 for mssing usrname, send 2 for missing password and send 3 for missing both usrname and password.

-->

ServerM

got authentication message, if the message char[] buff not equal to 0, send tcp to client to let client know whats wrong correspondingly. Otherwise, send "Di" to client to let client know authetication sucessed via tcp.

--> 

Client:

Got from ServerM via tcp by char[] buf. If authentication failed, repeat former steps for remaining 1 or 2 chances. Otherwise, will be ask to input query and categories. Which will be store in  string coursename and cat, and then be convert to string ms with other code "77" or "78" to send to ServerM.(77 for EE, 78 for CS, just to tell ServerM which department server will be contact for.)

-->

ServerM:

ServerM got message from tcp by char[] buf. and will first analyst the code in messsage is whether 77 or 78 first. if 77, then send rest of the message to ServerEE, if code is 78, will send rest of the message to ServerCS.

-->

ServerEE\CS:

Department Server got message contain course name and category by usd socket, then will will use char* cs and ctg to store course name and category. Then read file to see if can find corresponding course and information. Then send back to ServerM with what it got, or "none" to show no result.

-->

ServerM:

When ServerM got the message via udp from departments, and send back to Client.

-->

Client:

got the result and print on the screen, and start new query. Won't be closed until user press ctrl C.