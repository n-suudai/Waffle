#include "core/memory/alloc_header.h"


namespace Engine {
namespace Memory {


// ヘッダ情報の全体サイズ
EPtrDiff AllocHeader::s_HeaderSize = 0;

// ヘッダ情報メンバアドレスへのオフセット値
EArray<EPtrDiff, AllocHeader::HEADER_INFO_BIT_COUNT> AllocHeader::s_HeaderInfoOffsets = { 0 };

// 有効なヘッダ情報
HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


// どのヘッダ情報を保持するかを渡して初期化
EBool AllocHeader::Initialize(HeaderInfoFlags headerInfos)
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
        s_HeaderSize += sizeof(EVoid*);
    }

    // 確保サイズ
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBytes)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBytes)] = s_HeaderSize;
        s_HeaderSize += sizeof(ESize);
    }

    // ファイル名
    if (s_HeaderInfos & HeaderInfoFlagBits::FileName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FileName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const EChar*);
    }

    // 行数
    if (s_HeaderInfos & HeaderInfoFlagBits::Line)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Line)] = s_HeaderSize;
        s_HeaderSize += sizeof(EUInt32);
    }

    // 関数名
    if (s_HeaderInfos & HeaderInfoFlagBits::FunctionName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FunctionName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const EChar*);
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
        s_HeaderSize += sizeof(ESize);
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
        s_HeaderSize += sizeof(ESize);
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

    return true;
}

// 終了
EVoid AllocHeader::Terminate()
{
    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = HeaderInfoFlagBits::Required;
}

// 有効化されているか
EBool AllocHeader::IsEnabled(HeaderInfoFlags headerInfos)
{
    return (EBool)(s_HeaderInfos & headerInfos);
}

// オフセット取得
EPtrDiff AllocHeader::GetHeaderOffset(HeaderInfoIndex headerInfoIndex)
{
    return s_HeaderInfoOffsets[static_cast<int>(headerInfoIndex)];
}

// ヘッダ情報の全体サイズを取得
ESize AllocHeader::GetHeaderSize()
{
    return s_HeaderSize;
}

// アドレス
const EVoid* AllocHeader::GetBlock() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBlock);
    return ptr;
}

EVoid* AllocHeader::GetBlock()
{
    EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBlock);
    return ptr;
}

// 確保サイズ
ESize AllocHeader::GetBytes() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBytes);
    return *reinterpret_cast<const ESize*>(ptr);
}

// ファイル名
const EChar* AllocHeader::GetFileName() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::FileName);
    return reinterpret_cast<const EChar*>(ptr);
}

// 行数
EUInt32 AllocHeader::GetLine() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Line);
    return *reinterpret_cast<const EUInt32*>(ptr);
}

// 関数名
const EChar* AllocHeader::GetFunctionName() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::FunctionName);
    return reinterpret_cast<const EChar*>(ptr);
}

// 確保日時
time_t AllocHeader::GetDateTime() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::DateTime);
    return *reinterpret_cast<const time_t*>(ptr);
}

// バックトレースのハッシュ値
ESize AllocHeader::GetBackTraceHash() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::BackTraceHash);
    return *reinterpret_cast<const ESize*>(ptr);
}

// メモリ破壊検出用シグネチャ
AllocHeader::Signature AllocHeader::GetSignature() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Signature);
    return *reinterpret_cast<const Signature*>(ptr);
}

// ブックマーク
ESize AllocHeader::GetBookmark() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Bookmark);
    return *reinterpret_cast<const ESize*>(ptr);
}

// 親ヒープ領域
const Heap* AllocHeader::GetHeap() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Heap);
    return reinterpret_cast<const Heap*>(ptr);
}

Heap* AllocHeader::GetHeap()
{
    EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Heap);
    return reinterpret_cast<Heap*>(ptr);
}

// 次のヘッダへのポインタ (ヒープをウォークするのに必要)
const AllocHeader* AllocHeader::GetNext() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Next);
    return reinterpret_cast<const AllocHeader*>(ptr);
}

AllocHeader* AllocHeader::GetNext()
{
    EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Next);
    return reinterpret_cast<AllocHeader*>(ptr);
}

// 前のヘッダへのポインタ (ヒープをウォークするのに必要)
const AllocHeader* AllocHeader::GetPrev() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Prev);
    return reinterpret_cast<const AllocHeader*>(ptr);
}

AllocHeader* AllocHeader::GetPrev()
{
    EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Prev);
    return reinterpret_cast<AllocHeader*>(ptr);
}


} // namespace Memory
} // namespace Engine

