# Proxy Connection Project

This project establishes a connection to a proxy server, sends a request, and forwards data through the proxy. The code includes functions to create a request structure and connect to the proxy server.

## Features

- Dynamically loads the original `connect` function using `dlsym`.
- Creates a new socket for communication.
- Initializes the socket structure with the proxy server details.
- Connects to the proxy server.
- Creates a request using the `request` function.
- Sends the request to the proxy server.
- Reads the response from the proxy server.
- Checks if the connection through the proxy was successful.
- If successful, duplicates the socket to forward the data.

## Files

- `main.c`: Contains the main functions for creating a request and connecting to the proxy server.
- `toralize.h`: Header file containing the necessary definitions and includes.
- `toralize.so`: Shared library that is dynamically loaded.
- `Makefile`: Script to automate the build process.
