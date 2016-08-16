
#pragma once

#include "platform_selector.h"

#include <string>

namespace xml
{
	//--------------------------------------------------------------------------
	// to lowercase string
	inline void tolowercase(std::string &str) {
		for (auto &c : str) {
			tolower(c);
		}
	}


	//-------------------------------------------------------------------------
	// string to int
	inline bool toInt(const char* arg, int &val) {
		bool ret = true;
		try {
			val = std::stoi(arg);

		} catch (std::exception &ex) {
			ret = false;
		}

		return ret;
	}

	//-------------------------------------------------------------------------
	// string to int
	inline bool toInt(const std::string& arg, int &val) {
		return toInt(arg.c_str(), val);
	}

	//-------------------------------------------------------------------------
	// string to double
	inline bool toDouble(const char* arg, double &val) {
		bool ret = true;
		try {
			val = std::stod(arg);

		} catch (std::exception &ex) {
			ret = false;
		}

		return ret;
	}

	//-------------------------------------------------------------------------
	// string to double
	inline bool toDouble(const std::string& arg, double &val) {
		return toDouble(arg.c_str(), val);
	}

	//-----------------------------------------------------------------------------
	template <typename T=int_size>
	class DoubleConverter
	{
		public:
		//-----------------------------------------------------------------------------
		// As succinctly as possible, given a value like 12.345:
		// 1. convert the integral value => 12
		// 2. determine number of digits required to represent fraction (3)
		// 3. multiply value by 10^3 or 1000 => 12345.0
		// 4. multiply integral (12) by 1000 => 12000.0
		// 5. integer representation of fraction is now 12345-12000 or 345
		// 6. append string representation to result of 1.
		static std::string Convert(double value,double eps = 0.00001)
		{
			std::string ret;

			// limits of integral representation
			#if defined(_MSC_VER)
				#undef min
				#undef max
			#endif
			T imin = std::numeric_limits<T>::min();
			T imax = std::numeric_limits<T>::max();

			// handle special cases
			if (_IS_NAN(value))
			{
				ret = "nan";
			}
			else if (_IS_INF(value))
			{
				ret = "inf";
			}
			else if (value == 0.0)
			{
				ret = "0";
			}
			else
			{
				// 1. 
				int64_t ival = static_cast<int64_t>(value);
				// check that the floating point value can be represented by an integral value ...
				if (ival <= imin)
				{
					// this is decidedly non-standard ...
					ret = "-inf";
				}
				else if (ival >= imax)
				{
					// ditto but makes sense here I think ...
					ret = "+inf";
				}
				else
				{
					// 
					if (ival == 0)
					{
						ret += (value < 0.0 ? "-0" : "0");
					}
					else
					{
						ret = Chordia::toString(ival);
					}
					// work on the absolute value
					double avalue = fabs(value);
					// 2. how many digits required to represent fractional part?
					int digits = countFractionalDigits(avalue,eps);
					if (digits > 0)
					{
						// add decimal point
						ret += '.';
						// special case for less than 1.0
						if (avalue < 1.0)
						{
							int iv = 0;
							for (int digit = 0; digit < digits; digit++)
							{
								avalue *= 10;
								iv = static_cast<int>(avalue);
								ret += static_cast<char>('0' + iv);
								avalue -= static_cast<double>(iv);
							}
						}
						else
						{
							// 3. calculate multiplicand required to make value integral
							double d1 = pow(10.0,digits);
							// multiply the absolute value to scale it
							double d2 = avalue * d1;
							// 4. scale up integral value for subtraction
							d1 *= std::abs(ival);
							// 5. take integral value
							int64_t iv = static_cast<int64_t>(d2-d1);
							// 6. append conversion of 'integral' fraction
							// ack. need to ensure number is correctly zero padded
							// yipes. https://github.com/g40/esj/issues/10
							// 
							ret += Chordia::toString(iv,10,digits);
						}
					}
				}
			}
			return ret;
		}
	};

	//-----------------------------------------------------------------------------
	// bytes to upper-case hex - skipping the overhead of std::string conversion
	inline std::string BinToHex(const void* pSrc,size_t bytes)
	{
		uint8_t val = 0;
		static char hexChars[] = ("0123456789ABCDEF");
		const uint8_t* data = reinterpret_cast<const uint8_t*>(pSrc);
		std::string dst;
		for (int index = (int)bytes-1; index >= 0; index--)
		{
			val = (data[index] >> 4);
			val &= 0x0F;
			dst += hexChars[val];
			val = (data[index] >> 0);
			val &= 0x0F;
			dst += hexChars[val];
		}
		return dst;
	}

