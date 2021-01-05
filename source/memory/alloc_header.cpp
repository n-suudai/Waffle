#include "memory/alloc_header.h"
#include <cassert>
#include <time.h>


// TODO : リファクタリング


namespace waffle {
namespace memory {


template<typename T>
void writeValue(const T& value, void* pBlock)
{
    T* p = reinterpret_cast<T*>(pBlock);
    (*p) = value;
}

template<typename T>
const T& readValue(const void* pBlock)
{
    const T* p = reinterpret_cast<const T*>(pBlock);
    return (*p);
}

// ヘッダ情報の全体サイズ
wfl::ptrdiff_t AllocHeader::s_HeaderSize = 0;

// ヘッダ情報メンバアドレスへのオフセット値
wfl::array<wfl::ptrdiff_t, AllocHeader::HEADER_INFO_BIT_COUNT> AllocHeader::s_HeaderInfoOffsets = { 0 };

// 有効なヘッダ情報
HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


AllocHeader::AllocHeader()
{

}

AllocHeader::~AllocHeader()
{

}

// どのヘッダ情報を保持するかを渡して初期化
bool AllocHeader::initialize(HeaderInfoFlags headerInfos)
{
    if ((headerInfos & HeaderInfoFlagBits::Required) != HeaderInfoFlagBits::Required)
    {
        return false;
    }

    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = headerInfos;

    // アドレス
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBlock)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBlock)] = s_HeaderSize;
        s_HeaderSize += sizeof(void*); // 8
    }

    // 確保サイズ
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBytes)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBytes)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t); // 8
    }

    // ファイル名
    if (s_HeaderInfos & HeaderInfoFlagBits::FileName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FileName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const char*); // 8
    }

    // 行数
    if (s_HeaderInfos & HeaderInfoFlagBits::Line)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Line)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::uint32_t); // 4
    }

    // 関数名
    if (s_HeaderInfos & HeaderInfoFlagBits::FunctionName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FunctionName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const char*); // 8
    }

    // 確保日時
    if (s_HeaderInfos & HeaderInfoFlagBits::DateTime)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::DateTime)] = s_HeaderSize;
        s_HeaderSize += sizeof(time_t); // 8
    }

    // バックトレースのハッシュ値
    if (s_HeaderInfos & HeaderInfoFlagBits::BackTraceHash)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::BackTraceHash)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t); // 8
    }

    // メモリ破壊検出用シグネチャ
    if (s_HeaderInfos & HeaderInfoFlagBits::Signature)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Signature)] = s_HeaderSize;
        s_HeaderSize += sizeof(Signature*); // 8
    }

    // ブックマーク
    if (s_HeaderInfos & HeaderInfoFlagBits::Bookmark)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Bookmark)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t); // 8
    }

    // 親ヒープ領域
    if (s_HeaderInfos & HeaderInfoFlagBits::Heap)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Heap)] = s_HeaderSize;
        s_HeaderSize += sizeof(Heap*); // 8
    }

    // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    if (s_HeaderInfos & HeaderInfoFlagBits::Next)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Next)] = s_HeaderSize;
        s_HeaderSize += sizeof(AllocHeader*); // 8
    }

    // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
    if (s_HeaderInfos & HeaderInfoFlagBits::Prev)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Prev)] = s_HeaderSize;
        s_HeaderSize += sizeof(AllocHeader*); // 8
    }

    // 必須項目が含まれているかチェック
    assert(isEnabled(HeaderInfoFlagBits::Required));

    return true;
}

void AllocHeader::finalize()
{
    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = HeaderInfoFlagBits::Required;
}

bool AllocHeader::isInitialized()
{
    return s_HeaderSize > 0;
}

bool AllocHeader::isEnabled(HeaderInfoFlags headerInfos)
{
    return (bool)(s_HeaderInfos & headerInfos);
}

wfl::ptrdiff_t AllocHeader::getHeaderOffset(HeaderInfoIndex headerInfoIndex)
{
    return s_HeaderInfoOffsets[static_cast<int>(headerInfoIndex)];
}

wfl::size_t AllocHeader::getHeaderSize()
{
    return s_HeaderSize;
}

const void* AllocHeader::getBlock() const
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBlock))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        return readValue<const void*>(ptr);
    }
    return nullptr;
}

void* AllocHeader::getBlock()
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBlock))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        return readValue<void*>(ptr);
    }
    return nullptr;
}

wfl::size_t AllocHeader::getBytes() const
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBytes))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
        return readValue<wfl::size_t>(ptr);
    }
    return 0;
}

const char* AllocHeader::getFileName() const
{
    if (isEnabled(HeaderInfoFlagBits::FileName))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::FileName);
        return readValue<const char*>(ptr);
    }
    return "UNKNOWN FILE";
}

wfl::int32_t AllocHeader::getLine() const
{
    if (isEnabled(HeaderInfoFlagBits::Line))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Line);
        return readValue<wfl::int32_t>(ptr);
    }
    return 0;
}

