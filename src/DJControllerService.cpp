#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack &track)
{
    if (cache.contains(track.get_title()))
    {
        cache.get(track.get_title());
        return 1;
    }
    PointerWrapper<AudioTrack> track_clone = track.clone();
    AudioTrack *raw_pointer = track_clone.release();
    if (raw_pointer == nullptr)
    {
        std::cout << "[ERROR] Track:" << track.get_title() << "failed to clone\n";
        return 0;
    }
    raw_pointer->load();
    raw_pointer->analyze_beatgrid();
    PointerWrapper<AudioTrack> wrapped_track(raw_pointer); // legit because we're recieving a number - raw_pointer and not an object
    bool is_put = cache.put(std::move(wrapped_track));
    if (is_put)
    {
        return -1;
    }
    if (!is_put)
    {
        return 0;
    }
return 0;
}

void DJControllerService::set_cache_size(size_t new_size)
{
    cache.set_capacity(new_size);
}
// implemented
void DJControllerService::displayCacheStatus() const
{
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack *DJControllerService::getTrackFromCache(const std::string &track_title)
{
    return cache.get(track_title);
}