	//-------------------------------------------------------------------------
	inline std::string escape(const std::string& arg)
	{
		char chr = 0;
		std::string ret;
		for (size_t s = 0; s < arg.size(); s++)
		{
			chr = arg[s];
			if (chr == '"' || chr == '\\' || chr == '/')
			{
				ret += '\\';
				ret += chr;
			}
			else if (chr == '\b')
			{
				ret += "\\b";
			}
			else if (chr == '\f')
			{
				ret += "\\f";
			}
			else if (chr == '\n')
			{
				ret += "\\n";
			}
			else if (chr == '\r')
			{
				ret += "\\r";
			}
			else if (chr == '\t')
			{
				ret += "\\t";
			}
			else
			{
				ret += chr;
			}
		}
		return ret;
	}

	//-------------------------------------------------------------------------
	// convert from UTF16|32 to JSON encoded UTF8
	inline std::string escape(const std::wstring& arg)
	{
		int32_t chr = 0;
		std::string ret;
		for (size_t s = 0; s < arg.size(); s++)
		{
			chr = arg[s];
			if (chr == '"' || chr == '\\' || chr == '/')
			{
				ret += '\\';
				ret += static_cast<char>(chr);
			}
			else if (chr == '\b')
			{
				ret += "\\b";
			}
			else if (chr == '\f')
			{
				ret += "\\f";
			}
			else if (chr == '\n')
			{
				ret += "\\n";
			}
			else if (chr == '\r')
			{
				ret += "\\r";
			}
			else if (chr == '\t')
			{
				ret += "\\t";
			}
			// 
			// codepoint is in Basic Multilingual Plane
			else if (chr >= 0x255 && chr <= 0xFFFF)
			{
				// represented as a 4 character hex string
				ret += "\\u";
				ret += Chordia::BinToHex(&chr,sizeof(uint16_t));
			}
			// not in BMP -> construct a UTF-16 surrogate pair
			else if (chr > 0xFFFF)
			{
				chr -= 0x10000;
				int32_t first = 0xD800 | ((chr & 0xffc00) >> 10);
				ret += "\\u";
				ret += Chordia::BinToHex(&first, sizeof(uint16_t));
				int32_t last = 0xDC00 | (chr & 0x003ff);
				ret += "\\u";
				ret += Chordia::BinToHex(&last, sizeof(uint16_t));
			}
			else
			{
				ret += static_cast<char>(chr);
			}
		}
		return ret;
	}

//-----------------------------------------------------------------------------
// Light-weight replacement for stringstream and friends. 
class stringer
{
	//
	std::string m_buffer;

public:

	//-------------------------------------------------------------------------
	stringer()	{}

	//-------------------------------------------------------------------------
	stringer(const stringer& arg) : m_buffer(arg.m_buffer) {}

	//-------------------------------------------------------------------------
	stringer(const std::string& arg) : m_buffer(arg) {}

	//-------------------------------------------------------------------------
	stringer(const char* arg) : m_buffer(arg) {}

	//-------------------------------------------------------------------------
	// for debug support
	stringer(const char* file,int line)
	{
		(*this) << file << "(" << line << ") : ";
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(char arg)
	{
		m_buffer += arg;
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(const char* arg)
	{
		if (arg)
		{
			m_buffer += arg;
		}
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(const std::string& arg)
	{
		m_buffer += arg;
		return (*this);
	}

	//-------------------------------------------------------------------------
	// JME audit: UTF16|32 => UTF8
	stringer& operator<<(const std::wstring& arg)
	{
		m_buffer += convert(arg);
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(bool arg)
	{
		m_buffer += (arg ? "true" : "false");
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(int arg)
	{
		//
		m_buffer += toString(arg,10);
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(unsigned int arg)
	{
		//
		m_buffer += toString(arg,10);
		return (*this);
	}

    //-------------------------------------------------------------------------
    stringer& operator<<(long arg)
    {
        //
        m_buffer += toString(arg,10);
        return (*this);
    }
    

	//-------------------------------------------------------------------------
	stringer& operator<<(double arg)
	{
		// attempt to preserve representation of the original value ...
		m_buffer += Chordia::DoubleConverter<>::Convert(arg);
		return (*this);
	}

	//-------------------------------------------------------------------------
	const char* c_str() const
	{
		return m_buffer.c_str();
	}

	//-------------------------------------------------------------------------
	bool empty() const
	{
		return m_buffer.empty();
	}

	//-------------------------------------------------------------------------
	void clear()
	{
		m_buffer.clear();
	}

	//-------------------------------------------------------------------------
	size_t size() const
	{
		return m_buffer.size();
	}
};  // class

//-----------------------------------------------------------------------------
// debug support
static void Debug(const stringer& sb)
{
#if (defined(_WINDOWS_))
	OutputDebugStringA(sb.c_str());
#elif (defined(__MACH__))
	//NSLog(@"%s",(char*)sb.c_str());
	std::cout << sb.c_str();
#else

#endif
}

#if defined(_DEBUG) || defined(DEBUG)
#define DBMSG(sb)	Debug(Chordia::stringer(__FILE__,__LINE__) << sb << "\r\n");
#else
#define DBMSG(sb)	
#endif

}		// namespace


