#include "MiniLZO.h"
#include "stdtypes.h"
#include <vector>

#ifdef MINILZO_USE_ORIGINAL_C_LIBRARY
#include "oberhumer\minilzo.h"
#endif

bool MiniLZO::Compress(byte *src, size_t srcLen, byte *&dst, size_t &dstLen)
{
	dstLen = (srcLen + (srcLen / 16) + 64 + 3);
    dst = new byte[dstLen];

#ifdef MINILZO_USE_ORIGINAL_C_LIBRARY
	if (lzo_init() != LZO_E_OK) return false;
	byte work[LZO1X_1_MEM_COMPRESS];
	lzo_uint tmp = dstLen;
	int ret = lzo1x_1_compress(src, srcLen, dst, &tmp, work);
	dstLen = tmp;
	return ret == LZO_E_OK;
#else
    size_t tmp;

    if (srcLen <= M2_MAX_LEN + 5)
    {
        tmp = srcLen;
        dstLen = 0;
    } 
	else
    {
        byte workmem[DICT_SIZE];

        byte **dict = (byte**)&workmem;
        byte *in_end = src + srcLen;
        byte *ip_end = src + srcLen - M2_MAX_LEN - 5;
        byte *ii = src;
        byte *ip = src + 4;
        byte *op = dst;
        bool literal = false;
        bool match = false;
        uint32 offset;
        uint32 length;
        uint32 index;
        byte *pos;

        while (true)
        {
            offset = 0;
            index = D_INDEX1(ip);
            pos = ip - (ip - dict[index]);
            if (pos < src || (offset = (uint32) (ip - pos)) <= 0 || offset > M4_MAX_OFFSET)
                literal = true;
            else if (offset <= M2_MAX_OFFSET || pos[3] == ip[3])
            {
            } else
            {
                index = D_INDEX2(index);
                pos = ip - (ip - dict[index]);
                if (pos < src || (offset = (uint32) (ip - pos)) <= 0 || offset > M4_MAX_OFFSET)
                    literal = true;
                else if (offset <= M2_MAX_OFFSET || pos[3] == ip[3])
                {
                } else
                    literal = true;
            }

            if (!literal)
            {
                if (*((uint16*) pos) == *((uint16*) ip) && pos[2] == ip[2])
                    match = true;
            }

            literal = false;
            if (!match)
            {
                dict[index] = ip;
                ++ip;
                if (ip >= ip_end)
                    break;
                continue;
            }
            match = false;
            dict[index] = ip;
            if (ip - ii > 0)
            {
                uint32 t = (uint32) (ip - ii);
                if (t <= 3)
                {
                    op[-2] |= (byte) (t);
                } else if (t <= 18)
                    *op++ = (byte) (t - 3);
                else
                {
                    uint32 tt = t - 18;
                    *op++ = 0;
                    while (tt > 255)
                    {
                        tt -= 255;
                        *op++ = 0;
                    }
                    *op++ = (byte) (tt);
                }
                do
                {
                    *op++ = *ii++;
                } while (--t > 0);
            }
            ip += 3;
            if (pos[3] != *ip++ || pos[4] != *ip++ || pos[5] != *ip++
                || pos[6] != *ip++ || pos[7] != *ip++ || pos[8] != *ip++)
            {
                --ip;
                length = (uint32) (ip - ii);
                if (offset <= M2_MAX_OFFSET)
                {
                    --offset;
                    *op++ = (byte) (((length - 1) << 5) | ((offset & 7) << 2));
                    *op++ = (byte) (offset >> 3);
                } else if (offset <= M3_MAX_OFFSET)
                {
                    --offset;
                    *op++ = (byte) (M3_MARKER | (length - 2));
                    *op++ = (byte) ((offset & 63) << 2);
                    *op++ = (byte) (offset >> 6);
                } else
                {
                    offset -= 0x4000;
                    *op++ = (byte) (M4_MARKER | ((offset & 0x4000) >> 11) | (length - 2));
                    *op++ = (byte) ((offset & 63) << 2);
                    *op++ = (byte) (offset >> 6);
                }
            } else
            {
                byte* m = pos + M2_MAX_LEN + 1;
                while (ip < in_end && *m == *ip)
                {
                    ++m;
                    ++ip;
                }
                length = (uint32) (ip - ii);
                if (offset <= M3_MAX_OFFSET)
                {
                    --offset;
                    if (length <= 33)
                        *op++ = (byte) (M3_MARKER | (length - 2));
                    else
                    {
                        length -= 33;
                        *op++ = M3_MARKER | 0;
                        while (length > 255)
                        {
                            length -= 255;
                            *op++ = 0;
                        }
                        *op++ = (byte) (length);
                    }
                } else
                {
                    offset -= 0x4000;
                    if (length <= M4_MAX_LEN)
                        *op++ = (byte) (M4_MARKER | ((offset & 0x4000) >> 11) | (length - 2));
                    else
                    {
                        length -= M4_MAX_LEN;
                        *op++ = (byte) (M4_MARKER | ((offset & 0x4000) >> 11));
                        while (length > 255)
                        {
                            length -= 255;
                            *op++ = 0;
                        }
                        *op++ = (byte) (length);
                    }
                }
                *op++ = (byte) ((offset & 63) << 2);
                *op++ = (byte) (offset >> 6);
            }
            ii = ip;
            if (ip >= ip_end)
                break;
        }
        dstLen = (uint32) (op - dst);
        tmp = (uint32) (in_end - ii);

    }
    if (tmp > 0)
    {
		uint32 ii = (uint32)srcLen - tmp;
        if (dstLen == 0 && tmp <= 238)
        {
            dst[dstLen++] = (byte) (17 + tmp);
        } else if (tmp <= 3)
        {
            dst[dstLen - 2] |= (byte) (tmp);
        } else if (tmp <= 18)
        {
            dst[dstLen++] = (byte) (tmp - 3);
        } else
        {
            uint32 tt = tmp - 18;
            dst[dstLen++] = 0;
            while (tt > 255)
            {
                tt -= 255;
                dst[dstLen++] = 0;
            }
            dst[dstLen++] = (byte) (tt);
        }
        do
        {
			dst[dstLen++] = src[ii++];
        } while (--tmp > 0);
    }
    dst[dstLen++] = M4_MARKER | 1;
    dst[dstLen++] = 0;
    dst[dstLen++] = 0;

    return true;
#endif
}

