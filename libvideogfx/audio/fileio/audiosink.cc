
#include "libvideogfx/audio/fileio/audiosink.hh"
#include <algorithm>

namespace videogfx {

  void AudioSink::SendSamples(const int8*  samples,int len)
  {
    int16 buf[1000];

    while (len)
      {
	int 