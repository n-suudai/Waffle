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
T readValue(void* pBlock)
{
    T* p = reinterpret_cast<T*>(pBlock);
    return (*p);
}

// ヘッダ情報の全体サイズ
wfl::ptrdiff_t AllocHeader::s_HeaderSize = 0;

// ヘッダ情報メンバアドレスへのオフセット値
wfl::array<wfl::ptrdiff_t, AllocHeader::HEADER_INFO_BIT_COUNT> AllocHeader::s_HeaderInfoOffsets = { 0 };

// 有効なヘッダ情報
HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


//// TODO : アロケーションポリシーに沿ってメモリ確保
//void* AllocHeader::operator new(wfl::size_t bytes)
//{
//    char* p = new char[bytes];
//    return p;
//}
//
//void AllocHeader::operator delete(void* pBlock)
//{
//    char* p = reinterpret_cast<char*>(pBlock);
//    delete[] p;
//}

AllocHeader::AllocHeader()
    : m_pBuffer(nullptr)
{
    m_pBuffer = new char[getHeaderSize()];
}

AllocHeader::~AllocHeader()
{
    delete[] m_pBuffer;
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

// 終了
void AllocHeader::terminate()
{
    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = HeaderInfoFlagBits::Required;
}

// 有効化されているか
bool AllocHeader::isEnabled(HeaderInfoFlags headerInfos)
{
    return (bool)(s_HeaderInfos & headerInfos);
}

// オフセット取得
wfl::ptrdiff_t AllocHeader::getHeaderOffset(HeaderInfoIndex headerInfoIndex)
{
    return s_HeaderInfoOffsets[static_cast<int>(headerInfoIndex)];
}

// ヘッダ情報の全体サイズを取得
wfl::size_t AllocHeader::getHeaderSize()
{
    return s_HeaderSize;
}

// アドレス
const void* AllocHeader::getBlock() const
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBlock))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        return readValue<const void*>(ptr);
    }
    return nullptr;
}

void* AllocHeader::getBlock()
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBlock))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        return readValue<void*>(ptr);
    }
    return nullptr;
}

// 確保サイズ
wfl::size_t AllocHeader::getBytes() const
{
    if (isEnabled(HeaderInfoFlagBits::MemoryBytes))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
        return readValue<wfl::size_t>(ptr);
    }
    return 0;
}

// ファイル名
const char* AllocHeader::getFileName() const
{
    if (isEnabled(HeaderInfoFlagBits::FileName))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::FileName);
        return readValue<const char*>(ptr);
    }
    return "UNKNOWN FILE";
}

// 行数
wfl::int32_t AllocHeader::getLine() const
{
    if (isEnabled(HeaderInfoFlagBits::Line))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Line);
        return readValue<wfl::int32_t>(ptr);
    }
    return 0;
}

// 関数名
const char* AllocHeader::getFunctionName() const
{
    if (isEnabled(HeaderInfoFlagBits::FunctionName))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::FunctionName);
        return readValue<const char*>(ptr);
    }
    return "UNKNOWN FUNCTION";
}

// 確保日時
time_t AllocHeader::getDateTime() const
{
    if (isEnabled(HeaderInfoFlagBits::DateTime))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::DateTime);
        return readValue<time_t>(ptr);
    }
    return 0;
}

// バックトレースのハッシュ値
wfl::size_t AllocHeader::getBackTraceHash() const
{
    if (isEnabled(HeaderInfoFlagBits::BackTraceHash))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
        return readValue<wfl::size_t>(ptr);
    }
    return 0;
}

// メモリ破壊検出用シグネチャ
AllocHeader::Signature AllocHeader::getSignature() const
{
    if (isEnabled(HeaderInfoFlagBits::Signature))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Signature);
        return *readValue<Signature*>(ptr);
    }
    return 0;
}

// ブックマーク
wfl::size_t AllocHeader::getBookmark() const
{
    if (isEnabled(HeaderInfoFlagBits::Bookmark))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Bookmark);
        return readValue<wfl::size_t>(ptr);
    }
    return 0;
}

// 親ヒープ領域
const Heap* AllocHeader::getHeap() const
{
    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Heap);
        return readValue<const Heap*>(ptr);
    }
    return nullptr;
}

Heap* AllocHeader::getHeap()
{
    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Heap);
        return readValue<Heap*>(ptr);
    }
    return nullptr;
}

// 次のヘッダへのポインタ (ヒープをウォークするのに必要)
const AllocHeader* AllocHeader::getNext() const
{
    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Next);
        return readValue<const AllocHeader*>(ptr);
    }
    return nullptr;
}

AllocHeader* AllocHeader::getNext()
{
    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Next);
        return readValue<AllocHeader*>(ptr);
    }
    return nullptr;
}

// 前のヘッダへのポインタ (ヒープをウォークするのに必要)
const AllocHeader* AllocHeader::getPrev() const
{
    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Prev);
        return readValue<const AllocHeader*>(ptr);
    }
    return nullptr;
}

AllocHeader* AllocHeader::getPrev()
{
    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Prev);
        return readValue<AllocHeader*>(ptr);
    }
    return nullptr;
}

// リンクリストに追加
void AllocHeader::addLink(AllocHeader* pAllocHeader)
{
    if (!isEnabled(HeaderInfoFlagBits::Required)) { return; }

    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Next);
        writeValue<AllocHeader*>(pAllocHeader, ptr);
    }

    if (pAllocHeader)
    {
        void* ptr = pAllocHeader->m_pBuffer + getHeaderOffset(HeaderInfoIndex::Prev);
        writeValue<AllocHeader*>(this, ptr);
    }
}

// リンクリストから切り離す
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
            void* ptr = getPrev()->m_pBuffer + getHeaderOffset(HeaderInfoIndex::Next);
            writeValue<AllocHeader*>(getNext(), ptr);
        }
    }

    if (getNext() != nullptr)
    {
        {
            void* ptr = getNext()->m_pBuffer + getHeaderOffset(HeaderInfoIndex::Prev);
            writeValue<AllocHeader*>(getPrev(), ptr);
        }
    }
}

// 情報書き込み
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
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        writeValue<void*>(address, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::MemoryBytes))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
        writeValue<wfl::size_t>(bytes, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::FileName))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::FileName);
        writeValue<const char*>(file, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Line))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Line);
        writeValue<wfl::uint32_t>(static_cast<wfl::uint32_t>(line), ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::FunctionName))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::FunctionName);
        writeValue<const char*>(function, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::DateTime))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::DateTime);
        writeValue<time_t>(time(NULL), ptr); // TODO : クロスプラットフォーム
    }

    if (isEnabled(HeaderInfoFlagBits::BackTraceHash))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
        writeValue<wfl::size_t>(backTraceHash, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Signature))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Signature);
        Signature* pSignature = reinterpret_cast<Signature*>(
            reinterpret_cast<std::ptrdiff_t>(address) + bytes);
        writeValue<Signature*>(pSignature, ptr);
        (*pSignature) = SIGNATURE;
    }

    if (isEnabled(HeaderInfoFlagBits::Bookmark))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Bookmark);
        writeValue<wfl::size_t>(bookmark, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Heap);
        writeValue<Heap*>(pHeap, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Next);
        writeValue<AllocHeader*>(nullptr, ptr);
    }

    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        void* ptr = m_pBuffer + getHeaderOffset(HeaderInfoIndex::Prev);
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

