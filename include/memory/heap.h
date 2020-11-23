#pragma once


#include "common/types.h"
#include <mutex>


namespace waffle {
namespace memory {

class AllocHeader;
class IMemoryLeakReporter;
class IHeapTreeStatsReporter;
class IMemoryAssertionReporter;


class Heap final
{
public:
    Heap();

    ~Heap();

    void initialize();

    void activate(const char* name);

    void deactivate();

    const char* getName() const;

    bool isActive() const;

    template <typename Policy>
    inline void* allocate(
        wfl::size_t bytes,
        const char* file,
        wfl::int32_t line,
        const char* function);

    template <typename Policy>
    inline void* allocateAligned(
        wfl::size_t bytes,
        wfl::size_t alignment,
        const char* file,
        wfl::int32_t line,
        const char* function);

    template <typename Policy>
    inline void deallocate(void* pBlock);

    template <typename Policy>
    inline void deallocateAligned(void* pBlock);

    // �����N���X�g���\�z
    void addAllocation(AllocHeader* pAllocation);

    // �����N���X�g����؂藣��
    void eraseAllocation(AllocHeader* pAllocation);

    // �e�q�֌W�̍\�z������֐�
    void attachTo(Heap* pParent);

    // ���[�N�̃`�F�b�N�֐�
    void memoryLeakCheck(
        IMemoryLeakReporter* pReporter,
        wfl::size_t bookmarkStart,
        wfl::size_t bookmarkEnd) const;

    // �����W�̂��߂̊֐�
    void reportTreeStats(
        IHeapTreeStatsReporter* pAccumulator,
        wfl::int32_t depth) const;

    // �������j��̃`�F�b�N�֐�
    void memoryAssertionCheck(
        IMemoryAssertionReporter* pReporter,
        wfl::size_t bookmarkStart,
        wfl::size_t bookmarkEnd) const;

protected:
    void getTreeStats(
        wfl::size_t& totalBytes,
        wfl::size_t& totalPeakBytes,
        wfl::size_t& totalInstanceCount) const;

private:
    wfl::recursive_mutex m_protection;

    enum
    {
        NAMELENGTH = 128
    };
    char m_name[NAMELENGTH];

    wfl::size_t m_totalAllocatedBytes;
    wfl::size_t m_peakAllocatedBytes;
    wfl::size_t m_allocatedInstanceCount;
    AllocHeader* m_pAllocHeader; // �����N���X�g

    Heap* m_pParent;
    Heap* m_pFirstChild;
    Heap* m_pNextSibling;
    Heap* m_pPrevSibling;

    bool m_isActive;
};


template <typename Policy>
inline void* Heap::allocate(
    wfl::size_t bytes,
    const char* file,
    wfl::int32_t line,
    const char* function)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    // �V�O�l�`���T�C�Y���v���X
    constexpr size_t signatureSize = sizeof(AllocationSignature);

    // �|���V�[�𗘗p���ă��������m��
    void* pBlock = Policy::allocate(bytes + signatureSize);

    // �g���b�J�[�֏���o�^
    MemoryTracker::get().recordAllocation(
        pBlock,
        bytes,
        file,
        line,
        function,
        this);

    return pBlock;
}

template <typename Policy>
inline void* Heap::allocateAligned(
    wfl::size_t bytes,
    wfl::size_t alignment,
    const char* file,
    wfl::int32_t line,
    const char* function)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    // �V�O�l�`���T�C�Y���v���X
    constexpr size_t signatureSize = sizeof(AllocationSignature);

    // �|���V�[�𗘗p���ă��������m��
    void* pBlock = Policy::allocateAligned(bytes + signatureSize);

    // �g���b�J�[�֏���o�^
    MemoryTracker::get().recordAllocation(
        pBlock,
        bytes,
        file,
        line,
        function,
        this);

    return pBlock;
}

template <typename Policy>
inline void Heap::deallocate(void* pBlock)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    // �g���b�J�[��������폜
    MemoryTracker::get().recordDeallocation(pBlock, this);

    // �|���V�[�𗘗p���ă�������j��
    Policy::deallocate(pBlock);
}

template <typename Policy>
inline void Heap::deallocateAligned(void* pBlock)
{
    std::lock_guard<std::recursive_mutex> lock(m_protection);

    // �g���b�J�[��������폜
    MemoryTracker::get().recordDeallocation(pBlock, this);

    // �|���V�[�𗘗p���ă�������j��
    Policy::deallocateAligned(pBlock);
}


} // namespace memory
} // namespace waffle

