#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string &track_id) const
{
    return findSlot(track_id) != max_size;
}

AudioTrack *LRUCache::get(const std::string &track_id)
{
    size_t idx = findSlot(track_id);
    if (idx == max_size)
        return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track)
{
    if (track.get() == nullptr)
    {
        return false;
    }
    size_t curr_slot = findSlot(track.get()->get_title());

    if (curr_slot != max_size)
    {
        slots[curr_slot].access(++access_counter);
        return false;
    }
    PointerWrapper<AudioTrack> new_track = track.get()->clone();                // Maybe we are doing double clone, which is unesseccery due to clone in DJControllerServiece 
    if (findEmptySlot() == max_size)
    {
        evictLRU();
        size_t empt = findEmptySlot();
        slots[empt].store(std::move(new_track), ++access_counter);
        return true;
    }
    else{
        size_t empt = findEmptySlot();
        slots[empt].store(std::move(new_track),++access_counter);
    }
    return false;
    }

    bool LRUCache::evictLRU()
    {
        size_t lru = findLRUSlot();
        if (lru == max_size || !slots[lru].isOccupied())
            return false;
        slots[lru].clear();
        return true;
    }

    size_t LRUCache::size() const
    {
        size_t count = 0;
        for (const auto &slot : slots)
            if (slot.isOccupied())
                ++count;
        return count;
    }

    void LRUCache::clear()
    {
        for (auto &slot : slots)
        {
            slot.clear();
        }
    }

    void LRUCache::displayStatus() const
    {
        std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
        for (size_t i = 0; i < max_size; ++i)
        {
            if (slots[i].isOccupied())
            {
                std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                          << " (last access: " << slots[i].getLastAccessTime() << ")\n";
            }
            else
            {
                std::cout << "  Slot " << i << ": [EMPTY]\n";
            }
        }
    }

    size_t LRUCache::findSlot(const std::string &track_id) const
    {
        for (size_t i = 0; i < max_size; ++i)
        {
            if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id)
                return i;
        }
        return max_size;
    }

    /**
     * TODO: Implement the findLRUSlot() method for LRUCache
     */
    size_t LRUCache::findLRUSlot() const
    {
        uint64_t min_val = UINT64_MAX; // Defining flag value in case all cells are empty.
        size_t ret_index = max_size;
        for (size_t i = 0; i < max_size; ++i)
        {
            if (slots[i].isOccupied())
            {
                if (min_val > slots[i].getLastAccessTime())
                {
                    min_val = slots[i].getLastAccessTime();
                    ret_index = i;
                }
            }
        }
        return ret_index;
    }

    size_t LRUCache::findEmptySlot() const
    {
        for (size_t i = 0; i < max_size; ++i)
        {
            if (!slots[i].isOccupied())
                return i;
        }
        return max_size;
    }

    void LRUCache::set_capacity(size_t capacity)
    {
        if (max_size == capacity)
            return;
        // udpate max size
        max_size = capacity;
        // update the slots vector
        slots.resize(capacity);
    }