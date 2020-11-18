#pragma once


#include "common/utility/flags.h"
#include <array>


namespace Engine {
namespace Memory {


enum class HeaderInfoIndex : EUInt16
{
      MemoryBlock       // �A�h���X
    , MemoryBytes       // �m�ۃT�C�Y
    , FileName          // �t�@�C����
    , Line              // �s��
    , FunctionName      // �֐���
    , DateTime          // �m�ۓ���
    , BackTraceHash     // �o�b�N�g���[�X�̃n�b�V���l
    , Signature         // �������j�󌟏o�p�V�O�l�`��
    , Bookmark          // �u�b�N�}�[�N
    , Heap              // �e�q�[�v�̈�
    , Next              // ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    , Prev              // �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    , Num
};

enum class HeaderInfoFlagBits
{
      MemoryBlock   = (1 << static_cast<EUInt16>(HeaderInfoIndex::MemoryBlock))   // �A�h���X
    , MemoryBytes   = (1 << static_cast<EUInt16>(HeaderInfoIndex::MemoryBytes))   // �m�ۃT�C�Y
    , FileName      = (1 << static_cast<EUInt16>(HeaderInfoIndex::FileName))      // �t�@�C����
    , Line          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Line))          // �s��
    , FunctionName  = (1 << static_cast<EUInt16>(HeaderInfoIndex::FunctionName))  // �֐���
    , DateTime      = (1 << static_cast<EUInt16>(HeaderInfoIndex::DateTime))      // �m�ۓ���
    , BackTraceHash = (1 << static_cast<EUInt16>(HeaderInfoIndex::BackTraceHash)) // �o�b�N�g���[�X�̃n�b�V���l
    , Signature     = (1 << static_cast<EUInt16>(HeaderInfoIndex::Signature))     // �������j�󌟏o�p�V�O�l�`��
    , Bookmark      = (1 << static_cast<EUInt16>(HeaderInfoIndex::Bookmark))      // �u�b�N�}�[�N
    , Heap          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Heap))          // �e�q�[�v�̈�
    , Next          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Next))          // ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    , Prev          = (1 << static_cast<EUInt16>(HeaderInfoIndex::Prev))          // �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)

    // �K�{
    , Required = MemoryBlock
        | MemoryBytes
        | Heap
        | Next
        | Prev

    // ���ׂ�
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

// �w�b�_���
class AllocHeader
{
public:
    typedef EUInt32 Signature;

    // �V�O�l�`����`
    static constexpr Signature SIGNATURE = 0xCDCDCDCD;

    // �w�b�_���̍ő僁���o��
    static constexpr ESize HEADER_INFO_BIT_COUNT = sizeof(HeaderInfoFlagBits) * 8;

public:
    // �ǂ̃w�b�_����ێ����邩��n���ď�����
    static EBool Initialize(HeaderInfoFlags headerInfos = HeaderInfoFlagBits::Required);

    // �I��
    static EVoid Terminate();

    // �L��������Ă��邩
    static EBool IsEnabled(HeaderInfoFlags headerInfos);

    // �I�t�Z�b�g�擾
    static EPtrDiff GetHeaderOffset(HeaderInfoIndex headerInfoIndex);

    // �w�b�_���̑S�̃T�C�Y���擾
    static ESize GetHeaderSize();

    // �A�h���X
    const EVoid* GetBlock() const;
    EVoid* GetBlock();

    // �m�ۃT�C�Y
    ESize GetBytes() const;

    // �t�@�C����
    const EChar* GetFileName() const;

    // �s��
    EUInt32 GetLine() const;

    // �֐���
    const EChar* GetFunctionName() const;

    // �m�ۓ���
    time_t GetDateTime() const;

    // �o�b�N�g���[�X�̃n�b�V���l
    ESize GetBackTraceHash() const;

    // �������j�󌟏o�p�V�O�l�`��
    Signature GetSignature() const;

    // �u�b�N�}�[�N
    ESize GetBookmark() const;

    // �e�q�[�v�̈�
    const Heap* GetHeap() const;
    Heap* GetHeap();

    // ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    const AllocHeader* GetNext() const;
    AllocHeader* GetNext();

    // �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    const AllocHeader* GetPrev() const;
    AllocHeader* GetPrev();

private:
    static EPtrDiff s_HeaderSize; // �w�b�_���̑S�̃T�C�Y
    static EArray<EPtrDiff, HEADER_INFO_BIT_COUNT> s_HeaderInfoOffsets; // �w�b�_��񃁃��o�A�h���X�ւ̃I�t�Z�b�g�l
    static HeaderInfoFlags s_HeaderInfos; // �L���ȃw�b�_���
};



} // namespace Memory
} // namespace Engine

