# Arcfour Project

This project implements the RC4 stream cipher. It includes functions to initialize the cipher, encrypt data, and decrypt data.

## Features

- Initializes the RC4 cipher with a given key.
- Encrypts data using the RC4 stream cipher.
- Decrypts data using the RC4 stream cipher (encryption and decryption are symmetric operations in RC4).
- Includes a main program to demonstrate encryption and decryption.

## Files

- `main.c`: Contains the main functions for demonstrating encryption and decryption.
- `arcfour.c`: Implementation of the RC4 stream cipher.
- `arcfour.h`: Header file containing the necessary definitions and includes for the RC4 implementation.
- `Makefile`: Script to automate the build process.

## Usage

### Compilation

To compile the project, simply run:

```sh
make
```

### Running the Program

To run the program and see the RC4 encryption and decryption in action, use:

```sh
./example
```

### Example Output

The program will initialize the RC4 cipher, encrypt a sample text, and then decrypt it to show that the original text is recovered.

Replace `<key>` and `<plaintext>` in `main.c` to test with different inputs.

### Learn More

To read more about the RC4 cipher, visit the [Wikipedia page on RC4](https://en.wikipedia.org/wiki/RC4).
