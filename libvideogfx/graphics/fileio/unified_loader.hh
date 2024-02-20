/*********************************************************************
  libvideogfx/graphics/fileio/unified_loader.hh

  purpose:
    Makes the loading of files much easier by providing a unified
    interface to loading any kind of images and sequences.
    The input can even be modified by a set of filters like
    deinterlacing or cropping. A further nice feature is to
    save parts of the input-definition pipeline as a macro,
    so that sequences can be accessed very easily.

  usage:
    =macro -- macro expansion (define in ~/.libvideogfxrc)
    range:s:l -- only show 'l' pictures, starting from 's'
    length:l -- sequence length is 'l'
    start:s -- first frame at 's'
    decimate:f -- only show every f'th frame (f=3 -> 2,5,8,...)
    resize:w:h -- resize image
    crop:l:r:t:b -- crop away image borders
    quarter -- resize to quarter size (especially useful to deinterlace to CIF)
    alpha:n -- add alpha-channel file 'n' to the loaded images
    cache -- add a temporary disk-based image cache to allow searching
    rgb -- generate alternating R,G,B images

  author(