const char* AllocHeader::getFunctionName() const
{
    if (isEnabled(HeaderInfoFlagBits::FunctionName))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::FunctionName);
        return readValue<const char*>(ptr);
    }
    return "UNKNOWN FUNCTION";
}

time_t AllocHeader::getDateTime() const
{
    if (isEnabled(HeaderInfoFlagBits::DateTime))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::DateTime);
        return readValue<time_t>(ptr);
    }
    return 0;
}

wfl::size_t AllocHeader::getBackTraceHash() const
{
    if (isEnabled(HeaderInfoFlagBits::BackTraceHash))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
        return readValue<wfl::size_t>(ptr);
    }
    return 0;
}

AllocHeader::Signature AllocHeader::getSignature() const
{
    if (isEnabled(HeaderInfoFlagBits::Signature))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Signature);
        return *readValue<Signature*>(ptr);
    }
    return 0;
}

wfl::size_t AllocHeader::getBookmark() const
{
    if (isEnabled(HeaderInfoFlagBits::Bookmark))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Bookmark);
        return readValue<wfl::size_t>(ptr);
    }
    return 0;
}

const Heap* AllocHeader::getHeap() const
{
    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Heap);
        return readValue<const Heap*>(ptr);
    }
    return nullptr;
}

Heap* AllocHeader::getHeap()
{
    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Heap);
        return readValue<Heap*>(ptr);
    }
    return nullptr;
}

const AllocHeader* AllocHeader::getNext() const
{
    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
        return readValue<const AllocHeader*>(ptr);
    }
    return nullptr;
}

AllocHeader* AllocHeader::getNext()
{
    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
        return readValue<AllocHeader*>(ptr);
    }
    return nullptr;
}

const AllocHeader* AllocHeader::getPrev() const
{
    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Prev);
        return readValue<const AllocHeader*>(ptr);
    }
    return nullptr;
}

AllocHeader* AllocHeader::getPrev()
{
    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Prev);
        return readValue<AllocHeader*>(ptr);
    }
    return nullptr;
}

void AllocHeader::addLink(AllocHeader* pAllocHeader)
{
    if (!isEnabled(HeaderInfoFlagBits::Required)) { return; }

    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
        writeValue<AllocHeader*>(pAllocHeader, ptr);
    }

    if (pAllocHeader)
    {
        void* ptr = pAllocHeader + getHeaderOffset(HeaderInfoIndex::Prev);
        writeValue<AllocHeader*>(this, ptr);
    }
}

void AllocHeader::deleteLink(AllocHeader*& pAllocHeader)
{
    if (!isEnabled(HeaderInfoFlagBits::Required)) { return; }

    if (getPrev() == nullptr)
    {
        assert(this == pAllocHeader);
        pAllocHeader = getNext();
    }
    else
    {
        {
            void* ptr = getPrev() + getHeaderOffset(HeaderInfoIndex::Next);
            writeValue<AllocHeader*>(getNext(), ptr);
        }
    }

    if (getNext() != nullptr)
    {
        {
            void* ptr = getNext() + getHeaderOffset(HeaderInfoIndex::Prev);
            writeValue<AllocHeader*>(getPrev(), ptr);
        }
    }
}

void AllocHeader::record(
    void* address,
    wfl::size_t bytes,
    const char* file,
    wfl::int32_t line,
    const char* function,
    wfl::size_t backTraceHash,
    wfl::size_t bookmark,
    Heap* pHeap
    )
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBlock))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        writeValue<void*>(address, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::MemoryBytes))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
        writeValue<wfl::size_t>(bytes, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::FileName))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::FileName);
        writeValue<const char*>(file, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Line))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Line);
        writeValue<wfl::uint32_t>(static_cast<wfl::uint32_t>(line), ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::FunctionName))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::FunctionName);
        writeValue<const char*>(function, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::DateTime))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::DateTime);
        writeValue<time_t>(time(NULL), ptr); // TODO : クロスプラットフォーム
    }

    if (isEnabled(HeaderInfoFlagBits::BackTraceHash))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
        writeValue<wfl::size_t>(backTraceHash, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Signature))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Signature);
        Signature* pSignature = reinterpret_cast<Signature*>(
            reinterpret_cast<wfl::ptrdiff_t>(address) + bytes);
        writeValue<Signature*>(pSignature, ptr);
        (*pSignature) = SIGNATURE;
    }

    if (isEnabled(HeaderInfoFlagBits::Bookmark))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Bookmark);
        writeValue<wfl::size_t>(bookmark, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Heap);
        writeValue<Heap*>(pHeap, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
        writeValue<AllocHeader*>(nullptr, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Prev);
        writeValue<AllocHeader*>(nullptr, ptr);
    }
}

bool AllocHeader::isValidSignature() const
{
    if (isEnabled(HeaderInfoFlagBits::Signature))
    {
        return getSignature() == SIGNATURE;
    }
    return true;
}


} // namespace memory
} // namespace waffle

