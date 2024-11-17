# Threaded-Shannon-Encoder
Objective:
This assignment will introduce you to interprocess synchronization mechanisms in UNIX using named POSIX semaphores, pthread mutex semaphores, and pthread condition variables.

Problem:
For this assignment, you will modify your solution for programming assignment 1 to comply with the restrictions explained below.

Your multithreaded version to determine the Shannon codes and the encoded message must execute the following steps:
 
Read the input from STDIN (the Moodle server will implement input redirection to send the information from a file to STDIN). 
Create n POSIX threads (where n is the number of lines from the input). Each child thread executes the following tasks:
Receives the original message.
Creates the alphabet based on the received input string.
Sorts the alphabet in decreasing order based on the symbols' frequency. If two or more symbols have the same frequency, you must sort them in decreasing order based on their ASCII value.
Determines the Shannon codes for the symbols in the alphabet.
Generates the encoded message by using the Shannon codes.
Prints the original message, the alphabet, and the encoded message. 

Given the following input:
COSC 3360 COSC 1437
COSC 1336 COSC 2436
COSC 3320 COSC 3380
 
The expected output is:
Message: COSC 3360 COSC 1437

Alphabet: 
Symbol: C, Frequency: 4, Shannon code: 000
Symbol: 3, Frequency: 3, Shannon code: 001
Symbol:  , Frequency: 3, Shannon code: 010
Symbol: S, Frequency: 2, Shannon code: 1000
Symbol: O, Frequency: 2, Shannon code: 1010
Symbol: 7, Frequency: 1, Shannon code: 10111
Symbol: 6, Frequency: 1, Shannon code: 11001
Symbol: 4, Frequency: 1, Shannon code: 11010
Symbol: 1, Frequency: 1, Shannon code: 11100
Symbol: 0, Frequency: 1, Shannon code: 11110

Encoded message: 00010101000000010001001110011111001000010101000000010111001101000110111

Message: COSC 1336 COSC 2436

Alphabet: 
Symbol: C, Frequency: 4, Shannon code: 000
Symbol: 3, Frequency: 3, Shannon code: 001
Symbol:  , Frequency: 3, Shannon code: 010
Symbol: S, Frequency: 2, Shannon code: 1000
Symbol: O, Frequency: 2, Shannon code: 1010
Symbol: 6, Frequency: 2, Shannon code: 1011
Symbol: 4, Frequency: 1, Shannon code: 11010
Symbol: 2, Frequency: 1, Shannon code: 11100
Symbol: 1, Frequency: 1, Shannon code: 11110

Encoded message: 000101010000000101111000100110110100001010100000001011100110100011011

Message: COSC 3320 COSC 3380

Alphabet: 
Symbol: C, Frequency: 4, Shannon code: 000
Symbol: 3, Frequency: 4, Shannon code: 001
Symbol:  , Frequency: 3, Shannon code: 011
Symbol: S, Frequency: 2, Shannon code: 1001
Symbol: O, Frequency: 2, Shannon code: 1010
Symbol: 0, Frequency: 2, Shannon code: 1100
Symbol: 8, Frequency: 1, Shannon code: 11100
Symbol: 2, Frequency: 1, Shannon code: 11110

Encoded message: 0001010100100001100100111110110001100010101001000011001001111001100
