#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string &title, const std::vector<std::string> &artists,
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth)
{

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load()
{
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
              << "\" at " << sample_rate << "Hz/" << bit_depth << "bit (uncompressed)\n";
    long long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout << "  -> Estimated file size:" << size << " bytes";
    std::cout << "  -> Fast loading due to uncompressed format.";
}

void WAVTrack::analyze_beatgrid()
{
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
    double estimated_beats = (duration_seconds / 60.0) * bpm; // maybe change to long long type
    double precision_factor = 1;
    std::cout << "  -> Estimated beats: " << estimated_beats << "  -> Precision factpr: 1 (uncompresssed audion)";
}

double WAVTrack::get_quality_score() const
{
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double base_score = 70;
    if (sample_rate >= 44100)
    {
        base_score = base_score + 10;
    }
    if (sample_rate >= 96000)
    {
        base_score = base_score + 5;
    }
    if (bit_depth >= 16)
    {
        base_score = base_score + 10;
    }
    if (bit_depth >= 24)
    {
        base_score = base_score + 5;
    }
    if (base_score > 100.0)
    {
        return 100.0;
    }
    return base_score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const
{
    WAVTrack *clone_track = new WAVTrack(title, artists, duration_seconds, bpm, sample_rate, bit_depth);
    delete[] clone_track->waveform_data;
    // deep copying waveform array:
    clone_track->waveform_size = waveform_size;
    clone_track->waveform_data = new double[waveform_size];
    for (int i = 0; i < waveform_size; i++)
    {
        clone_track->waveform_data[i] = waveform_data[i];
    }
    return PointerWrapper<AudioTrack>(clone_track);
}