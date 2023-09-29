#ifndef MEMMAP_REGISTER_H_
#define MEMMAP_REGISTER_H_

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>

// Enable or disable debug prints
constexpr bool enableDebugPrints = false;

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
  class Bitfield {
  public:
    // Pass a reference to the Register instance to the Bitfield constructor
    Bitfield(Register<size, RegisterAccess> &reg, unsigned int num)
        : owner{&reg}, bit_num{num} {}

    operator BaseType() const {
      owner->printDebug("Bitfield read");
      return read();
    }

    void operator=(BaseType val) {
      owner->printDebug("Bitfield write", val);
      write(val);
    }

  protected:
    BaseType read() const {
      return (*(owner->raw_ptr) & (1 << bit_num)) ? 1 : 0;
    }

    void write(BaseType val) {
      if (val == 0)
        *(owner->raw_ptr) &= ~(1 << bit_num);
      else
        *(owner->raw_ptr) |= (1 << bit_num);
    }

  private:
    friend class Register<size, RegisterAccess>;
    Register<size, RegisterAccess> *owner;
    unsigned int bit_num;
  };

public:
  explicit Register(std::uintptr_t address)
      : raw_ptr{reinterpret_cast<volatile BaseType *>(address)} {
    if (raw_ptr == nullptr) {
      throw std::invalid_argument("Invalid memory address for register.");
    }
    printDebug("Register constructor", read());
  }

  void operator=(BaseType bit_mask) {
    printDebug("Register write", bit_mask);
    write(bit_mask);
  }

  operator BaseType() const {
    BaseType val = read();
    printDebug("Register read", val);
    return val;
  }

  void operator|=(std::uint32_t bit_mask) {
    BaseType val = read();
    val |= bit_mask;
    printDebug("Register OR=", val);
    or_assign(bit_mask);
  }

  void operator&=(std::uint32_t bit_mask) {
    BaseType val = read();
    val &= bit_mask;
    printDebug("Register AND=", val);
    and_assign(bit_mask);
  }

  BaseType &operator++() {
    BaseType val = read();
    ++val;
    printDebug("Register ++", val);
    ++(*raw_ptr);
    return *raw_ptr;
  }

  BaseType operator++(int) {
    BaseType old = read();
    BaseType val = old;
    ++val;
    printDebug("Register post-increment", val);
    operator++();
    return old;
  }

  BaseType &operator--() {
    BaseType val = read();
    --val;
    printDebug("Register --", val);
    --(*raw_ptr);
    return *raw_ptr;
  }

  BaseType operator--(int) {
    BaseType old = read();
    BaseType val = old;
    --val;
    printDebug("Register post-decrement", val);
    operator--();
    return old;
  }

  Bitfield operator[](unsigned int index) {
    if (index >= size) {
      throw std::out_of_range("Bitfield index out of bounds.");
    }
    printDebug("Bitfield access");
    return Bitfield{*this, index};
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

  void or_assign(std::uint32_t bit_mask) {
    *raw_ptr |= bit_mask;
    printDebug("Raw OR=", *raw_ptr);
  }

  void and_assign(std::uint32_t bit_mask) {
    *raw_ptr &= bit_mask;
    printDebug("Raw AND=", *raw_ptr);
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

template <typename E> struct UnderlyingType {
  typedef typename std::underlying_type<E>::type type;
};

template <typename E> struct EnumTypesOnly {
  typedef typename std::enable_if<std::is_enum<E>::value, E>::type type;
};

template <typename E, typename T = typename EnumTypesOnly<E>::type>
constexpr typename UnderlyingType<E>::type EnumToUnderlyingType(E e) {
  return static_cast<typename UnderlyingType<E>::type>(e);
}

#endif // MEMMAP_REGISTER_H_
