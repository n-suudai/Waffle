#pragma once


#include "common/utility/flags.h"
#include <array>


namespace waffle {
namespace memory {


enum class HeaderInfoIndex : wfl::uint16_t
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
      MemoryBlock   = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::MemoryBlock))   // �A�h���X
    , MemoryBytes   = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::MemoryBytes))   // �m�ۃT�C�Y
    , FileName      = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::FileName))      // �t�@�C����
    , Line          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Line))          // �s��
    , FunctionName  = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::FunctionName))  // �֐���
    , DateTime      = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::DateTime))      // �m�ۓ���
    , BackTraceHash = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::BackTraceHash)) // �o�b�N�g���[�X�̃n�b�V���l
    , Signature     = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Signature))     // �������j�󌟏o�p�V�O�l�`��
    , Bookmark      = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Bookmark))      // �u�b�N�}�[�N
    , Heap          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Heap))          // �e�q�[�v�̈�
    , Next          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Next))          // ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    , Prev          = (1 << static_cast<wfl::uint16_t>(HeaderInfoIndex::Prev))          // �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)

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
    typedef wfl::uint32_t Signature;

    // �V�O�l�`����`
    static constexpr Signature SIGNATURE = 0xCDCDCDCD;

    // �w�b�_���̍ő僁���o��
    static constexpr wfl::size_t HEADER_INFO_BIT_COUNT = sizeof(HeaderInfoFlagBits) * 8;

public:
    // �ǂ̃w�b�_����ێ����邩��n���ď�����
    static bool initialize(HeaderInfoFlags headerInfos = HeaderInfoFlagBits::Required);

    // �I��
    static void terminate();

    // �L��������Ă��邩
    static bool isEnabled(HeaderInfoFlags headerInfos);

    // �I�t�Z�b�g�擾
    static wfl::ptrdiff_t getHeaderOffset(HeaderInfoIndex headerInfoIndex);

    // �w�b�_���̑S�̃T�C�Y���擾
    static wfl::size_t getHeaderSize();

    // �A�h���X
    const void* getBlock() const;
    void* getBlock();

    // �m�ۃT�C�Y
    wfl::size_t getBytes() const;

    // �t�@�C����
    const char* getFileName() const;

    // �s��
    wfl::uint32_t getLine() const;

    // �֐���
    const char* getFunctionName() const;

    // �m�ۓ���
    time_t getDateTime() const;

    // �o�b�N�g���[�X�̃n�b�V���l
    wfl::size_t getBackTraceHash() const;

    // �������j�󌟏o�p�V�O�l�`��
    Signature getSignature() const;

    // �u�b�N�}�[�N
    wfl::size_t getBookmark() const;

    // �e�q�[�v�̈�
    const Heap* getHeap() const;
    Heap* getHeap();

    // ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    const AllocHeader* getNext() const;
    AllocHeader* getNext();

    // �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    const AllocHeader* getPrev() const;
    AllocHeader* getPrev();

private:
    static wfl::ptrdiff_t s_HeaderSize; // �w�b�_���̑S�̃T�C�Y
    static wfl::array<wfl::ptrdiff_t, HEADER_INFO_BIT_COUNT> s_HeaderInfoOffsets; // �w�b�_��񃁃��o�A�h���X�ւ̃I�t�Z�b�g�l
    static HeaderInfoFlags s_HeaderInfos; // �L���ȃw�b�_���
};



} // namespace Memory
} // namespace Engine

