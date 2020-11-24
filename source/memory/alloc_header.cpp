#include "memory/alloc_header.h"
#include <cassert>
#include <time.h>


// TODO : リファクタリング


namespace waffle {
namespace memory {


// ヘッダ情報の全体サイズ
wfl::ptrdiff_t AllocHeader::s_HeaderSize = 0;

// ヘッダ情報メンバアドレスへのオフセット値
wfl::array<wfl::ptrdiff_t, AllocHeader::HEADER_INFO_BIT_COUNT> AllocHeader::s_HeaderInfoOffsets = { 0 };

// 有効なヘッダ情報
HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


// TODO : アロケーションポリシーに沿ってメモリ確保
void* AllocHeader::operator new(wfl::size_t bytes)
{
    return new char[bytes];
}

void AllocHeader::operator delete(void* pBlock)
{
    char* p = reinterpret_cast<char*>(pBlock);
    delete[] p;
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
        s_HeaderSize += sizeof(void*);
    }

    // 確保サイズ
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBytes)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBytes)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t);
    }

    // ファイル名
    if (s_HeaderInfos & HeaderInfoFlagBits::FileName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FileName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const char*);
    }

    // 行数
    if (s_HeaderInfos & HeaderInfoFlagBits::Line)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Line)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::uint32_t);
    }

    // 関数名
    if (s_HeaderInfos & HeaderInfoFlagBits::FunctionName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FunctionName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const char*);
    }

    // 確保日時
    if (s_HeaderInfos & HeaderInfoFlagBits::DateTime)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::DateTime)] = s_HeaderSize;
        s_HeaderSize += sizeof(time_t);
    }

    // バックトレースのハッシュ値
    if (s_HeaderInfos & HeaderInfoFlagBits::BackTraceHash)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::BackTraceHash)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t);
    }

    // メモリ破壊検出用シグネチャ
    if (s_HeaderInfos & HeaderInfoFlagBits::Signature)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Signature)] = s_HeaderSize;
        s_HeaderSize += sizeof(Signature*);
    }

    // ブックマーク
    if (s_HeaderInfos & HeaderInfoFlagBits::Bookmark)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Bookmark)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t);
    }

    // 親ヒープ領域
    if (s_HeaderInfos & HeaderInfoFlagBits::Heap)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Heap)] = s_HeaderSize;
        s_HeaderSize += sizeof(Heap*);
    }

    // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    if (s_HeaderInfos & HeaderInfoFlagBits::Next)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Next)] = s_HeaderSize;
        s_HeaderSize += sizeof(AllocHeader*);
    }

    // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
    if (s_HeaderInfos & HeaderInfoFlagBits::Prev)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Prev)] = s_HeaderSize;
        s_HeaderSize += sizeof(AllocHeader*);
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
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
    return ptr;
}

void* AllocHeader::getBlock()
{
    void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
    return ptr;
}

// 確保サイズ
wfl::size_t AllocHeader::getBytes() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
    return *reinterpret_cast<const wfl::size_t*>(ptr);
}

// ファイル名
const char* AllocHeader::getFileName() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::FileName);
    return reinterpret_cast<const char*>(ptr);
}

// 行数
wfl::int32_t AllocHeader::getLine() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Line);
    return *reinterpret_cast<const wfl::int32_t*>(ptr);
}

// 関数名
const char* AllocHeader::getFunctionName() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::FunctionName);
    return reinterpret_cast<const char*>(ptr);
}

// 確保日時
time_t AllocHeader::getDateTime() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::DateTime);
    return *reinterpret_cast<const time_t*>(ptr);
}

// バックトレースのハッシュ値
wfl::size_t AllocHeader::getBackTraceHash() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
    return *reinterpret_cast<const wfl::size_t*>(ptr);
}

// メモリ破壊検出用シグネチャ
AllocHeader::Signature AllocHeader::getSignature() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Signature);
    return *reinterpret_cast<const Signature*>(ptr);
}

// ブックマーク
wfl::size_t AllocHeader::getBookmark() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Bookmark);
    return *reinterpret_cast<const wfl::size_t*>(ptr);
}

// 親ヒープ領域
const Heap* AllocHeader::getHeap() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Heap);
    return reinterpret_cast<const Heap*>(ptr);
}

Heap* AllocHeader::getHeap()
{
    void* ptr = this + getHeaderOffset(HeaderInfoIndex::Heap);
    return reinterpret_cast<Heap*>(ptr);
}

// 次のヘッダへのポインタ (ヒープをウォークするのに必要)
const AllocHeader* AllocHeader::getNext() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
    return reinterpret_cast<const AllocHeader*>(ptr);
}

AllocHeader* AllocHeader::getNext()
{
    void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
    return reinterpret_cast<AllocHeader*>(ptr);
}

