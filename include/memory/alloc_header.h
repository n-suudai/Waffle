#pragma once


#include "common/utility/flags.h"
#include "memory/policy.h"
#include <array>


namespace waffle {
namespace memory {


enum class HeaderInfoIndex : wfl::uint16_t
{
      MemoryBlock       // アドレス
    , MemoryBytes       // 確保サイズ
    , FileName          // ファイル名
    , Line              // 行数
    , FunctionName      // 関数名
    , DateTime          // 確保日時
    , BackTraceHash     // バックトレースのハッシュ値
    , Signature         // メモリ破壊検出用シグネチャ
    , Bookmark          // ブックマーク
    , Heap              // 親ヒープ領域
    , Next              // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    , Prev              // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
    , Num
};

enum class HeaderInfoFlagBits
{
      MemoryBlock   = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::MemoryBlock))   // アドレス
    , MemoryBytes   = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::MemoryBytes))   // 確保サイズ
    , FileName      = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::FileName))      // ファイル名
    , Line          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Line))          // 行数
    , FunctionName  = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::FunctionName))  // 関数名
    , DateTime      = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::DateTime))      // 確保日時
    , BackTraceHash = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::BackTraceHash)) // バックトレースのハッシュ値
    , Signature     = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Signature))     // メモリ破壊検出用シグネチャ
    , Bookmark      = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Bookmark))      // ブックマーク
    , Heap          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Heap))          // 親ヒープ領域
    , Next          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Next))          // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    , Prev          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Prev))          // 前のヘッダへのポインタ (ヒープをウォークするのに必要)

    // 必須
    , Required = MemoryBlock
        | MemoryBytes
        | Heap
        | Next
        | Prev

    // すべて
    , All = MemoryBlock
        | MemoryBytes
        | FileName
        | Line
        | FunctionName
        | DateTime
        | BackTraceHash
        | Signature
        | Bookmark
        | Heap
        | Next
        | Prev
};


typedef Flags<HeaderInfoFlagBits> HeaderInfoFlags;

class Heap;

// ヘッダ情報
class AllocHeader final
{
private:
    static void* operator new[](wfl::size_t bytes) = delete;
    static void operator delete[](void* pBlock) = delete;

public:
    static inline void* operator new(wfl::size_t bytes)
    {
        return AllocatePolicy::allocate(bytes + getHeaderSize());
    }

    static void operator delete(void* pBlock)
    {
        AllocatePolicy::deallocate(pBlock);
    }

public:
    typedef wfl::uint32_t Signature;

    // シグネチャ定義
    static constexpr Signature SIGNATURE = 0xCDCDCDCD;

    // ヘッダ情報の最大メンバ数
    static constexpr wfl::size_t HEADER_INFO_BIT_COUNT = sizeof(HeaderInfoFlagBits) * 8;

public:
    AllocHeader();

    ~AllocHeader();

public:
    // どのヘッダ情報を保持するかを渡して初期化
    static bool initialize(HeaderInfoFlags headerInfos = HeaderInfoFlagBits::Required);

    // 終了
    static void terminate();

    // 有効化されているか
    static bool isEnabled(HeaderInfoFlags headerInfos);

    // オフセット取得
    static wfl::ptrdiff_t getHeaderOffset(HeaderInfoIndex headerInfoIndex);

    // ヘッダ情報の全体サイズを取得
    static wfl::size_t getHeaderSize();

    // アドレス
    const void* getBlock() const;
    void* getBlock();

    // 確保サイズ
    wfl::size_t getBytes() const;

    // ファイル名
    const char* getFileName() const;

    // 行数
    wfl::int32_t getLine() const;

    // 関数名
    const char* getFunctionName() const;

    // 確保日時
    time_t getDateTime() const;

    // バックトレースのハッシュ値
    wfl::size_t getBackTraceHash() const;

    // メモリ破壊検出用シグネチャ
    Signature getSignature() const;

    // ブックマーク
    wfl::size_t getBookmark() const;

    // 親ヒープ領域
    const Heap* getHeap() const;
    Heap* getHeap();

    // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    const AllocHeader* getNext() const;
    AllocHeader* getNext();

    // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
    const AllocHeader* getPrev() const;
    AllocHeader* getPrev();

    // リンクリストに追加
    void addLink(AllocHeader* pAllocHeader);

    // リンクリストから切り離す
    void deleteLink(AllocHeader*& pAllocHeader);

    // 情報書き込み
    void record(
        void* address,
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function,
        wfl::size_t backTraceHash,
        wfl::size_t bookmark,
        Heap* pHeap);

    bool isValidSignature() const;

private:
    static wfl::ptrdiff_t s_HeaderSize; // ヘッダ情報の全体サイズ
    static wfl::array<wfl::ptrdiff_t, HEADER_INFO_BIT_COUNT> s_HeaderInfoOffsets; // ヘッダ情報メンバアドレスへのオフセット値
    static HeaderInfoFlags s_HeaderInfos; // 有効なヘッダ情報
};



} // namespace memory


template<>
struct FlagTraits<memory::HeaderInfoFlagBits>
{
    enum { allFlags = wfl::uint64_t(memory::HeaderInfoFlagBits::All) };
};


} // namespace waffle

