#pragma once

class String
{
public:
	static const u64 npos = -1;
protected:
	char* buffer;

	void Construct(const char* string, u64 len = npos)
	{
		if (buffer)
			free(buffer);
		if (len == npos)
			len = strlen(string);
		buffer = (char*)malloc(len + 1);
		if (buffer) {
			if (len)
				strncpy(buffer, string, len);
			buffer[len] = '\0';
		}
	}
public:
	String() : buffer(NULL) { Construct(""); }
	String(const String& string) : buffer(NULL) { Construct(string.C()); }
	String(const String& string, u64 off, u64 len = npos) : buffer(NULL) { Construct(string.C() + off, len); }
	String(const char* string, u64 len = npos) : buffer(NULL) { Construct(string, len); }

	~String() { free(buffer); buffer = NULL; }

	String& operator =(const String& string)
	{
		Construct(string.C(), string.Size());
		return *this;
	}
	String& operator =(const char* string)
	{
		Construct(string);
		return *this;
	}

	String operator +(const String& string) const
	{
		String out(*this, 0, Size() + string.Size());
		strcat(out.buffer, string.buffer);
		return out;
	}

	String& operator +=(const String& string)
	{
		char* newbuffer = (char*)malloc(Size() + string.Size());
		strcpy(newbuffer, buffer);
		strcat(newbuffer, string.buffer);
		free(buffer);
		buffer = newbuffer;
		return *this;
	}

	const char* C() const { return buffer; }

	char& operator [](u64 index) { return buffer[index]; }

	u64 Size() const { return strlen(buffer); }
	u64 Length() const { return strlen(buffer); } // TODO: UTF8

	s64 Compare(const String& string) const { return strcmp(buffer, string.C()); }
	s64 Compare(const String& string, u64 len) const { return strncmp(buffer, string.C(), len); }
	s64 Compare(u64 off1, const String& string, u64 off2, u64 len) const { return strncmp(buffer + off1, string.C() + off2, len); }
	s64 Compare(const char* string) const { return strcmp(buffer, string); }
	s64 Compare(const char* string, u64 len) const { return strncmp(buffer, string, len); }
	s64 Compare(u64 off1, const char* string, u64 len) const { return strncmp(buffer + off1, string, len); }

	String Substring(u64 off = 0, u64 len = npos) const { return String(buffer + off, len); }
};
