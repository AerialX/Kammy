#include <sys.h>

#define KAMMY_VERSION "0.1"
#define KAMMY_MAGIC 0x1337baad

struct Lv2Module;

bool Kammy_Init();
const Lv2Module* Kammy_Load(const void* data);

struct Lv2opd
{
	u64 Address;
	u64 TableOfContents;
	u64 Reserved;
};

struct Lv2Module
{
	u32 Magic;			// KAMMY_MAGIC
	u32 HeaderSize;	// DataOffset = sizeof(Lv2Module) = 0x20
	u64 MainEntry;		// Main OPD entry
	u64 TextBase;		// Compiled base
	u64 TextEnd;		// DataSize = TextEnd - TextBase
	u8  Data[];

	bool Execute(u64* ret = NULL) const;

	bool Verify() const
	{
		return Magic == KAMMY_MAGIC;
	}

	uint32_t GetDataSize() const
	{
		return TextEnd - TextBase;
	}

	const Lv2opd* GetMain() const
	{
		return (const Lv2opd*)(Data + (MainEntry - TextBase));
	}
};
