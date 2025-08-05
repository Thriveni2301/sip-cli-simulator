# SIP CLI Simulator (WIP)

A simple command-line SIP client written in C++ that demonstrates the core SIP signaling flow:
`INVITE` → `100 Trying` → `180 Ringing` → `200 OK` → `ACK` → media → `BYE`.

This is intended for learning, testing, or automation of SIP signaling using custom UDP sockets without any heavy SIP libraries.

---

## Features (Currently Working)

- Sends SIP `INVITE` to destination SIP server
- Handles incoming `100 Trying`, `180 Ringing`, `200 OK`
- Sends proper `ACK` and `BYE`
- Uses raw UDP sockets, fully custom message construction

---

## Build Instructions

```bash
g++ -std=c++17 -o sipcli src/main.cpp src/SIPMessageBuilder.cpp src/UDPSocket.cpp src/ConfigLoader.cpp


