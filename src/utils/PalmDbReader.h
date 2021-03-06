/* Copyright 2020 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

// http://en.wikipedia.org/wiki/PDB_(Palm_OS)
// http://wiki.mobileread.com/wiki/PDB
struct PdbHeader {
    /* 31 chars + 1 null terminator */
    char name[32] = {0};
    u16 attributes = 0;
    u16 version = 0;
    u32 createTime = 0;
    u32 modifyTime = 0;
    u32 backupTime = 0;
    u32 modificationNumber = 0;
    u32 appInfoID = 0;
    u32 sortInfoID = 0;
    // 8 bytes in the file +1 for zero termination
    char typeCreator[8 + 1] = {0};
    u32 idSeed = 0;
    u32 nextRecordList = 0;
    u16 numRecords = 0;
};

struct PdbRecordHeader {
    u32 offset = 0;
    u8 flags = 0; // deleted, dirty, busy, secret, category
    char uniqueID[3] = {0};
};

class PdbReader {
    // content of pdb file
    const char* data = nullptr;
    size_t dataSize = 0;

    // offset of each pdb record within the file + a sentinel
    // value equal to file size to simplify use
    Vec<PdbRecordHeader> recInfos;

    bool ParseHeader();

  public:
    PdbHeader hdr;

    PdbReader() = default;
    ~PdbReader();

    bool Parse(std::string_view);

    const char* GetDbType();
    size_t GetRecordCount();
    std::string_view GetRecord(size_t recNo);

    static PdbReader* CreateFromData(std::string_view);
    static PdbReader* CreateFromFile(const char* filePath);

#if OS_WIN
    static PdbReader* CreateFromFile(const WCHAR* filePath);
    static PdbReader* CreateFromStream(IStream* stream);
#endif
};
