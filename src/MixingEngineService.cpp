#include "MixingEngineService.h"
#include <iostream>
#include <memory>

/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // initializing both decks to nullptr
    decks[0] = nullptr;
    decks[1] = nullptr;
    auto_sync = false; // setting auto_sync to false
    bpm_tolerance = 0;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService()
{
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    for (int i = 0; i < 2; i++) // deleting decks and then setting pointers to nullptr
    {
        if (decks[i] != nullptr)
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}

// Implementing Move operator as part of rule of 5

MixingEngineService::MixingEngineService(MixingEngineService &&other) noexcept
    : active_deck(other.active_deck), auto_sync(other.auto_sync), bpm_tolerance(other.bpm_tolerance)
{
    // first, we steal the pointers from the original deck
    decks[0] = other.decks[0];
    decks[1] = other.decks[1];
    // then deleting the original to avoid double deletion:
    other.decks[0] = nullptr;
    other.decks[1] = nullptr;
}

// Implementing Move assignment operator as part of rule of 5

MixingEngineService &MixingEngineService::operator=(MixingEngineService &&other) noexcept
{
    // check for self assignment
    if (this != &other)
    {
        // first we clean the object parameters
        for (int i = 0; i < 2; i++)
        {
            if (decks[i] != nullptr)
            {
                delete decks[i];
            }
        }
        // then, we steal from other
        decks[0] = other.decks[0];
        decks[1] = other.decks[1];
        active_deck = other.active_deck;
        auto_sync = other.auto_sync;
        bpm_tolerance = other.bpm_tolerance;
        // then deleting the original to avoid double deletion:
        other.decks[0] = nullptr;
        other.decks[1] = nullptr;
    }
    return *this;
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
{
    std::cout << "\n=== Loading Track to Deck ===\n";
    bool is_first_track = false;
    PointerWrapper<AudioTrack> cloned_track = track.clone(); // cloning track polymorphically using and wrap in PointerWrapper for safety

    if (cloned_track.get() == nullptr) // if clone fails - logging an error and returning 1
    {
        std::cout << "[ERROR] Track : " << track.get_title() << "failed to clone\n";
        return -1;
    }

    size_t target_deck = 1 - active_deck; // calculating 1- active deck to the one that is not currently active

    if (decks[0] == nullptr && decks[1] == nullptr)
    {
        target_deck = 0;
        is_first_track = true;
    }
    // logging deck switch
    std::cout << "[Deck Switch] Target deck: " << target_deck << "\n";

    if (decks[target_deck] != nullptr) // if decks[target] is not nullptr - delete the track and setting pointer to nullptr
    {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    // performing loading simulation and beat analysis
    cloned_track.get()->load();
    cloned_track.get()->analyze_beatgrid();
    // checking if an active deck exists and auto_sync is enabled
    if (decks[active_deck] && auto_sync)
    {
        if (!can_mix_tracks(cloned_track)) // if bpm difference exceeds tolerance -> sync bpm on cloned track
        {
            sync_bpm(cloned_track);
        }
    }

    AudioTrack *raw_pointer = cloned_track.release(); // releasing  pointer from PointerWrapper and assigning to deck[target]
    decks[target_deck] = raw_pointer;
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << target_deck << "\n";

    if (is_first_track == false) // if its not the first track ->  log unload and delete track from decks[active deck]
    {
        std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")" << "\n";
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    // switch active decks and log messege
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
    if (decks[active_deck] == nullptr) // verifying decks[active_deck] is not nullptr
    {
        return false;
    }
    if (track.get() == nullptr) // verifying input track wrapper contains valid pointer
    {
        return false;
    }
    return std::abs((track.get()->get_bpm()) - (decks[active_deck]->get_bpm())) <= bpm_tolerance; // calculating absolute bpm diff and returning true if diff is less or equal to bpm tolerance
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack> &track) const
{
    if (decks[active_deck] != nullptr && track.get() != nullptr) // verifying both active deck and new track are valid
    {
        int original_BPM = track->get_bpm();                              // getting original bpm from the new track for logging
        int avg_bpm = (original_BPM + decks[active_deck]->get_bpm()) / 2; // calculating average bpm and update new tracks bpm
        track->set_bpm(avg_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << original_BPM << " to " << avg_bpm << "\n"; // loging sync
    }
}