bool MiniLZO::Decompress(byte *src, size_t srcLen, byte*& dst, size_t &dstLen)
{
	dstLen = srcLen * 20;
    dst = new byte[dstLen];
#ifdef MINILZO_USE_ORIGINAL_C_LIBRARY
	lzo_uint tmp = dstLen;
	int ret = lzo1x_decompress(src, srcLen, dst, &tmp, NULL);
	dstLen = tmp;
	return ret == LZO_E_OK;
#else
    uint32 t = 0;
    byte* input = src;
    byte* output = dst;
    byte* pos = NULL;
    byte* ip_end = input + srcLen;
    byte* op_end = output + dstLen;
    byte* ip = input;
    byte* op = output;
    bool match = false;
    bool match_next = false;
    bool match_done = false;
    bool copy_match = false;
    bool first_literal_run = false;
    bool eof_found = false;

    if (*ip > 17)
    {
        t = (uint32) (*ip++ -17);
        if (t < 4)
            match_next = true;
        else
        {
            if ((uint32) (op_end - op) < t)
            {
                delete dst;
                return false;                
            }
            if ((uint32) (ip_end - ip) < t + 1)
            {
                delete dst;
                return false;
            }
            do
            {
                *op++ = *ip++;
            } while (--t > 0);
            first_literal_run = true;
        }
    }
    while (!eof_found && ip < ip_end)
    {
        if (!match_next && !first_literal_run)
        {
            t = *ip++;
            if (t >= 16)
                match = true;
            else
            {
                if (t == 0)
                {
                    if ((ip_end - ip) < 1)
                    {
                        delete dst;
						return false;
                    }
                    while (*ip == 0)
                    {
                        t += 255;
                        ++ip;
                        if ((ip_end - ip) < 1)
                        {
                            delete dst;
							return false;
                        }
                    }
                    t += (uint32) (15 + *ip++);
                }
                if ((uint32) (op_end - op) < t + 3)
                {
                    delete dst;
					return false;
                }
                if ((uint32) (ip_end - ip) < t + 4)
                {
                    delete dst;
					return false;
                }
                for (int x = 0; x < 4; ++x, ++op, ++ip)
                    *op = *ip;
                if (--t > 0)
                {
                    if (t >= 4)
                    {
                        do
                        {
                            for (int x = 0; x < 4; ++x, ++op, ++ip)
                                *op = *ip;
                            t -= 4;
                        } while (t >= 4);
                        if (t > 0)
                        {
                            do
                            {
                                *op++ = *ip++;
                            } while (--t > 0);
                        }
                    } else
                    {
                        do
                        {
                            *op++ = *ip++;
                        } while (--t > 0);
                    }
                }
            }
        }
        if (!match && !match_next)
        {
            first_literal_run = false;

            t = *ip++;
            if (t >= 16)
                match = true;
            else
            {
                pos = op - (1 + M2_MAX_OFFSET);
                pos -= t >> 2;
                pos -= *ip++ << 2;
                if (pos < output || pos >= op)
                {
                    delete dst;
					return false;
                }
                if ((op_end - op) < 3)
                {
                    delete dst;
					return false;
                }
                *op++ = *pos++;
                *op++ = *pos++;
                *op++ = *pos++;
                match_done = true;
            }
        }
        match = false;
        do
        {
            if (t >= 64)
            {
                pos = op - 1;
                pos -= (t >> 2) & 7;
                pos -= *ip++ << 3;
                t = (t >> 5) - 1;
                if (pos < output || pos >= op)
                {
                    delete dst;
					return false;
                }
                if ((uint32) (op_end - op) < t + 2)
                {
                    delete dst;
					return false;
                }
                copy_match = true;
            } else if (t >= 32)
            {
                t &= 31;
                if (t == 0)
                {
                    if ((ip_end - ip) < 1)
                    {
                        delete dst;
						return false;
                    }
                    while (*ip == 0)
                    {
                        t += 255;
                        ++ip;
                        if ((ip_end - ip) < 1)
                        {
                            delete dst;
							return false;
                        }
                    }
                    t += (uint32) (31 + *ip++);
                }
                pos = op - 1;
                pos -= (*(uint16*) ip) >> 2;
                ip += 2;
            } else if (t >= 16)
            {
                pos = op;
                pos -= (t & 8) << 11;

                t &= 7;
                if (t == 0)
                {
                    if ((ip_end - ip) < 1)
                    {
                        delete dst;
						return false;
                    }
                    while (*ip == 0)
                    {
                        t += 255;
                        ++ip;
                        if ((ip_end - ip) < 1)
                        {
                            delete dst;
							return false;
                        }
                    }
                    t += (uint32) (7 + *ip++);
                }
                pos -= (*(uint16*) ip) >> 2;
                ip += 2;
                if (pos == op)
                    eof_found = true;
                else
                    pos -= 0x4000;
            } else
            {
                pos = op - 1;
                pos -= t >> 2;
                pos -= *ip++ << 2;
                if (pos < output || pos >= op)
                {
                    delete dst;
					return false;
                }
                if ((op_end - op) < 2)
                {
                    delete dst;
					return false;
                }
                *op++ = *pos++;
                *op++ = *pos++;
                match_done = true;
            }
            if (!eof_found && !match_done && !copy_match)
            {
                if (pos < output || pos >= op)
                {
                    delete dst;
					return false;
                }
                if ((uint32) (op_end - op) < t + 2)
                {
                    delete dst;
					return false;
                }
            }
            if (!eof_found && t >= 2 * 4 - 2 && (op - pos) >= 4 && !match_done && !copy_match)
            {
                for (int x = 0; x < 4; ++x, ++op, ++pos)
                    *op = *pos;
                t -= 2;
                do
                {
                    for (int x = 0; x < 4; ++x, ++op, ++pos)
                        *op = *pos;
                    t -= 4;

                } while (t >= 4);
                if (t > 0)
                {
                    do
                    {
                        *op++ = *pos++;
                    } while (--t > 0);
                }
            } else if (!eof_found && !match_done)
            {
                copy_match = false;

                *op++ = *pos++;
                *op++ = *pos++;
                do
                {
                    *op++ = *pos++;
                } while (--t > 0);
            }

            if (!eof_found && !match_next)
            {
                match_done = false;

                t = (uint32) (ip[-2] & 3);
                if (t == 0)
                    break;
            }
            if (!eof_found)
            {
                match_next = false;
                if ((uint32) (op_end - op) < t)
                {
                    delete dst;
					return false;
                }
                if ((uint32) (ip_end - ip) < t + 1)
                {
                    delete dst;
					return false;
                }
                *op++ = *ip++;
                if (t > 1)
                {
                    *op++ = *ip++;
                    if (t > 2)
                        *op++ = *ip++;
                }
                t = *ip++;
            }
        } while (!eof_found && ip < ip_end);
    }
    if (!eof_found)
    {
        delete dst;
		return false;
    } else
    {
        if (ip > ip_end)
        {
            delete dst;
			return false;
        } else if (ip < ip_end)
        {
            delete dst;
			return false;
        }
    }

	dstLen = op - output;
    return true;
#endif
}