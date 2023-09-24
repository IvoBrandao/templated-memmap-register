#ifndef MEMMAP_REGISTER_H_
#define MEMMAP_REGISTER_H_

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>

// Documentation should be added throughout the code.

template <unsigned int S> struct RegisterTraits {};

template <> struct RegisterTraits<8> {
  using PrimitiveType = std::uint8_t;
};

template <> struct RegisterTraits<16> {
  using PrimitiveType = std::uint16_t;
};

template <> struct RegisterTraits<32> {
  using PrimitiveType = std::uint32_t;
};

template <> struct RegisterTraits<64> {
  using PrimitiveType = std::uint64_t;
};

class ReadOnly {};
class WriteOnly {};
class ReadWrite : public ReadOnly, public WriteOnly {};

template <std::size_t S, typename RA = ReadWrite> class Register {
  using BaseType = typename RegisterTraits<S>::PrimitiveType;

public:
  class Bitfield {
  public:
    /// \brief Bitfield constructor
    /// \param reg pointer to the register containing the bitfield
    /// \param num bit number of the bitfield
    Bitfield(Register<S, RA> *reg, unsigned int num)
        : owner{reg}, bit_num{num} {}

    /// \brief Bitfield copy constructor
    /// \return reference to the bitfield
    operator BaseType() const { return read(); }

    /// \brief Bitfield assignment operator
    /// \param val value to assign to the bitfield
    /// \return reference to the bitfield
    void operator=(BaseType val) { write(val); }

  protected:
    /// \brief Bitfield read method
    BaseType read() const {
      return (*(owner->raw_ptr) & (1 << bit_num)) ? 1 : 0;
    }

    /// \brief Bitfield write method
    /// \param val value to write to the bitfield
    void write(BaseType val) {
      if (val == 0)
        *(owner->raw_ptr) &= ~(1 << bit_num);
      else
        *(owner->raw_ptr) |= (1 << bit_num);
    }

  private:
    // Register class is a friend of Bitfield class
    friend class Register<S, RA>;
    // Pointer to the register containing the bitfield
    Register<S, RA> *owner;
    // Bit number of the bitfield
    unsigned int bit_num;
  };

  /// \brief Register constructor
  /// \param address address of the register
  /// \return reference to the register
  explicit Register(std::uint32_t address)
      : raw_ptr{reinterpret_cast<volatile BaseType *>(address)} {

    if (trace_enabled) {
      std::cout << "Register::Register() - address: " << address << std::endl;
    }

    if (raw_ptr == nullptr) {
      throw std::invalid_argument("Invalid memory address");
    }
  }

  /// \brief Register default constructor
  /// \return reference to the register
  explicit Register() : raw_ptr{nullptr} {

    if (trace_enabled) {
      std::cout << "Register::Register() - address: NULL " << std::endl;
    }
  }

  /// \brief Register copy constructor
  /// \param bit_mask value to assign to the register
  void operator=(BaseType bit_mask) { write(bit_mask); }

  /// \brief Register copy constructor
  /// \return value of the register
  operator BaseType() const { return read(); }

  /// \brief OR operator for the register
  /// \param bit_mask value to XOR with the register
  void operator|=(std::uint32_t bit_mask) { or_assign(bit_mask); }

  // \brief AND operator for the register
  /// \param bit_mask value to AND with the register
  void operator&=(std::uint32_t bit_mask) { and_assign(bit_mask); }

  /// \brief XOR operator for the register
  /// \param bit_mask value to XOR with the register
  void operator^=(std::uint32_t bit_mask) { xor_assign(bit_mask); }

  /// \brief increment operator for the register
  /// \return reference to the register
  BaseType &operator++() {
    ++(*raw_ptr);
    return *raw_ptr;
  }

  /// \brief increment operator for the register
  /// \return value of the register
  BaseType operator++(int) {
    BaseType old = *raw_ptr;
    operator++();
    return old;
  }

  /// \brief decrement operator for the register
  /// \return reference to the register
  BaseType &operator--() {
    --(*raw_ptr);
    return *raw_ptr;
  }

  // \brief decrement operator for the register
  /// \return value of the register
  BaseType operator--(int) {
    BaseType old = *raw_ptr;
    operator--();
    return old;
  }

  /// \brief Bitfield operator for the register
  /// \param index bit number of the bitfield
  /// \return reference to the bitfield
  Bitfield operator[](unsigned int index) { return Bitfield{this, index}; }

  /// \brief returns the address of the register
  /// \return address of the register
  BaseType GetAddress() const {
    return static_cast<BaseType>(reinterpret_cast<std::uintptr_t>(raw_ptr));
  }

  /// \brief sets the address of the register
  /// \param address address of the register
  void SetAddress(std::uint32_t address) {
    raw_ptr = reinterpret_cast<volatile BaseType *>(address);
  }

protected:
  /// \brief Register write method
  /// \param bit_mask value to write to the register
  void write(BaseType bit_mask) {

    if (trace_enabled) {
      std::cout << "Register::write() - address: " << GetAddress()
                << " value: " << bit_mask << std::endl;
    }

    *raw_ptr = bit_mask;
  }

  /// \brief Register read method
  /// \return value of the register
  BaseType read() const {

    if (trace_enabled) {
      std::cout << "Register::read() - address: " << GetAddress()
                << " value: " << *raw_ptr << std::endl;
    }
    return *raw_ptr;
  }

  /// \brief Register OR method
  /// \param bit_mask value to OR with the register
  void or_assign(std::uint32_t bit_mask) {

    if (trace_enabled) {
      std::cout << "Register::or_assign() - address: " << GetAddress()
                << " value: " << bit_mask << std::endl;
    }
    *raw_ptr |= bit_mask;
  }

  /// \brief Register AND method
  /// \param bit_mask value to AND with the register
  void and_assign(std::uint32_t bit_mask) {

    if (trace_enabled) {
      std::cout << "Register::and_assign() - address: " << GetAddress()
                << " value: " << bit_mask << std::endl;
    }
    *raw_ptr &= bit_mask;
  }

  /// \brief Register XOR method
  /// \param bit_mask value to XOR with the register
  void xor_assign(std::uint32_t bit_mask) {

    if (trace_enabled) {
      std::cout << "Register::xor_assign() - address: " << GetAddress()
                << " value: " << bit_mask << std::endl;
    }

    *raw_ptr ^= bit_mask;
  }

private:
  bool trace_enabled = true;
  // Pointer to the register
  volatile BaseType *raw_ptr;
};

/// \brief Enum to underlying type conversion
/// \param e enum value
/// \return underlying type of the enum value
template <typename E>
using UnderlyingType = typename std::underlying_type<E>::type;

/// \brief Enum to underlying type conversion
/// \param e enum value
/// \return underlying type of the enum value
template <typename E,
          typename = typename std::enable_if<std::is_enum<E>::value>::type>
constexpr UnderlyingType<E> EnumToUnderlyingType(E e) {
  return static_cast<UnderlyingType<E>>(e);
}
#endif
