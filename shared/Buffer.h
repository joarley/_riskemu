#ifndef _RISKEMULIBRARY_BUFFER_H_
#define _RISKEMULIBRARY_BUFFER_H_

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "stdtypes.h"
#include "Packable.h"

#include <vector>
#include <string>
#include <cstddef>
#include <cstring>

using namespace std;

typedef boost::shared_ptr<Buffer> Buffer_ptr;

class Buffer : public boost::enable_shared_from_this<Buffer> {
public:
	template<class T> struct SizedValue
	{
		SizedValue(T value, size_t size): value(value), size(size){}
		T value;
		size_t size;
	};

	template<class T> struct FromPositionedValue
	{
		FromPositionedValue(T value, size_t position): value(value), position(position){}
		T value;
		size_t position;
	};

	template<class T> struct ToPositionedValue
	{
		ToPositionedValue(T value, size_t position): value(value), position(position){}
		T value;
		size_t position;
	};

	template<class T> static SizedValue<void*> Bytes(T* value, size_t size)
	{
		return SizedValue<void*>((void*)value, size);
	}

	static SizedValue<string&> StringSizeFixed(string& value, size_t size)
	{
		return SizedValue<string&>(value, size);
	}

	static SizedValue<char*&> StringSizeFixed(char*& value, size_t size)
	{
		return SizedValue<char*&>(value, size);
	}

	static SizedValue<const char*> StringSizeFixed(const char* value, size_t size)
	{
		return SizedValue<const char*>(value, size);
	}

	template<class T> static FromPositionedValue<T&> FromPosition(T& value, size_t position)
	{
		return FromPositionedValue<T&>(value, position);
	}

	template<class T> static ToPositionedValue<T> ToPosition(T value, size_t position)
	{
		return ToPositionedValue<T>(value, position);
	}
public:
    Buffer(size_t maxLength = 4096) : m_buffer(maxLength) {
        m_maxLength = maxLength;
        m_length = 0;
        m_readerOffset = 0;
        m_writerOffset = 0;
    }

    Buffer(byte* buffer, size_t length) : m_buffer(length) {
        m_buffer.assign(buffer, buffer + length);
        m_maxLength = length;
        m_length = length;
        m_readerOffset = 0;
        m_writerOffset = length;
    }

    Buffer(const Buffer& b) {
        m_buffer = b.m_buffer;
        m_length = b.m_length;
        m_readerOffset = b.m_readerOffset;
        m_writerOffset = b.m_writerOffset;
    }

	inline byte* Data() {
        return &m_buffer[0];
    }

    inline size_t Length() {
        return m_length;
    }

    inline size_t MaxLength() {
        return m_maxLength;
    }

	inline void Clear()
	{
		memset(Data(), 0, MaxLength()); 
	}

    inline void SetLength(size_t length) {
        m_length = length < m_maxLength ? length : m_maxLength;
    }

    inline void SetMaxLength(size_t length) {
        m_buffer.resize(length);
        m_maxLength = length;
    }

    inline void SetReaderOffset(size_t offset) {
        m_readerOffset = offset > m_maxLength ? m_maxLength : offset;
    }

    inline void SetWriteOffset(size_t offset) {
        m_writerOffset = offset > m_maxLength ? m_maxLength : offset;
    }
    
    inline size_t GetReaderOffset() {
        return m_readerOffset;
    }

    inline size_t GetWriteOffset() {
        return m_writerOffset;
    }

	template<class T> inline typename boost::enable_if<boost::is_fundamental<T>, Buffer&>::type operator<<(const T value)
	{
		size_t size = sizeof(T);
		if(AddNumber(value, m_writerOffset))
			m_writerOffset += size;
		return *this;
	}

	inline Buffer& operator<<(Packable& value)
	{
		AddPack(value);
		return *this;
	}

	inline Buffer& operator<<(string& value)
	{
		return *this << value.c_str();		
	}

	inline Buffer& operator<<(const char* value)
	{
		size_t size = strlen(value) + 1;
		if(AddString(value, size, m_writerOffset))
			m_writerOffset += size;
		return *this;
	}

	inline Buffer& operator<<(SizedValue<const char*>& value)
	{
		if(AddString(value.value, value.size, m_writerOffset))
			m_writerOffset += value.size;
		return *this;
	}

	inline Buffer& operator<<(SizedValue<string&>& value)
	{
		return *this << StringSizeFixed(value.value.c_str(), value.size);
	}

