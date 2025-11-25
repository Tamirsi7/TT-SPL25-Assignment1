#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>

DJLibraryService::DJLibraryService(const Playlist &playlist)
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo> &library_tracks)
{
    int counter = 0;
    for (int i = 0; i < library_tracks.size(); i++)
    {
        AudioTrack *new_track = nullptr;
        if (library_tracks[i].type == "MP3")
        {
            new_track = new MP3Track(library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, library_tracks[i].extra_param1, (bool)library_tracks[i].extra_param2);
        }
        if (library_tracks[i].type == "WAV")
        {
            new_track = new WAVTrack(library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);
        }
        if (new_track != nullptr)
        {
            library.push_back(new_track);
            counter++;
        }
    }
    std::cout << "[INFO] Track library built: " << counter << " tracks loaded";
}

// adding distructor
DJLibraryService::~DJLibraryService()
{
    for(int i=0;i<library.size();i++){
        if(library[i]!=nullptr){
            delete library[i];
        }
    }
}

/**
 * @brief Display the current state of the DJ library playlist
 *
 */
void DJLibraryService::displayLibrary() const
{
    std::cout << "=== DJ Library Playlist: "
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty())
    {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 *
 * @return Playlist&
 */
Playlist &DJLibraryService::getPlaylist()
{
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 *
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack *DJLibraryService::findTrack(const std::string &track_title)
{
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string &playlist_name,
                                               const std::vector<int> &track_indices)
{
    std::cout << "[INFO] Loading playlist: " << playlist_name;
    Playlist *new_playlist = new Playlist(playlist_name);
    int count = 0;
    for (int i = 0; i < track_indices.size(); i++)
    {
        if (track_indices[i] <= 0 || track_indices[i] > library.size())
        {
            std::cout << "[WARNING] Invalid track index: " << track_indices[i];
            continue; // skip current index
        }
        AudioTrack *raw_cloned_track = library[track_indices[i] - 1]->clone().release();
        if (raw_cloned_track == nullptr)
        {
            std::cout << "[ERROR] Track: " << library[track_indices[i]]->get_title() << " failed to clone\n";
            continue;
        }
        raw_cloned_track->load();
        raw_cloned_track->analyze_beatgrid();
        new_playlist->add_track(raw_cloned_track);
        count++;

        std::cout << "Added '" << raw_cloned_track->get_title() << "' to playlist '" << new_playlist->get_name() << "'\n";
    }
    std::cout << "[INFO] Playlist loaded: " << new_playlist->get_name() << " (" << count << " tracks)";
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const
{
    std::vector<std::string> tracks_names;
    std::vector<AudioTrack *> tracks_from_playlist = playlist.getTracks();
    for (int i = 0; i < tracks_from_playlist.size(); i++)
    {
        tracks_names.push_back(tracks_from_playlist[i]->get_title());
    }
    return tracks_names;
}
