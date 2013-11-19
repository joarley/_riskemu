#ifndef _RISKEMULIBRARY_BUFFER_H_
#define _RISKEMULIBRARY_BUFFER_H_

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "stdtypes.h"
#include "Packable.h"
#include "Utils.h"

#include <vector>
#include <string>
#include <cstddef>
#include <cstring>

using namespace std;

class Buffer;
typedef boost::shared_ptr<Buffer> Buffer_ptr;

class Buffer : public boost::enable_shared_from_this<Buffer> {
public:
	template<class T> struct SizedValue
	{
		SizedValue(size_t size, T value): value(value), size(size){}
		T value;
		size_t size;
	};

	template<class T> struct FromPositionedValue
	{
		FromPositionedValue(size_t position, T value): value(value), position(position){}
		T value;
		size_t position;
	};

	template<class T> struct ToPositionedValue
	{
		ToPositionedValue(size_t position, T value): value(value), position(position){}
		T value;
		size_t position;
	};

    template<class T> static const SizedValue<void*> Bytes(size_t size, T *value)
	{
        return SizedValue<void*>(size, value);
	}

    static const SizedValue<string&> StringSizeFixed(size_t size, string &value)
	{
		return SizedValue<string&>(size, value);
	}

    static const SizedValue<char*&> StringSizeFixed(size_t size, char *&value)
	{
		return SizedValue<char*&>(size, value);
	}

    static const SizedValue<const char*> StringSizeFixed(size_t size, const char *value)
	{
		return SizedValue<const char*>(size, value);
	}

    template<class T> static const FromPositionedValue<T&> FromPosition(size_t position, T &value)
	{
		return FromPositionedValue<T&>(position, value);
	}

	template<class T> static const ToPositionedValue<T> ToPosition(size_t position, T value)
	{
		return ToPositionedValue<T>(position, value);
	}
public:
    Buffer(size_t maxLength = 4096) : buffer(maxLength) {
        this->length = 0;
        this->readerOffset = 0;
        this->writerOffset = 0;
    }

    Buffer(byte* buffer, size_t length) : buffer(length) {
        this->buffer.assign(buffer, buffer + length);
        this->length = length;
        this->readerOffset = 0;
        this->writerOffset = length;
    }

    Buffer(const Buffer& b) {
        this->buffer = b.buffer;
        this->length = b.length;
        this->readerOffset = b.readerOffset;
        this->writerOffset = b.writerOffset;
    }

	inline byte* Data() {
        return &this->buffer[0];
    }

    inline size_t Length() {
        return this->length;
    }

    inline size_t MaxLength() {
        return buffer.size();
    }

	inline void Clear()
	{
		memset(Data(), 0, MaxLength()); 
	}

    inline void SetLength(size_t length) {
		this->length = length < MaxLength() ? length : MaxLength();
    }

    inline void SetMaxLength(size_t length) {
        this->buffer.resize(length);        
    }

    inline void SetReaderOffset(size_t offset) {
		this->readerOffset = offset > MaxLength() ? MaxLength() : offset;
    }

    inline void SetWriteOffset(size_t offset) {
		this->writerOffset = offset > MaxLength() ? MaxLength() : offset;
    }
    
    inline size_t GetReaderOffset() {
        return this->readerOffset;
    }

    inline size_t GetWriteOffset() {
        return this->writerOffset;
    }

