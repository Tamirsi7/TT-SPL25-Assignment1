#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string &title, const std::vector<std::string> &artists,
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags)
{

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load()
{
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    // TODO: Implement MP3 loading with format-specific operations
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    if (has_id3_tags)
    {
        std::cout << "  → Processing ID3 metadata (artist info, album art, etc.)...\n";
    }
    else
    {
        std::cout << "  → No ID3 tags found.\n";
    }
    std::cout << "  → Decoding MP3 frames...\n";
    std::cout << "  → Load complete.\n";
}

void MP3Track::analyze_beatgrid()
{
    std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement MP3-specific beat detection analysis
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    long long estimated_beats = (duration_seconds / 60.0) * bpm; // maybe change to long long type
    double precision_factor = bitrate / 320.0;
    std::cout << "  → Estimated beats: " << estimated_beats << "  → Compression precision factor: " << precision_factor<< "\n";
}

double MP3Track::get_quality_score() const
{
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything
    double base_score = (bitrate / 320.0) * 100.0;
    if (has_id3_tags)
    {
        base_score = base_score + 5;
    }
    if (bitrate < 128)
    {
        base_score = base_score - 10;
    }
    if (base_score > 100.0)
    {
        return 100.0;
    }
    else if (base_score < 0.0)
    {
        return 0.0;
    }
    else
    {
        return base_score;
    }
}

PointerWrapper<AudioTrack> MP3Track::clone() const
{
 /*   MP3Track *clone_track = new MP3Track(title, artists, duration_seconds, bpm, bitrate, has_id3_tags);
    delete[] clone_track->waveform_data;
    // deep copying waveform array:
    clone_track->waveform_size = waveform_size;
    clone_track->waveform_data = new double[waveform_size];
    for (size_t i = 0; i < waveform_size; i++)
    {
        clone_track->waveform_data[i] = waveform_data[i];
    }
        */
    return PointerWrapper<AudioTrack>(new MP3Track(*this));
}