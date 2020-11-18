#include "core/memory/alloc_header.h"


namespace Engine {
namespace Memory {


// �w�b�_���̑S�̃T�C�Y
EPtrDiff AllocHeader::s_HeaderSize = 0;

// �w�b�_��񃁃��o�A�h���X�ւ̃I�t�Z�b�g�l
EArray<EPtrDiff, AllocHeader::HEADER_INFO_BIT_COUNT> AllocHeader::s_HeaderInfoOffsets = { 0 };

// �L���ȃw�b�_���
HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


// �ǂ̃w�b�_����ێ����邩��n���ď�����
EBool AllocHeader::Initialize(HeaderInfoFlags headerInfos)
{
    if ((headerInfos & HeaderInfoFlagBits::Required) != HeaderInfoFlagBits::Required)
    {
        return false;
    }

    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = headerInfos;

    // �A�h���X
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBlock)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBlock)] = s_HeaderSize;
        s_HeaderSize += sizeof(EVoid*);
    }

    // �m�ۃT�C�Y
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBytes)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBytes)] = s_HeaderSize;
        s_HeaderSize += sizeof(ESize);
    }

    // �t�@�C����
    if (s_HeaderInfos & HeaderInfoFlagBits::FileName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FileName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const EChar*);
    }

    // �s��
    if (s_HeaderInfos & HeaderInfoFlagBits::Line)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Line)] = s_HeaderSize;
        s_HeaderSize += sizeof(EUInt32);
    }

    // �֐���
    if (s_HeaderInfos & HeaderInfoFlagBits::FunctionName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FunctionName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const EChar*);
    }

    // �m�ۓ���
    if (s_HeaderInfos & HeaderInfoFlagBits::DateTime)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::DateTime)] = s_HeaderSize;
        s_HeaderSize += sizeof(time_t);
    }

    // �o�b�N�g���[�X�̃n�b�V���l
    if (s_HeaderInfos & HeaderInfoFlagBits::BackTraceHash)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::BackTraceHash)] = s_HeaderSize;
        s_HeaderSize += sizeof(ESize);
    }

    // �������j�󌟏o�p�V�O�l�`��
    if (s_HeaderInfos & HeaderInfoFlagBits::Signature)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Signature)] = s_HeaderSize;
        s_HeaderSize += sizeof(Signature*);
    }

    // �u�b�N�}�[�N
    if (s_HeaderInfos & HeaderInfoFlagBits::Bookmark)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Bookmark)] = s_HeaderSize;
        s_HeaderSize += sizeof(ESize);
    }

    // �e�q�[�v�̈�
    if (s_HeaderInfos & HeaderInfoFlagBits::Heap)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Heap)] = s_HeaderSize;
        s_HeaderSize += sizeof(Heap*);
    }

    // ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    if (s_HeaderInfos & HeaderInfoFlagBits::Next)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Next)] = s_HeaderSize;
        s_HeaderSize += sizeof(AllocHeader*);
    }

    // �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
    if (s_HeaderInfos & HeaderInfoFlagBits::Prev)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Prev)] = s_HeaderSize;
        s_HeaderSize += sizeof(AllocHeader*);
    }

    return true;
}

// �I��
EVoid AllocHeader::Terminate()
{
    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = HeaderInfoFlagBits::Required;
}

// �L��������Ă��邩
EBool AllocHeader::IsEnabled(HeaderInfoFlags headerInfos)
{
    return (EBool)(s_HeaderInfos & headerInfos);
}

// �I�t�Z�b�g�擾
EPtrDiff AllocHeader::GetHeaderOffset(HeaderInfoIndex headerInfoIndex)
{
    return s_HeaderInfoOffsets[static_cast<int>(headerInfoIndex)];
}

// �w�b�_���̑S�̃T�C�Y���擾
ESize AllocHeader::GetHeaderSize()
{
    return s_HeaderSize;
}

// �A�h���X
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

// �m�ۃT�C�Y
ESize AllocHeader::GetBytes() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBytes);
    return *reinterpret_cast<const ESize*>(ptr);
}

// �t�@�C����
const EChar* AllocHeader::GetFileName() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::FileName);
    return reinterpret_cast<const EChar*>(ptr);
}

// �s��
EUInt32 AllocHeader::GetLine() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Line);
    return *reinterpret_cast<const EUInt32*>(ptr);
}

// �֐���
const EChar* AllocHeader::GetFunctionName() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::FunctionName);
    return reinterpret_cast<const EChar*>(ptr);
}

// �m�ۓ���
time_t AllocHeader::GetDateTime() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::DateTime);
    return *reinterpret_cast<const time_t*>(ptr);
}

// �o�b�N�g���[�X�̃n�b�V���l
ESize AllocHeader::GetBackTraceHash() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::BackTraceHash);
    return *reinterpret_cast<const ESize*>(ptr);
}

// �������j�󌟏o�p�V�O�l�`��
AllocHeader::Signature AllocHeader::GetSignature() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Signature);
    return *reinterpret_cast<const Signature*>(ptr);
}

// �u�b�N�}�[�N
ESize AllocHeader::GetBookmark() const
{
    const EVoid* ptr = this + GetHeaderOffset(HeaderInfoIndex::Bookmark);
    return *reinterpret_cast<const ESize*>(ptr);
}

// �e�q�[�v�̈�
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

// ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
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

// �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
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

