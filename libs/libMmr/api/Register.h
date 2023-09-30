#ifndef MEMMAP_REGISTER_H_
#define MEMMAP_REGISTER_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

/// @brief Enable or disable debug prints.
constexpr bool enableDebugPrints = false;

/// @brief Template struct to define traits for different register sizes.
template <unsigned int size> struct RegisterTraits {};

/// @brief RegisterTraits specialization for 8-bit registers.
template <> struct RegisterTraits<8> {
  typedef std::uint8_t PrimitiveType;
};

/// @brief RegisterTraits specialization for 16-bit registers.
template <> struct RegisterTraits<16> {
  typedef std::uint16_t PrimitiveType;
};

/// @brief RegisterTraits specialization for 32-bit registers.
template <> struct RegisterTraits<32> {
  typedef std::uint32_t PrimitiveType;
};

/// @brief RegisterTraits specialization for 64-bit registers.
template <> struct RegisterTraits<64> {
  typedef std::uint64_t PrimitiveType;
};

/// @brief Class to represent a read-only register.
class ReadOnly {};

/// @brief Class to represent a write-only register.
class WriteOnly {};

/// @brief Class to represent a read-write register.
class ReadWrite : public ReadOnly, public WriteOnly {};

/// @brief Template class to model a memory-mapped register.
/// @tparam size The size of the register in bits.
/// @tparam RegisterAccess The access type for the register (ReadOnly,
/// WriteOnly, or ReadWrite).
template <std::size_t size, typename RegisterAccess = ReadWrite>
class Register {
  typedef typename RegisterTraits<size>::PrimitiveType BaseType;

public:
  /// @brief Constructor for a register without an address.
  explicit Register() : raw_ptr{nullptr} {}

  /// @brief Constructor for a register with a specified memory address.
  /// @param address The memory address of the register.
  explicit Register(std::uintptr_t address)
      : raw_ptr{reinterpret_cast<volatile BaseType *>(address)} {}

  // Bit Proxy class for bit manipulation
  class BitProxy {
  public:
    /// @brief Constructor for a BitProxy.
    /// @param reg The parent register.
    /// @param bit_position The bit position to access.
    BitProxy(Register &reg, std::size_t bit_position)
        : register_(reg), bit_position_(bit_position) {}

    /// @brief Read the bit.
    operator bool() const { return register_.getBit(bit_position_); }

    /// @brief Write the bit.
    /// @param value The value to set the bit to.
    BitProxy &operator=(bool value) {
      register_.setBit(bit_position_, value);
      return *this;
    }

  private:
    Register &register_;
    std::size_t bit_position_;
  };

  /// @brief Bit access operator [] - provides a proxy for bit manipulation.
  /// @param bit_position The bit position to access.
  BitProxy operator[](std::size_t bit_position) {
    return BitProxy(*this, bit_position);
  }

  // Bit access operator [] - read
  /// @brief Read a bit at a specific bit position.
  /// @param bit_position The bit position to read.
  bool getBit(std::size_t bit_position) const {
    assert(bit_position < size);

    BaseType val = read();
    return ((val >> bit_position) & 0x01) == 0x01;
  }

