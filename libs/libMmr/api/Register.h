#ifndef MEMMAP_REGISTER_H_
#define MEMMAP_REGISTER_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

// Enable or disable debug prints
constexpr bool enableDebugPrints = true;

template <unsigned int size> struct RegisterTraits {};

template <> struct RegisterTraits<8> {
  typedef std::uint8_t PrimitiveType;
};

template <> struct RegisterTraits<16> {
  typedef std::uint16_t PrimitiveType;
};

template <> struct RegisterTraits<32> {
  typedef std::uint32_t PrimitiveType;
};

template <> struct RegisterTraits<64> {
  typedef std::uint64_t PrimitiveType;
};

class ReadOnly {};

class WriteOnly {};

class ReadWrite : public ReadOnly, public WriteOnly {};

template <std::size_t size, typename RegisterAccess = ReadWrite>
class Register {
  typedef typename RegisterTraits<size>::PrimitiveType BaseType;

public:
  // add a contructor that allows to instanciate the register without an address
  explicit Register() : raw_ptr{nullptr} {}

  explicit Register(std::uintptr_t address)
      : raw_ptr{reinterpret_cast<volatile BaseType *>(address)} {
    if (raw_ptr == nullptr) {
      throw std::invalid_argument("Invalid memory address for register.");
    }
    printDebug("Register constructor", read());
  }

  // Bit Proxy class for bit manipulation
  class BitProxy {
  public:
    BitProxy(Register &reg, std::size_t bit_position)
        : register_(reg), bit_position_(bit_position) {}

    // Read the bit
    operator bool() const { return register_.getBit(bit_position_); }

    // Write the bit
    BitProxy &operator=(bool value) {
      register_.setBit(bit_position_, value);
      return *this;
    }

  private:
    Register &register_;
    std::size_t bit_position_;
  };

  // Bit access operator [] - provides a proxy for bit manipulation
  BitProxy operator[](std::size_t bit_position) {
    assert(raw_ptr != nullptr);
    assert(bit_position < size);

    return BitProxy(*this, bit_position);
  }

  // Bit access operator [] - read
  bool getBit(std::size_t bit_position) const {
    assert(raw_ptr != nullptr);
    assert(bit_position < size);

    BaseType val = read();
    return ((val >> bit_position) & 0x01) == 0x01;
  }

  // Bit access operator [] - write
  void setBit(std::size_t bit_position, bool value) {
    assert(raw_ptr != nullptr);
    assert(bit_position < size);

    BaseType val = read();
    if (value) {
      val |= (1U << bit_position); // Set the bit
    } else {
      val &= ~(1U << bit_position); // Clear the bit
    }
    write(val);
  }

  // Assignment operator to read the value
  operator BaseType() const {
    BaseType val = read();
    printDebug("Register read (assignment operator)", val);
    return val;
  }

  // Assignment operator to write the value
  void operator=(BaseType value) {
    write(value);
    printDebug("Register =", value);
  }

  // Increment and decrement operators
  BaseType &operator++() {
    BaseType val = read();
    ++val;
    write(val);
    printDebug("Register ++", val);
    return *raw_ptr;
  }

  BaseType operator++(int) {
    BaseType old = read();
    BaseType val = old;
    ++val;
    write(val);
    printDebug("Register post-increment", val);
    return old;
  }

  BaseType &operator--() {
    BaseType val = read();
    --val;
    write(val);
    printDebug("Register --", val);
    return *raw_ptr;
  }

  BaseType operator--(int) {
    BaseType old = read();
    BaseType val = old;
    --val;
    write(val);
    printDebug("Register post-decrement", val);
    return old;
  }

  // Addition and subtraction operators
  void operator+=(BaseType value) {
    BaseType val = read();
    val += value;
    write(val);
    printDebug("Register +=", val);
  }

  void operator-=(BaseType value) {
    BaseType val = read();
    val -= value;
    write(val);
    printDebug("Register -=", val);
  }

  // Bitwise NOT operator
  void operator~() {
    BaseType val = read();
    val = ~val;
    write(val);
    printDebug("Register ~", val);
  }

  // Bitwise OR operator
  void operator|=(BaseType bit_mask) {
    BaseType val = read();
    val |= bit_mask;
    write(val);
    printDebug("Register |=", val);
  }

  // Bitwise AND operator
  void operator&=(BaseType bit_mask) {
    BaseType val = read();
    val &= bit_mask;
    write(val);
    printDebug("Register &=", val);
  }

  // Bitwise XOR operator
  void operator^=(BaseType bit_mask) {
    BaseType val = read();
    val ^= bit_mask;
    write(val);
    printDebug("Register ^=", val);
  }

  std::uintptr_t GetAddress() const {
    return reinterpret_cast<std::uintptr_t>(raw_ptr);
  }

  void SetAddress(std::uintptr_t address) {
    raw_ptr = reinterpret_cast<volatile BaseType *>(address);
    if (raw_ptr == nullptr) {
      throw std::invalid_argument("Invalid memory address for register.");
    }
    printDebug("Register SetAddress", read());
  }

protected:
  void write(BaseType bit_mask) {
    *raw_ptr = bit_mask;
    printDebug("Raw write", bit_mask);
  }

  BaseType read() const {
    BaseType val = *raw_ptr;
    printDebug("Raw read", val);
    return val;
  }

private:
  volatile BaseType *raw_ptr;

  // Helper function for debug printing
  void printDebug(const char *message, BaseType value = 0) const {
    if (enableDebugPrints) {
      std::cout << message << " (Address: " << GetAddress()
                << ", Value: " << value << ")" << std::endl;
    }
  }
};

/// @brief Helper struct to get the underlying type of an enum class
template <typename E> struct UnderlyingType {
  typedef typename std::underlying_type<E>::type type;
};

/// @brief Helper struct to enable only enum types
template <typename E> struct EnumTypesOnly {
  typedef typename std::enable_if<std::is_enum<E>::value, E>::type type;
};

/// @brief Converts an enum class to its underlying type (underlying type of the
/// enum class)
template <typename E, typename T = typename EnumTypesOnly<E>::type>
constexpr typename UnderlyingType<E>::type EnumToUnderlyingType(E e) {
  return static_cast<typename UnderlyingType<E>::type>(e);
}

#endif // MEMMAP_REGISTER_H_
