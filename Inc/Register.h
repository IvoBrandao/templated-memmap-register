#ifndef MEMMAP_REGISTER_H_
#define MEMMAP_REGISTER_H_

#include <cstdint>
#include <cstddef>
#include <type_traits>

//!< \brief template for type register trait
template<unsigned int size> struct RegisterTraits {
};

//!< \brief template declaration of valid trait for register size of 8 bits
template<> struct RegisterTraits<8> {
	using PrimitiveType = std::uint8_t;
};

//!< \brief template declaration of valid trait for register size of 16 bits
template<> struct RegisterTraits<16> {
	using PrimitiveType = std::uint16_t;
};

//!< \brief template declaration of valid trait for register size of 32 bits
template<> struct RegisterTraits<32> {
	using PrimitiveType = std::uint32_t;
};

//!< \brief template declaration of valid trait for register size of 64 bits
template<> struct RegisterTraits<64> {
	using PrimitiveType = std::uint64_t;
};

//! \brief Register Access Type Read only 
class ReadOnly {
};
//! \brief Register Access Type Write only access
class WriteOnly {
};
//! \brief Register Access Type ReadWrite
//! \note Inherite from 
class ReadWrite: public ReadOnly, public WriteOnly {
};

//! \brief template to abstract memory mapped registers
template<std::size_t size, typename RegisterAccess = ReadWrite>
class Register 
{
	//! base type contains is the primitivve type selecteed by the user for this register
	using BaseType = typename RegisterTraits<size>::PrimitiveType;

public:

	//! \brief class to access the bits in a register 
	class Bitfield 
	{

	public:
		//! \brief overload the read operator
		operator Register::BaseType() const {
			return read(RegisterAccess { });
		}
		
		//! \brief overload the assignement operator
		void operator=(Register::BaseType val) {
			write(val, RegisterAccess { });
		}

	protected:
	
		//! \brief Construtor
		Bitfield(Register<size, RegisterAccess> *reg, unsigned int num) :
				owner { reg }, bit_num { num } {
		}

		//! \brief 
		Register::BaseType read(ReadOnly) const {
			return (*(owner->raw_ptr) & (1 << bit_num)) ? 1 : 0;
		}

		//! \brief 
		void write(Register::BaseType val, WriteOnly) 
		{
			if (val == 0)
				*(owner->raw_ptr) &= ~(1 << bit_num);
			else
				*(owner->raw_ptr) |= (1 << bit_num);
		}

	private:
		//! to access the register , add the Register class as a friend class 
		friend class Register<size, RegisterAccess> ;

		//! Pointer ot the Register that owns the Bitfield
		Register<size, RegisterAccess> *owner;
		
		//! Number of bit available in this register
		unsigned int bit_num;
	};

public:
	//! \brief default constructor
	//! \param address - memory address of the register
	explicit Register(std::uint32_t address) :
			raw_ptr { reinterpret_cast<std::uint32_t*>(address) } {
	}

	/// ==== Bitwise arithmetic operators ====
	//! \brief overload assignment operator
	//! \param bit_mask bit mask to assign to the register
	inline void operator=(BaseType bit_mask) {
		write(bit_mask, RegisterAccess { });
	}

	//! \brief
	inline operator BaseType() const {
		return read(RegisterAccess { });
	}

	//! \brief
	//! \param bit_mask
	inline void operator|=(std::uint32_t bit_mask) {
		or_assign(bit_mask, RegisterAccess { });
	}

	/// ==== Comparison operators ====
	
	//! \brief
	//! \param bit_mask
	inline void operator&=(std::uint32_t bit_mask) {
		and_assign(bit_mask, RegisterAccess { });
	}

	//! prefix increment
	BaseType& operator++() {
		// actual increment takes place here
		(*this->raw_ptr)++;
		return *this->raw_ptr; // return new value by reference
	}

	//! postfix increment
	inline BaseType operator++(int) {
		BaseType old = *this->raw_ptr; // copy old value
		operator++();  // prefix increment
		return old;    // return old value
	}

	//! prefix decrement
	inline BaseType& operator--() {
		// actual decrement takes place here
		return *this->raw_ptr; // return new value by reference
	}

	//! postfix decrement
	inline BaseType operator--(int) {
		BaseType old = *this->raw_ptr; // copy old value
		operator--();  // prefix decrement
		return old;    // return old value
	}

	//!
	inline Bitfield operator[](unsigned int index) {
		// Check size of index
		return Bitfield { this, index };
	}

	//!
	inline BaseType GetAddress(void) {
		return static_cast<BaseType>(raw_ptr);
	}

	//!
	inline void SetAddress(std::uint32_t address )
	{
		this->raw_ptr = reinterpret_cast<std::uint32_t*>(address);
	}

protected:

	//! \brief
	//! \param bit_mask
	//! \param WriteOnly
	inline void write(BaseType bit_mask, WriteOnly) {
		if (raw_ptr != nullptr) {
			*raw_ptr = bit_mask;
		}
	}

	//! \brief
	//! \param bit_mask
	//! \param WriteOnly
	inline BaseType read(ReadOnly) const {
		return *raw_ptr;
	}

	//! \brief
	//! \param bit_mask
	//! \param ReadWrite
	inline void or_assign(std::uint32_t bit_mask, ReadWrite) {
		if (raw_ptr != nullptr) {

			*raw_ptr |= bit_mask;
		}
	}

	//! \brief
	//! \param bit_mask
	//! \param ReadWrite
	inline void and_assign(std::uint32_t bit_mask, ReadWrite) {
		if (raw_ptr != nullptr) {
			*raw_ptr &= bit_mask;
		}
	}

private:

	//! pointer to the memory mapped register
	volatile BaseType *raw_ptr;

};



template <typename E>
using UnderlyingType = typename std::underlying_type<E>::type;

template <typename E>
using EnumTypesOnly = typename std::enable_if<std::is_enum<E>::value, E>::type;

template <typename E, typename = EnumTypesOnly<E>>
constexpr UnderlyingType<E> EnumToUnderlyingType(E e) 
{
    return static_cast<UnderlyingType<E>>(e);
}

#endif

