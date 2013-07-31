#ifndef _BUFFER_H_
#define _BUFFER_H_

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

	template<class T> inline typename boost::enable_if<boost::is_fundamental<T>, Buffer&>::type operator<<(T value)
	{
		size_t size = sizeof(T);
		if(Add(value, m_writerOffset))
			m_writerOffset += size;
		return *this;
	}

	inline Buffer& operator<<(Packable& value)
	{
		value.Pack(shared_from_this());
		return *this;
	}

	inline Buffer& operator<<(string& value)
	{
		return *this << value.c_str();		
	}

	inline Buffer& operator<<(const char* value)
	{
		size_t size = strlen(value) + 1;
		if(AddStringSizeFixed(value, size, m_writerOffset))
			m_writerOffset += size;
		return *this;
	}

	template<class T> inline typename boost::enable_if<boost::is_fundamental<T>, bool>::type Add(T value, size_t offset) {
        size_t size = sizeof(T);
        if (offset + size > m_maxLength) {
            return false;
        }
        *(T*)&m_buffer[offset] = value;
        m_length = offset + size > m_length ? offset + size : m_length;
        return true;
    }

    inline bool AddPack(Packable& value, size_t offset) {
		size_t writeof = m_writerOffset;
        SetWriteOffset(offset);
        value.Pack(shared_from_this());
        SetWriteOffset(writeof);
        return true;
    }

    inline bool AddStringSizeFixed(const char* value, size_t size, size_t offset) {
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

    inline bool AddBytes(void* pvalue, size_t size) {
        if (m_writerOffset + size > m_maxLength) {
            return false;
        }
        AddBytes(pvalue, size, m_writerOffset);
        m_writerOffset += size;
        return true;
    }

    inline bool AddBytes(void* pvalue, size_t size, size_t offset) {
        if (offset + size > m_maxLength) {
            return false;
        }
        memcpy(&m_buffer[offset], pvalue, size);
        m_length = offset + size > m_length ? offset + size : m_length;
        return true;
    }

	template<class T> inline typename boost::enable_if<boost::is_fundamental<T>, Buffer&>::type operator>>(T value)
	{
		size_t size = sizeof(T);
		if(Get(value, m_writerOffset))
			m_readerOffset += size;
		return *this;
	}

	inline Buffer& operator>>(Packable& value)
	{
		value.Unpack(shared_from_this());
		return *this;
	}

	inline Buffer& operator>>(string& value)
	{
		char *str;
		*this >> str;
		value = str;
		delete str;
		return *this;
	}

	inline Buffer& operator>>(char*& value)
	{
		size_t size = strlen((char*)&m_buffer[m_readerOffset]) + 1;
		if(GetStringSizeFixed(value, size, m_readerOffset))
			m_readerOffset += size;
		return *this;
	}

	template<class T> inline typename boost::enable_if<boost::is_fundamental<T>, bool>::type Get(T value, size_t offset) {
        size_t size = sizeof(T);
        if (offset + size > m_maxLength) return false;
        value = *(T*)&m_buffer[offset];
        return true;
    }

	inline bool GetPack(Packable& value, size_t offset) {
		size_t readof = m_writerOffset;
        SetReaderOffset(offset);
        value.Unpack(shared_from_this());
        SetReaderOffset(readof);
        return true;
    }

	inline bool GetStringSizeFixed(char*& value, size_t size, size_t offset) {
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
protected:
    vector<byte> m_buffer;
    size_t m_maxLength;
    size_t m_length;
    size_t m_readerOffset;
    size_t m_writerOffset;
};

//using namespace math;
//
//template<> inline Buffer& Buffer::Add(Half value) {
//    return Add((uint16)value);
//}
//
//template<> inline Buffer& Buffer::Add(Half value, size_t offset) {
//    return Add((uint16)value, offset);
//}
//
//template<> inline Half Buffer::Get<Half>() {
//    return Get<uint16>();
//}
//
//template<> inline Half Buffer::Get<Half>(size_t offset) {
//    return Get<uint16>(offset);
//}

//template<> inline char* Buffer::GetString<char*>() {
//    if (m_readerOffset >= m_length) {
//        return NULL;
//    }
//    size_t size = strlen((char*) &m_buffer[m_readerOffset]) + 1;
//    if (m_readerOffset + size > m_length) {
//        return NULL;
//    }
//    char* value = new char[size];
//    strcpy(value, (char*) &m_buffer[m_readerOffset]);
//    m_readerOffset += size;
//    return value;
//}
//
//template<> inline char* Buffer::GetString<char*>(size_t offset) {
//    if (offset >= m_length) {
//        return NULL;
//    }
//    size_t size = strlen((char*) &m_buffer[offset]) + 1;
//    if (offset + size >= m_length) {
//        return NULL;
//    }
//    char* value = new char[size];
//    strcpy(value, (char*) &m_buffer[offset]);
//    return value;
//}
//
//template<> inline string Buffer::GetString() {
//    char* vreturn = GetString<char*>();
//    if (!vreturn) {
//        return string();
//    }
//    string value(vreturn);
//    delete[] vreturn;
//    return value;
//}
//
//template<> inline string Buffer::GetString(size_t offset) {
//    char* vreturn = GetString<char*>(offset);
//    if (!vreturn) {
//        return string();
//    }
//    string value(vreturn);
//    delete[] vreturn;
//    return value;
//}
//
//template<> inline char* Buffer::GetStringSizeFixed(size_t size) {
//    if (m_readerOffset >= m_length) {
//        return NULL;
//    }
//
//    if (m_readerOffset + size > m_length) {
//        return NULL;
//    }
//    char* value = new char[size + 1];
//    memcpy(value, (char*) &m_buffer[m_readerOffset], size);
//    value[size] = 0;
//    m_readerOffset += size;
//    return value;
//}
//
//template<> inline char* Buffer::GetStringSizeFixed(size_t size, size_t offset) {
//    if (offset >= m_length) {
//        return NULL;
//    }
//
//    if (offset + size >= m_length) {
//        return NULL;
//    }
//    char* value = new char[size + 1];
//    memcpy(value, (char*) &m_buffer[offset], size);
//    value[size] = 0;
//    return value;
//}
//
//template<> inline string Buffer::GetStringSizeFixed(size_t size) {
//    char* vreturn = GetStringSizeFixed<char*>(size);
//    if (!vreturn) {
//        return string();
//    }
//    string value(vreturn);
//    delete[] vreturn;
//    return value;
//}
//
//template<> inline string Buffer::GetStringSizeFixed(size_t size, size_t offset) {
//    char* vreturn = GetStringSizeFixed<char*>(size, offset);
//    if (!vreturn) {
//        return string();
//    }
//    string value(vreturn);
//    delete[] vreturn;
//    return value;
//}

#endif