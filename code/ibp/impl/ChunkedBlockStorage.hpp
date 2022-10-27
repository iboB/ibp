// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <type_traits>
#include <cstdint>

namespace ibp::impl
{
// storage composed of a singly linked list of chunks
template <typename T>
class ChunkedBlockStorage
{
    static_assert(std::is_trivial_v<T>);
    static_assert(std::alignment_of_v<T> <= std::alignment_of_v<std::max_align_t>);
    struct Chunk
    {
        Chunk* next;
        T* buf() { return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(this) + sizeof(Chunk)); }
    };
    static_assert(sizeof(Chunk) % std::alignment_of_v<std::max_align_t> == 0);
    Chunk* m_front = nullptr;
    Chunk* m_back = nullptr;
    T* m_end = nullptr; // in last chunk
    T* m_capEnd = nullptr; // capacity end in last chunk
    uint32_t m_chunkSize; // number of T elements in a chunk (capacity)

    Chunk* newChunk()
    {
        auto ret = reinterpret_cast<Chunk*>(new uint8_t[sizeof(Chunk) + m_chunkSize * sizeof(T)]);
        ret->next = nullptr;
        return ret;
    }

    void resetEndFromBack()
    {
        m_end = m_back->buf();
        m_capEnd = m_end + m_chunkSize;
    }

    void free()
    {
        Chunk* next = m_front;
        while (next)
        {
            auto cur = reinterpret_cast<uint8_t*>(next);
            next = next->next;
            delete[] cur;
        }
    }

    void take(ChunkedBlockStorage& other)
    {
        m_front = other.m_front;
        other.m_front = nullptr;
        m_back = other.m_back;
        other.m_back = nullptr;
        m_end = other.m_end;
        other.m_end = nullptr;
        m_capEnd = other.m_capEnd;
        other.m_capEnd = nullptr;
        m_chunkSize = other.m_chunkSize;
    }
public:
    explicit ChunkedBlockStorage(uint32_t chunkSize = 128)
        : m_chunkSize(chunkSize)
    {
    }

    void init()
    {
        m_front = m_back = newChunk();
        resetEndFromBack();
    }

    bool valid() const
    {
        return !!m_front;
    }

    bool empty()
    {
        return !m_front || m_end == m_front->buf();
    }

    ~ChunkedBlockStorage()
    {
        free();
    }

    ChunkedBlockStorage(const ChunkedBlockStorage&) = delete;
    ChunkedBlockStorage& operator=(const ChunkedBlockStorage&) = delete;

    ChunkedBlockStorage(ChunkedBlockStorage&& other) noexcept
    {
        take(other);
    }
    ChunkedBlockStorage& operator=(ChunkedBlockStorage&& other) noexcept
    {
        free();
        take(other);
        return *this;
    }

    T& emplace_back()
    {
        if (m_end == m_capEnd)
        {
            if (m_back->next)
            {
                m_back = m_back->next;
            }
            else
            {
                auto nc = newChunk();
                m_back->next = nc;
                m_back = nc;
            }
            resetEndFromBack();
        }
        return *m_end++;
    }

    void reset()
    {
        m_back = m_front;
        resetEndFromBack();
    }

    class ConstIterator
    {
    public:
        T* p;
        uint32_t csize;
        Chunk* c;

        ConstIterator& operator++()
        {
            ++p;
            if (c && p == c->buf() + csize)
            {
                c = c->next;
                if (c)
                {
                    p = c->buf();
                }
            }
            return *this;
        }
        const T& operator*() const { return *p; }
        const T* operator->() const { return p; }

        bool operator==(const ConstIterator& other) const
        {
            return p == other.p;
        }
        bool operator!=(const ConstIterator& other) const
        {
            return p != other.p;
        }
    };

    ConstIterator begin() const
    {
        T* pbegin = m_front ? m_front->buf() : nullptr;
        return {pbegin, m_chunkSize, m_front};
    }

    ConstIterator end() const
    {
        return {m_end, m_chunkSize, m_back};
    }
};
}
