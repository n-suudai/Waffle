#pragma once


#include "common/utility/flags.h"
#include <array>


namespace Engine {
namespace Memory {


enum class HeaderInfoIndex : EUInt16
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
      MemoryBlock   = (1 << static_cast<EUInt16>(HeaderInfoIndex::MemoryBlock))   // アドレス
    , MemoryBytes   = (1 << static_cast<EUInt16>(HeaderInfoIndex::MemoryBytes))   // 確保サイズ
    , FileName      = (1 << static_cast<EUInt16>(HeaderInfoIndex::FileName))      // ファイル名
    , Line          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Line))          // 行数
    , FunctionName  = (1 << static_cast<EUInt16>(HeaderInfoIndex::FunctionName))  // 関数名
    , DateTime      = (1 << static_cast<EUInt16>(HeaderInfoIndex::DateTime))      // 確保日時
    , BackTraceHash = (1 << static_cast<EUInt16>(HeaderInfoIndex::BackTraceHash)) // バックトレースのハッシュ値
    , Signature     = (1 << static_cast<EUInt16>(HeaderInfoIndex::Signature))     // メモリ破壊検出用シグネチャ
    , Bookmark      = (1 << static_cast<EUInt16>(HeaderInfoIndex::Bookmark))      // ブックマーク
    , Heap          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Heap))          // 親ヒープ領域
    , Next          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Next))          // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    , Prev          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Prev))          // 前のヘッダへのポインタ (ヒープをウォークするのに必要)

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
class AllocHeader
{
public:
    typedef EUInt32 Signature;

    // シグネチャ定義
    static constexpr Signature SIGNATURE = 0xCDCDCDCD;

    // ヘッダ情報の最大メンバ数
    static constexpr ESize HEADER_INFO_BIT_COUNT = sizeof(HeaderInfoFlagBits) * 8;

public:
    // どのヘッダ情報を保持するかを渡して初期化
    static EBool Initialize(HeaderInfoFlags headerInfos = HeaderInfoFlagBits::Required);

    // 終了
    static EVoid Terminate();

    // 有効化されているか
    static EBool IsEnabled(HeaderInfoFlags headerInfos);

    // オフセット取得
    static EPtrDiff GetHeaderOffset(HeaderInfoIndex headerInfoIndex);

    // ヘッダ情報の全体サイズを取得
    static ESize GetHeaderSize();

    // アドレス
    const EVoid* GetBlock() const;
    EVoid* GetBlock();

    // 確保サイズ
    ESize GetBytes() const;

    // ファイル名
    const EChar* GetFileName() const;

    // 行数
    EUInt32 GetLine() const;

    // 関数名
    const EChar* GetFunctionName() const;

    // 確保日時
    time_t GetDateTime() const;

    // バックトレースのハッシュ値
    ESize GetBackTraceHash() const;

    // メモリ破壊検出用シグネチャ
    Signature GetSignature() const;

    // ブックマーク
    ESize GetBookmark() const;

    // 親ヒープ領域
    const Heap* GetHeap() const;
    Heap* GetHeap();

    // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    const AllocHeader* GetNext() const;
    AllocHeader* GetNext();

    // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
    const AllocHeader* GetPrev() const;
    AllocHeader* GetPrev();

private:
    static EPtrDiff s_HeaderSize; // ヘッダ情報の全体サイズ
    static EArray<EPtrDiff, HEADER_INFO_BIT_COUNT> s_HeaderInfoOffsets; // ヘッダ情報メンバアドレスへのオフセット値
    static HeaderInfoFlags s_HeaderInfos; // 有効なヘッダ情報
};



} // namespace Memory
} // namespace Engine

