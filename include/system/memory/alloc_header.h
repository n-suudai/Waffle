﻿#pragma once


#include "system/common/utility/flags.h"
#include "system/memory/policy/policy.h"
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
    [[nodiscard]] static bool initialize(HeaderInfoFlags headerInfos = HeaderInfoFlagBits::Required);

    static void finalize();

    [[nodiscard]] static bool isInitialized();

    [[nodiscard]] static bool isEnabled(HeaderInfoFlags headerInfos);

    [[nodiscard]] static wfl::ptrdiff_t getHeaderOffset(HeaderInfoIndex headerInfoIndex);

    [[nodiscard]] static wfl::size_t getHeaderSize();

public:
    [[nodiscard]] const void* getBlock() const;
    [[nodiscard]] void* getBlock();

    [[nodiscard]] wfl::size_t getBytes() const;

    [[nodiscard]] const char* getFileName() const;

    [[nodiscard]] wfl::int32_t getLine() const;

    [[nodiscard]] const char* getFunctionName() const;

    [[nodiscard]] time_t getDateTime() const;

    [[nodiscard]] wfl::size_t getBackTraceHash() const;

    [[nodiscard]] Signature getSignature() const;

    [[nodiscard]] wfl::size_t getBookmark() const;

    [[nodiscard]] const Heap* getHeap() const;
    [[nodiscard]] Heap* getHeap();

    [[nodiscard]] const AllocHeader* getNext() const;
    [[nodiscard]] AllocHeader* getNext();

    [[nodiscard]] const AllocHeader* getPrev() const;
    [[nodiscard]] AllocHeader* getPrev();

    void addLink(AllocHeader* pAllocHeader);

    void deleteLink(AllocHeader*& pAllocHeader);

    void record(
        void* address,
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function,
        wfl::size_t backTraceHash,
        wfl::size_t bookmark,
        Heap* pHeap);

    [[nodiscard]] bool isValidSignature() const;

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