  // Bit access operator [] - write
  /// @brief Write a bit at a specific bit position.
  /// @param bit_position The bit position to write.
  /// @param value The value to set the bit to.
  void setBit(std::size_t bit_position, bool value) {
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
  /// @brief Conversion operator to read the value of the register.
  operator BaseType() const {

    BaseType val = read();
    printDebug("Register read (assignment operator)", val);
    return val;
  }

  // Assignment operator to write the value
  /// @brief Assignment operator to write a value to the register.
  /// @param value The value to write.
  void operator=(BaseType value) {

    write(value);
    printDebug("Register =", value);
  }

  // Increment and decrement operators
  /// @brief Prefix increment operator.
  BaseType &operator++() {

    BaseType val = read();
    ++val;
    write(val);
    printDebug("Register ++", val);
    return *raw_ptr;
  }

  /// @brief Postfix increment operator.
  BaseType operator++(int) {

    BaseType old = read();
    BaseType val = old;
    ++val;
    write(val);
    printDebug("Register post-increment", val);
    return old;
  }

  /// @brief Prefix decrement operator.
  BaseType &operator--() {

    BaseType val = read();
    --val;
    write(val);
    printDebug("Register --", val);
    return *raw_ptr;
  }

  /// @brief Postfix decrement operator.
  BaseType operator--(int) {

    BaseType old = read();
    BaseType val = old;
    --val;
    write(val);
    printDebug("Register post-decrement", val);
    return old;
  }

  // Addition and subtraction operators
  /// @brief Addition assignment operator.
  void operator+=(BaseType value) {

    BaseType val = read();
    val += value;
    write(val);
    printDebug("Register +=", val);
  }

  /// @brief Subtraction assignment operator.
  void operator-=(BaseType value) {

    BaseType val = read();
    val -= value;
    write(val);
    printDebug("Register -=", val);
  }

  // Bitwise NOT operator
  /// @brief Bitwise NOT operator.
  void operator~() {
    BaseType val = read();
    val = ~val;
    write(val);
    printDebug("Register ~", val);
  }

  // Bitwise OR operator
  /// @brief Bitwise OR assignment operator.
  void operator|=(BaseType bit_mask) {

    BaseType val = read();
    val |= bit_mask;
    write(val);
    printDebug("Register |=", val);
  }

  // Bitwise AND operator
  /// @brief Bitwise AND assignment operator.
  void operator&=(BaseType bit_mask) {

    BaseType val = read();
    val &= bit_mask;
    write(val);
    printDebug("Register &=", val);
  }

  // Bitwise XOR operator
  /// @brief Bitwise XOR assignment operator.
  void operator^=(BaseType bit_mask) {
    BaseType val = read();
    val ^= bit_mask;
    write(val);
    printDebug("Register ^=", val);
  }

  /// @brief Get the memory address of the register.
  /// @return The memory address of the register.
  std::uintptr_t GetAddress() const {
    return reinterpret_cast<std::uintptr_t>(raw_ptr);
  }

  /// @brief Set the memory address of the register.
  /// @param address The memory address to set.
  void SetAddress(std::uintptr_t address) {
    raw_ptr = reinterpret_cast<volatile BaseType *>(address);
  }

protected:
  /// @brief Write a value to the register.
  /// @param bit_mask The value to write.
  void write(BaseType bit_mask) {
    assert(raw_ptr != nullptr);
    *raw_ptr = bit_mask;
    printDebug("Raw write", bit_mask);
  }

  /// @brief Read the value from the register.
  /// @return The value read from the register.
  BaseType read() const {
    assert(raw_ptr != nullptr);
    BaseType val = *raw_ptr;
    printDebug("Raw read", val);
    return val;
  }

private:
  volatile BaseType *raw_ptr;

  /// @brief Helper function for debug printing.
  /// @param message The debug message.
  /// @param value The value to print (optional).
  void printDebug(const char *message, BaseType value = 0) const {
    if (enableDebugPrints) {
      std::cout << message << " (Address: " << GetAddress()
                << ", Value: " << value << ")" << std::endl;
    }
  }
};

/// @brief Helper struct to get the underlying type of an enum class.
template <typename E> struct UnderlyingType {
  typedef typename std::underlying_type<E>::type type;
};

/// @brief Helper struct to enable only enum types.
template <typename E> struct EnumTypesOnly {
  typedef typename std::enable_if<std::is_enum<E>::value, E>::type type;
};

/// @brief Converts an enum class to its underlying type (underlying type of the
/// enum class).
/// @tparam E The enum class type.
/// @tparam T The underlying type (automatically determined).
/// @param e The enum value to convert.
/// @return The underlying type value.
template <typename E, typename T = typename EnumTypesOnly<E>::type>
constexpr typename UnderlyingType<E>::type EnumToUnderlyingType(E e) {
  return static_cast<typename UnderlyingType<E>::type>(e);
}

#endif // MEMMAP_REGISTER_H_
