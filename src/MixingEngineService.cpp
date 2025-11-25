#include "MixingEngineService.h"
#include <iostream>
#include <memory>

/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    auto_sync = false;
    bpm_tolerance = 0;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService()
{
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    for (int i = 0; i < 2; i++)
    {
        if (decks[i] != nullptr)
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
{
    std::cout << "\n=== Loading Track to Deck === \n";
    bool is_first_track = false;
    PointerWrapper<AudioTrack> cloned_track = track.clone();

    if (cloned_track.get() == nullptr)
    {
        std::cout << "[ERROR] Track : " << track.get_title() << "failed to clone \n";
        return -1;
    }

    size_t target_deck = 1 - active_deck;

    if (decks[0] == nullptr && decks[1] == nullptr)
    {
        target_deck = 0;
        is_first_track = true;
    }

    std::cout << "[Deck Switch] Target deck: " << target_deck << "\n";

    if (decks[target_deck] != nullptr)
    {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    cloned_track.get()->load();
    cloned_track.get()->analyze_beatgrid();

    if (decks[active_deck] && auto_sync)
    {
        if (!can_mix_tracks(cloned_track))
        {
            sync_bpm(cloned_track);
        }
    }

    AudioTrack *raw_pointer = cloned_track.release();
    decks[target_deck] = raw_pointer;
    std::cout << "[Load Complete] " << track.get_title() << " is now loaded on deck " << target_deck << "\n";

    if (is_first_track == false)
    {
        std::cout << "[Unload] Unloading previous deck " << active_deck <<" ("<< decks[active_deck]->get_title() <<")"<< "\n";
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }

    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << "\n";
    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const
{
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i)
    {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 *
 * Check if two tracks can be mixed based on BPM difference.
 *
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack> &track) const
{
    if (decks[active_deck] == nullptr)
    {
        return false;
    }
    if (track.get() == nullptr)
    {
        return false;
    }
    return std::abs((track.get()->get_bpm()) - (decks[active_deck]->get_bpm()) <= bpm_tolerance);
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack> &track) const
{
    if (decks[active_deck] == nullptr || track.get() == nullptr)
    {
        return;
    }
    int original_BPM = track.get()->get_bpm();
    int avg_bpm = (original_BPM + decks[active_deck]->get_bpm())/2;
    track.get()->set_bpm(avg_bpm);

    std::cout << "[Sync BPM] Syncing BPM from " << original_BPM << "to" << avg_bpm << "\n";
}
