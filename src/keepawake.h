#pragma once

// Keeps Windows awake while an AirPlay client is connected.
//
// The video window is a plain GStreamer window, so Windows does not know that
// a stream is playing. On battery the PC can then go to sleep (or Modern
// Standby) in the middle of a mirroring session, which drops Wi-Fi and cuts the
// client off.
namespace keepawake
{
// True while this process holds an established TCP connection with a
// non-loopback peer. uxplay-windows only accepts connections from AirPlay
// clients, so this means "a client is connected".
bool hasClientConnection();

// Asks Windows to keep the system and the display on (true), or lets them
// sleep again (false). The request belongs to the calling thread, so call it
// from the GUI thread only.
void set(bool on);
} // namespace keepawake
