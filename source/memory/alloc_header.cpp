#include "memory/alloc_header.h"


namespace waffle {
namespace memory {


// �w�b�_���̑S�̃T�C�Y
wfl::ptrdiff_t AllocHeader::s_HeaderSize = 0;

// �w�b�_��񃁃��o�A�h���X�ւ̃I�t�Z�b�g�l
wfl::array<wfl::ptrdiff_t, AllocHeader::HEADER_INFO_BIT_COUNT> AllocHeader::s_HeaderInfoOffsets = { 0 };

// �L���ȃw�b�_���
HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


// �ǂ̃w�b�_����ێ����邩��n���ď�����
bool AllocHeader::initialize(HeaderInfoFlags headerInfos)
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
        s_HeaderSize += sizeof(void*);
    }

    // �m�ۃT�C�Y
    if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBytes)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBytes)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::size_t);
    }

    // �t�@�C����
    if (s_HeaderInfos & HeaderInfoFlagBits::FileName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FileName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const char*);
    }

    // �s��
    if (s_HeaderInfos & HeaderInfoFlagBits::Line)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Line)] = s_HeaderSize;
        s_HeaderSize += sizeof(wfl::uint32_t);
    }

    // �֐���
    if (s_HeaderInfos & HeaderInfoFlagBits::FunctionName)
    {
        s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FunctionName)] = s_HeaderSize;
        s_HeaderSize += sizeof(const char*);
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
        s_HeaderSize += sizeof(wfl::size_t);
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
        s_HeaderSize += sizeof(wfl::size_t);
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
void AllocHeader::terminate()
{
    s_HeaderSize = 0;
    s_HeaderInfoOffsets.fill(0);
    s_HeaderInfos = HeaderInfoFlagBits::Required;
}

// �L��������Ă��邩
bool AllocHeader::isEnabled(HeaderInfoFlags headerInfos)
{
    return (bool)(s_HeaderInfos & headerInfos);
}

// �I�t�Z�b�g�擾
wfl::ptrdiff_t AllocHeader::getHeaderOffset(HeaderInfoIndex headerInfoIndex)
{
    return s_HeaderInfoOffsets[static_cast<int>(headerInfoIndex)];
}

// �w�b�_���̑S�̃T�C�Y���擾
wfl::size_t AllocHeader::getHeaderSize()
{
    return s_HeaderSize;
}

// �A�h���X
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

// �m�ۃT�C�Y
wfl::size_t AllocHeader::getBytes() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::MemoryBytes);
    return *reinterpret_cast<const wfl::size_t*>(ptr);
}

// �t�@�C����
const char* AllocHeader::getFileName() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::FileName);
    return reinterpret_cast<const char*>(ptr);
}

// �s��
wfl::uint32_t AllocHeader::getLine() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Line);
    return *reinterpret_cast<const wfl::uint32_t*>(ptr);
}

// �֐���
const char* AllocHeader::getFunctionName() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::FunctionName);
    return reinterpret_cast<const char*>(ptr);
}

// �m�ۓ���
time_t AllocHeader::getDateTime() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::DateTime);
    return *reinterpret_cast<const time_t*>(ptr);
}

// �o�b�N�g���[�X�̃n�b�V���l
wfl::size_t AllocHeader::getBackTraceHash() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::BackTraceHash);
    return *reinterpret_cast<const wfl::size_t*>(ptr);
}

// �������j�󌟏o�p�V�O�l�`��
AllocHeader::Signature AllocHeader::getSignature() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Signature);
    return *reinterpret_cast<const Signature*>(ptr);
}

// �u�b�N�}�[�N
wfl::size_t AllocHeader::getBookmark() const
{
    const void* ptr = this + getHeaderOffset(HeaderInfoIndex::Bookmark);
    return *reinterpret_cast<const wfl::size_t*>(ptr);
}

// �e�q�[�v�̈�
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

// ���̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
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

// �O�̃w�b�_�ւ̃|�C���^ (�q�[�v���E�H�[�N����̂ɕK�v)
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


} // namespace memory
} // namespace waffle