	inline Buffer& operator<<(SizedValue<void*>& value)
	{
		if(AddBytes(value.value, value.size, m_writerOffset))
			m_writerOffset += value.size;
		return *this;
	}

	template<class T> inline Buffer& operator<<(ToPositionedValue<T>& value)
	{
		size_t pos = GetWriteOffset();
		SetWriteOffset(value.position);
		*this << value.value;
		SetWriteOffset(pos);
		return *this;
	}

	template<class T> inline typename boost::enable_if_c<boost::is_fundamental<T>::value && !boost::is_const<T>::value, Buffer&>::type operator>>(T& value)
	{
		size_t size = sizeof(T);
		if(GetNumber(value, m_readerOffset))
			m_readerOffset += size;
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
		size_t size = strlen((char*)&m_buffer[m_readerOffset]) + 1;
		if(GetString(value, size, m_readerOffset))
			m_readerOffset += size;			
		return *this;
	}



	inline Buffer& operator>>(SizedValue<char*&>& value)
	{
		if(GetString(value.value, value.size, m_readerOffset))
			m_readerOffset += value.size;
		return *this;
	}

	inline Buffer& operator>>(SizedValue<string&>& value)
	{
		char* str;
		if(GetString(str, value.size, m_readerOffset))
			m_readerOffset += value.size;
		value.value = str;
		delete str;
		return *this;
	}

	inline Buffer& operator>>(SizedValue<void*&>& value)
	{
		if(GetBytes(value.value, value.size, m_readerOffset))
			m_readerOffset += value.size;
		return *this;
	}

	template<class T> inline Buffer& operator>>(FromPositionedValue<T>& value)
	{
		size_t pos = GetReaderOffset();
		SetReaderOffset(value.position);
		*this >> value.value;
		SetReaderOffset(pos);
		return *this;
	}	
private:
	template<class T> inline bool AddNumber(T value, size_t offset) {
        size_t size = sizeof(T);
        if (offset + size > m_maxLength) {
            return false;
        }
        *(T*)&m_buffer[offset] = value;
        m_length = offset + size > m_length ? offset + size : m_length;
        return true;
    }

	inline bool AddPack(Packable& value) {
		value.Pack(shared_from_this());        
        return true;
	}

    inline bool AddPack(Packable& value, size_t offset) {
		size_t writeof = m_writerOffset;
        SetWriteOffset(offset);
        value.Pack(shared_from_this());
        SetWriteOffset(writeof);
        return true;
    }

	inline bool AddString(const char* value, size_t size, size_t offset) {
        if (offset + size > m_maxLength) {
            return false;
        }

        size_t stringSize = strlen(value);
        size_t paddingSize = size > stringSize ? size - stringSize : 0;

        memcpy(&m_buffer[offset], value, size - paddingSize);
        memset(&m_buffer[offset + size - paddingSize], 0, paddingSize);

        m_length = offset + size > m_length ? offset + size : m_length;
        return true;
	}

    inline bool AddBytes(void* value, size_t size, size_t offset) {
        if (offset + size > m_maxLength) {
            return false;
        }
        memcpy(&m_buffer[offset], value, size);
        m_length = offset + size > m_length ? offset + size : m_length;
        return true;
    }

	template<class T> inline bool GetNumber(T &value, size_t offset) {
        size_t size = sizeof(T);
        if (offset + size > m_length) return false;
        value = *(T*)&m_buffer[offset];
        return true;
    }

	inline bool GetPack(Packable& value) {
		value.Unpack(shared_from_this());
        return true;
    }

	inline bool GetPack(Packable& value, size_t offset) {
		size_t readof = m_readerOffset;
        SetReaderOffset(offset);
        value.Unpack(shared_from_this());
        SetReaderOffset(readof);
        return true;
    }

	inline bool GetString(char *&value, size_t size, size_t offset) {
        if (offset + size > m_length) {
            return false;
        }

		if(m_buffer[offset + size - 1] != '\0')
			value = new char[size + 1];		
		else
			value = new char[size];

		memcpy(value, &m_buffer[offset], size);

		if(m_buffer[offset + size - 1] != '\0')
			value[size] = '\0';
		
        return true;
	}

	inline bool GetBytes(void*& value, size_t size, size_t offset) {
        if (offset + size > m_length) {
            return false;
        }
        memcpy(value, &m_buffer[offset], size);
        return true;
    }
protected:
    vector<byte> m_buffer;
    size_t m_maxLength;
    size_t m_length;
    size_t m_readerOffset;
    size_t m_writerOffset;
};

#endif //_RISKEMULIBRARY_BUFFER_H_