    inline Buffer& operator<<(const uint8 value)
	{
		size_t size = sizeof(uint8);
		if(AddNumber(value, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}
	
    inline Buffer& operator<<(const int8 value)
	{
		return *this << (uint8)value;
	}
	
    inline Buffer& operator<<(const uint16 value)
	{
		size_t size = sizeof(uint16);
		if(AddNumber(value, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}
	
    inline Buffer& operator<<(const int16 value)
	{
		return *this << (uint16)value;
	}
	
    inline Buffer& operator<<(const uint32 value)
	{
		size_t size = sizeof(uint32);
		if(AddNumber(value, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}
	
    inline Buffer& operator<<(const int32 value)
	{
		return *this << (uint32)value;
	}
	
    inline Buffer& operator<<(const uint64 value)
	{
		size_t size = sizeof(uint64);
		if(AddNumber(value, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}
	
    inline Buffer& operator<<(const int64 value)
	{
		return *this << (uint64)value;
	}
	
    inline Buffer& operator<<(const f64 value)
	{
		size_t size = sizeof(f64);
		if(AddNumber(value, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}
	
    inline Buffer& operator<<(const f32 value)
	{
		size_t size = sizeof(f32);
		if(AddNumber(value, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}

    inline Buffer& operator<<(const Packable &value)
	{
		AddPack(value);
		return *this;
	}

    inline Buffer& operator<<(const string &value)
	{
		return *this << value.c_str();		
	}

	inline Buffer& operator<<(const char* value)
	{
		size_t size = strlen(value) + 1;
		if(AddString(value, size, this->writerOffset))
			this->writerOffset += size;
		return *this;
	}

    inline Buffer& operator<<(const SizedValue<const char*> value)
	{
		if(AddString(value.value, value.size, this->writerOffset))
			this->writerOffset += value.size;
		return *this;
	}

    inline Buffer& operator<<(const SizedValue<const string&> &value)
	{
        return *this << StringSizeFixed(value.size, value.value.c_str());
	}

    inline Buffer& operator<<(const SizedValue<void*> value)
	{
		if(AddBytes(value.value, value.size, this->writerOffset))
			this->writerOffset += value.size;
		return *this;
	}

    template<class T> inline Buffer& operator<<(const ToPositionedValue<T> value)
	{
		size_t pos = GetWriteOffset();
		SetWriteOffset(value.position);
		*this << value.value;
		SetWriteOffset(pos);
		return *this;
	}

		inline Buffer& operator>>(uint8& value)
	{
		size_t size = sizeof(uint8);
		if(GetNumber(value, this->readerOffset))
			this->readerOffset += size;
		return *this;
	}
	
	inline Buffer& operator>>(int8& value)
	{
		return *this >> (uint8&)value;
	}
	
	inline Buffer& operator>>(uint16& value)
	{
		size_t size = sizeof(uint16);
		if(GetNumber(value, this->readerOffset))
			this->readerOffset += size;
		return *this;
	}
	
	inline Buffer& operator>>(int16& value)
	{
		return *this >> (uint16&)value;
	}
	
	inline Buffer& operator>>(uint32& value)
	{
		size_t size = sizeof(uint32);
		if(GetNumber(value, this->readerOffset))
			this->readerOffset += size;
		return *this;
	}
	
	inline Buffer& operator>>(int32& value)
	{
		return *this >> (uint32&)value;
	}
	
	inline Buffer& operator>>(uint64& value)
	{
		size_t size = sizeof(uint64);
		if(GetNumber(value, this->readerOffset))
			this->readerOffset += size;
		return *this;
	}
	
	inline Buffer& operator>>(int64& value)
	{
		return *this >> (uint64&)value;
	}
	
	inline Buffer& operator>>(f32& value)
	{
		size_t size = sizeof(f32);
		if(GetNumber(value, this->readerOffset))
			this->readerOffset += size;
		return *this;
	}
	
	inline Buffer& operator>>(f64& value)
	{
		size_t size = sizeof(f64);
		if(GetNumber(value, this->readerOffset))
			this->readerOffset += size;
		return *this;
	}

	inline Buffer& operator>>(Packable& value)
	{
		GetPack(value);
		return *this;
	}

	inline Buffer& operator>>(string &value)
	{
		char *str;
		*this >> str;
		value = str;
		delete str;
		return *this;
	}

	inline Buffer& operator>>(char *&value)
	{
		size_t size = strlen_secure((char*)&this->buffer[this->readerOffset], this->length - this->readerOffset) + 1;
		if(GetString(value, size, this->readerOffset))
			this->readerOffset += size;			
		return *this;
	}



	inline Buffer& operator>>(const SizedValue<char*&>& value)
	{
		if(GetString(value.value, value.size, this->readerOffset))
			this->readerOffset += value.size;
		return *this;
	}

	inline Buffer& operator>>(const SizedValue<string&>& value)
	{
		char* str;
		if(GetString(str, value.size, this->readerOffset))
			this->readerOffset += value.size;
		value.value = str;
		delete str;
		return *this;
	}

	inline Buffer& operator>>(const SizedValue<void*>& value)
	{
		if(GetBytes(value.value, value.size, this->readerOffset))
			this->readerOffset += value.size;
		return *this;
	}

	template<class T> inline Buffer& operator>>(const FromPositionedValue<T>& value)
	{
		size_t pos = GetReaderOffset();
		SetReaderOffset(value.position);
		*this >> value.value;
		SetReaderOffset(pos);
		return *this;
	}	
private:
	inline bool AddNumber(const uint8 value, size_t offset) {
        size_t size = sizeof(uint8);
		if (offset + size > MaxLength()) return false;
        *(uint8*)&this->buffer[offset] = value;
        this->length = offset + size > this->length ? offset + size : this->length;
		return true;
	}
	
	inline bool AddNumber(const uint16 value, size_t offset) {
        size_t size = sizeof(uint16);
		if (offset + size > MaxLength()) return false;
        *(uint16*)&this->buffer[offset] = value;
        this->length = offset + size > this->length ? offset + size : this->length;
		return true;
	}
	
	inline bool AddNumber(const uint32 value, size_t offset) {
        size_t size = sizeof(uint32);
		if (offset + size > MaxLength()) return false;
        *(uint32*)&this->buffer[offset] = value;
        this->length = offset + size > this->length ? offset + size : this->length;
		return true;
	}
	
	inline bool AddNumber(const uint64 value, size_t offset) {
        size_t size = sizeof(uint64);
		if (offset + size > MaxLength()) return false;
        *(uint64*)&this->buffer[offset] = value;
        this->length = offset + size > this->length ? offset + size : this->length;
		return true;
	}
	
	inline bool AddNumber(const f64 value, size_t offset) {
        size_t size = sizeof(f64);
		if (offset + size > MaxLength()) return false;
        *(f64*)&this->buffer[offset] = value;
        this->length = offset + size > this->length ? offset + size : this->length;
		return true;
	}
	
	inline bool AddNumber(const f32 value, size_t offset) {
        size_t size = sizeof(f32);
		if (offset + size > MaxLength()) return false;
        *(f32*)&this->buffer[offset] = value;
        this->length = offset + size > this->length ? offset + size : this->length;
		return true;
	}

	inline bool AddPack(const Packable& value) {
		value.Pack(shared_from_this());        
        return true;
	}

    inline bool AddPack(Packable& value, size_t offset) {
		size_t writeof = this->writerOffset;
        SetWriteOffset(offset);
        value.Pack(shared_from_this());
        SetWriteOffset(writeof);
        return true;
    }

	inline bool AddString(const char* value, size_t size, size_t offset) {
		if (offset + size > MaxLength()) {
            return false;
        }

        size_t stringSize = strlen(value);
        size_t paddingSize = size > stringSize ? size - stringSize : 0;

        memcpy(&this->buffer[offset], value, size - paddingSize);
        memset(&this->buffer[offset + size - paddingSize], 0, paddingSize);

        this->length = offset + size > this->length ? offset + size : this->length;
        return true;
	}

    inline bool AddBytes(const void* value, size_t size, size_t offset) {
		if (offset + size > MaxLength()) {
            return false;
        }
        memcpy(&this->buffer[offset], value, size);
        this->length = offset + size > this->length ? offset + size : this->length;
        return true;
    }

	inline bool GetNumber(uint8 &value, size_t offset) {
        size_t size = sizeof(uint8);
        if (offset + size > this->length) return false;
        value = *(uint8*)&this->buffer[offset];
        return true;
    }
	
	inline bool GetNumber(uint16 &value, size_t offset) {
        size_t size = sizeof(uint16);
        if (offset + size > this->length) return false;
        value = *(uint16*)&this->buffer[offset];
        return true;
    }
	
	inline bool GetNumber(uint64 &value, size_t offset) {
        size_t size = sizeof(uint64);
        if (offset + size > this->length) return false;
        value = *(uint64*)&this->buffer[offset];
        return true;
    }
	
	inline bool GetNumber(uint32 &value, size_t offset) {
        size_t size = sizeof(uint32);
        if (offset + size > this->length) return false;
        value = *(uint32*)&this->buffer[offset];
        return true;
    }
	
	inline bool GetNumber(f64 &value, size_t offset) {
        size_t size = sizeof(f64);
        if (offset + size > this->length) return false;
        value = *(f64*)&this->buffer[offset];
        return true;
    }
	
	inline bool GetNumber(f32 &value, size_t offset) {
        size_t size = sizeof(f32);
        if (offset + size > this->length) return false;
        value = *(f32*)&this->buffer[offset];
        return true;
    }

	inline bool GetPack(Packable &value) {
		value.Unpack(shared_from_this());
        return true;
    }

	inline bool GetPack(Packable &value, size_t offset) {
		size_t readof = this->readerOffset;
        SetReaderOffset(offset);
        value.Unpack(shared_from_this());
        SetReaderOffset(readof);
        return true;
    }

	inline bool GetString(char *&value, size_t size, size_t offset) {
        if (offset + size > this->length) {
            return false;
        }

		if(this->buffer[offset + size - 1] != '\0')
			value = new char[size + 1];		
		else
			value = new char[size];

		memcpy(value, &this->buffer[offset], size);

		if(this->buffer[offset + size - 1] != '\0')
			value[size] = '\0';
		
        return true;
	}

	inline bool GetBytes(void *value, size_t size, size_t offset) {
        if (offset + size > this->length) {
            return false;
        }
        memcpy(value, &this->buffer[offset], size);
        return true;		
    }
protected:
    vector<byte> buffer;
    size_t length;
    size_t readerOffset;
    size_t writerOffset;
};

#endif //_RISKEMULIBRARY_BUFFER_H_
