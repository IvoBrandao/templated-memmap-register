# Memory-Mapped Register Library

This is a C++ library for working with memory-mapped registers in embedded systems. It provides a flexible and type-safe way to access and manipulate registers of various sizes.

## Features

- Support for memory-mapped registers of 8, 16, 32, and 64 bits.
- Access control for read-only, write-only, and read-write registers.
- Bitfield access for individual bits within registers.
- Type-safe conversions between enums and their underlying types.

## Usage

To use this library in your embedded project, follow these steps:

1. Clone the repository to your development environment:

    ```sh
    git clone https://github.com/IvoBrandao/memory-mapped-register-library.git
    ```

2. Include the Library Header

    To access the library's functionality in your C++ source code, include the `mmregister.h` header file at the beginning of your source files:

    ```cpp 
    #include "mmregister.h"
    ```

3. Define and Initialize Registers

    Use the Register class to define and initialize memory-mapped registers. Specify the register size (in bits) as a template parameter and provide the memory address of the register as an argument to the constructor. Here's an example of defining a 32-bit read-write register at memory address `0x40000000`:

    ```cpp
    Register<32> myRegister(0x40000000);
    ```

4. Reading Register Values

    To read the value of a register, simply use the register variable in your code. For example:

    ```cpp
    // Read the value of myRegister
    uint32_t value = myRegister;
    ```
    
    This will store the current value of the register in the value variable

5. Writing Register Values  
    
    To write a value to a register, use the assignment operator (=) with the register variable. For example:

    ```cpp
    // Write the value 0x12345678 to myRegister
    myRegister = 0x12345678;
    ```
    This will update the register's value to 0x12345678.

6. Accessing Individual Bits

    The library provides a Bitfield class for accessing individual bits within registers. You can use the Bitfield class to read, write, and manipulate specific bits. Here's an example:

    ```cpp
    // Example usage for a 16-bit read-write register at address 0x40000004
    Register<16> statusRegister(0x40000004);

    // Access bit 3 of the register
    Register<16>::Bitfield bit3 = statusRegister[3];

    // Read the value of bit 3
    bool isBitSet = bit3;

    // Set bit 3 to 1
    bit3 = 1;

    ```

    Use the Bitfield class to interact with individual bits when needed.


## License
This library is provided under the MIT License. See the LICENSE file for details.