// 前のヘッダへのポインタ (ヒープをウォークするのに必要)
const AllocHeader* AllocHeader::getPrev() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Prev);
    return reinterpret_cast<const AllocHeader*>(ptr);
}

AllocHeader* AllocHeader::getPrev()
{
    void* ptr = this + getHeaderOffset(HeaderInfoIndex::Prev);
    return reinterpret_cast<AllocHeader*>(ptr);
}

// リンクリストに追加
void AllocHeader::addLink(AllocHeader* pAllocHeader)
{
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
        wfl::ptrdiff_t& addr = (*reinterpret_cast<wfl::ptrdiff_t*>(ptr));
        addr = reinterpret_cast<wfl::ptrdiff_t>(pAllocHeader);
    }

    {
        void* ptr = pAllocHeader + getHeaderOffset(HeaderInfoIndex::Prev);
        wfl::ptrdiff_t& addr = (*reinterpret_cast<wfl::ptrdiff_t*>(ptr));
        addr = reinterpret_cast<wfl::ptrdiff_t>(this);
    }
}

// リンクリストから切り離す
AllocHeader* AllocHeader::deleteLink()
{
    AllocHeader* pTop = nullptr;

    if (getPrev() == nullptr)
    {
        pTop = getNext();
    }
    else
    {
        {
            void* ptr = getPrev() + getHeaderOffset(HeaderInfoIndex::Next);
            wfl::ptrdiff_t& addr = (*reinterpret_cast<wfl::ptrdiff_t*>(ptr));
            addr = reinterpret_cast<wfl::ptrdiff_t>(getNext());
        }
    }

    if (getNext() != nullptr)
    {
        {
            void* ptr = getNext() + getHeaderOffset(HeaderInfoIndex::Prev);
            wfl::ptrdiff_t& addr = (*reinterpret_cast<wfl::ptrdiff_t*>(ptr));
            addr = reinterpret_cast<wfl::ptrdiff_t>(getPrev());
        }
    }

    return pTop;
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
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBlock);
        wfl::ptrdiff_t& addr = (*reinterpret_cast<wfl::ptrdiff_t*>(ptr));
        addr = reinterpret_cast<wfl::ptrdiff_t>(address);
    }

    if (isEnabled(HeaderInfoFlagBits::MemoryBytes))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
        wfl::size_t& addr = (*reinterpret_cast<wfl::size_t*>(ptr));
        addr = bytes;
    }

    if (isEnabled(HeaderInfoFlagBits::FileName))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::FileName);
        const char*& addr = (*reinterpret_cast<const char**>(ptr));
        addr = file;
    }

    if (isEnabled(HeaderInfoFlagBits::Line))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Line);
        wfl::int32_t& addr = (*reinterpret_cast<wfl::int32_t*>(ptr));
        addr = line;
    }

    if (isEnabled(HeaderInfoFlagBits::FunctionName))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::FunctionName);
        const char*& addr = (*reinterpret_cast<const char**>(ptr));
        addr = function;
    }

    if (isEnabled(HeaderInfoFlagBits::DateTime))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::DateTime);
        time_t& addr = (*reinterpret_cast<time_t*>(ptr));
        addr = time(NULL); // TODO : クロスプラットフォーム
    }

    if (isEnabled(HeaderInfoFlagBits::BackTraceHash))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
        wfl::size_t& addr = (*reinterpret_cast<wfl::size_t*>(ptr));
        addr = backTraceHash;
    }

    if (isEnabled(HeaderInfoFlagBits::Signature))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Signature);
        Signature& addr = (*reinterpret_cast<Signature*>(ptr));
        addr = SIGNATURE;
    }

    if (isEnabled(HeaderInfoFlagBits::Bookmark))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Bookmark);
        wfl::size_t& addr = (*reinterpret_cast<wfl::size_t*>(ptr));
        addr = bookmark;
    }

    if (isEnabled(HeaderInfoFlagBits::Heap))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Heap);
        Heap*& addr = (*reinterpret_cast<Heap**>(ptr));
        addr = pHeap;
    }

    if (isEnabled(HeaderInfoFlagBits::Next))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Next);
        wfl::nullptr_t& addr = (*reinterpret_cast<wfl::nullptr_t*>(ptr));
        addr = nullptr;
    }

    if (isEnabled(HeaderInfoFlagBits::Prev))
    {
        void* ptr = this + getHeaderOffset(HeaderInfoIndex::Prev);
        wfl::nullptr_t& addr = (*reinterpret_cast<wfl::nullptr_t*>(ptr));
        addr = nullptr;
    }
}

bool AllocHeader::isValidSignature() const
{
    return getSignature() == SIGNATURE;
}


} // namespace memory
} // namespace waffle

