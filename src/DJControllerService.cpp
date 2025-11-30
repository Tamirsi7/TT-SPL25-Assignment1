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
    if (cache.contains(track.get_title())) //if track with same title exists in cache return 1
    {
        cache.get(track.get_title());
        return 1;
    }
    PointerWrapper<AudioTrack> track_clone = track.clone(); //creating a polymorphic clone of the track using and unwrap the PointerWrapper to get the raw pointer
    AudioTrack *raw_pointer = track_clone.release(); 
    if (raw_pointer == nullptr) //if clone is nullptr, logging error and return 0
    {
        std::cout << "[ERROR] Track:" << track.get_title() << "failed to clone\n";
        return 0;
    }
    raw_pointer->load(); //simulating load and beatgrid anaysis
    raw_pointer->analyze_beatgrid();
    PointerWrapper<AudioTrack> wrapped_track(raw_pointer); // legit because we're recieving a number - raw_pointer and not an object
    bool is_put = cache.put(std::move(wrapped_track)); 
    if (is_put)//if put returns true - return 1
    {
        return -1;
    }
    if (!is_put)// else return 0
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
    return cache.get(track_title); //looking up track in cache by title, returning pointer if found , nullptr otherwise
}
