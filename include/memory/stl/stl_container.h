#pragma once

#include "memory/stl/stl_allocator.h"
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace waffle {
namespace memory {


// Vector
template <typename T, typename Alloc = STLAllocator<T>>
using Vector = wfl::vector<T, Alloc>;

// List
template <typename T, typename Alloc = STLAllocator<T>>
using List = wfl::list<T, Alloc>;

// ForwardList
template <typename T, typename Alloc = STLAllocator<T>>
using ForwardList = wfl::forward_list<T, Alloc>;

// Map
template <typename Key, typename T, typename Compare = wfl::less<Key>,
    typename Alloc = STLAllocator<wfl::pair<const Key, T>>>
    using Map = wfl::map<Key, T, Compare, Alloc>;

// MultiMap
template <typename Key, typename T, typename Compare = wfl::less<Key>,
    typename Alloc = STLAllocator<wfl::pair<const Key, T>>>
    using MultiMap = wfl::multimap<Key, T, Compare, Alloc>;

// UnorderedMap
template <typename Key, typename T, typename Hash = wfl::hash<Key>,
    typename Pred = wfl::equal_to<Key>,
    typename Alloc = STLAllocator<wfl::pair<const Key, T>>>
    using UnorderedMap = wfl::unordered_map<Key, T, Hash, Pred, Alloc>;

// UnorderedMultiMap
template <typename Key, typename T, typename Hash = wfl::hash<Key>,
    typename Pred = wfl::equal_to<Key>,
    typename Alloc = STLAllocator<wfl::pair<const Key, T>>>
    using UnorderedMultiMap =
    wfl::unordered_multimap<Key, T, Hash, Pred, Alloc>;

// Set
template <typename Key, typename Compare = wfl::less<Key>,
    typename Alloc = STLAllocator<Key>>
    using Set = wfl::set<Key, Compare, Alloc>;

// MultiSet
template <typename Key, typename Compare = wfl::less<Key>,
    typename Alloc = STLAllocator<Key>>
    using MultiSet = wfl::multiset<Key, Compare, Alloc>;

// UnorderedSet
template <typename Value, typename Hash = wfl::hash<Value>,
    typename Pred = wfl::less<Value>,
    typename Alloc = STLAllocator<Value>>
    using UnorderedSet = wfl::unordered_set<Value, Hash, Pred, Alloc>;

// UnorderedMultiSet
template <typename Value, typename Hash = wfl::hash<Value>,
    typename Pred = wfl::less<Value>,
    typename Alloc = STLAllocator<Value>>
    using UnorderedMultiSet = wfl::unordered_multiset<Value, Hash, Pred, Alloc>;

// Deque
template <typename T, typename Alloc = STLAllocator<T>>
using Deque = wfl::deque<T, Alloc>;

// Queue
template <typename T, typename Sequence = Deque<T>>
using Queue = wfl::queue<T, Sequence>;

// Stack
template <typename T, typename Sequence = Deque<T>>
using Stack = wfl::stack<T, Sequence>;


} // namespace memory


// Vector
template <typename T, typename Alloc = memory::STLAllocator<T>>
using Vector = memory::Vector<T, Alloc>;

// List
template <typename T, typename Alloc = memory::STLAllocator<T>>
using List = memory::List<T, Alloc>;

// ForwardList
template <typename T, typename Alloc = memory::STLAllocator<T>>
using ForwardList = memory::ForwardList<T, Alloc>;

// Map
template <typename Key, typename T, typename Compare = wfl::less<Key>,
    typename Alloc = memory::STLAllocator<wfl::pair<const Key, T>>>
    using Map = memory::Map<Key, T, Compare, Alloc>;

// MultiMap
template <typename Key, typename T, typename Compare = wfl::less<Key>,
    typename Alloc = memory::STLAllocator<wfl::pair<const Key, T>>>
    using MultiMap = memory::MultiMap<Key, T, Compare, Alloc>;

// UnorderedMap
template <typename Key, typename T, typename Hash = wfl::hash<Key>,
    typename Pred = wfl::equal_to<Key>,
    typename Alloc = memory::STLAllocator<wfl::pair<const Key, T>>>
    using UnorderedMap = memory::UnorderedMap<Key, T, Hash, Pred, Alloc>;

// UnorderedMultiMap
template <typename Key, typename T, typename Hash = wfl::hash<Key>,
    typename Pred = wfl::equal_to<Key>,
    typename Alloc = memory::STLAllocator<wfl::pair<const Key, T>>>
    using UnorderedMultiMap =
    memory::UnorderedMultiMap<Key, T, Hash, Pred, Alloc>;

// Set
template <typename Key, typename Compare = wfl::less<Key>,
    typename Alloc = memory::STLAllocator<Key>>
    using Set = memory::Set<Key, Compare, Alloc>;

// MultiSet
template <typename Key, typename Compare = wfl::less<Key>,
    typename Alloc = memory::STLAllocator<Key>>
    using MultiSet = memory::MultiSet<Key, Compare, Alloc>;

// UnorderedSet
template <typename Value, typename Hash = wfl::hash<Value>,
    typename Pred = wfl::less<Value>,
    typename Alloc = memory::STLAllocator<Value>>
    using UnorderedSet = memory::UnorderedSet<Value, Hash, Pred, Alloc>;

// UnorderedMultiSet
template <typename Value, typename Hash = wfl::hash<Value>,
    typename Pred = wfl::less<Value>,
    typename Alloc = memory::STLAllocator<Value>>
    using UnorderedMultiSet = memory::UnorderedMultiSet<Value, Hash, Pred, Alloc>;

// Deque
template <typename T, typename Alloc = memory::STLAllocator<T>>
using Deque = memory::Deque<T, Alloc>;

// Queue
template <typename T, typename Sequence = Deque<T>>
using Queue = memory::Queue<T, Sequence>;

// Stack
template <typename T, typename Sequence = Deque<T>>
using Stack = memory::Stack<T, Sequence>;


} // namespace waffle

