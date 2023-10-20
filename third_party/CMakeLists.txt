

include(ExternalProject)

# Build capnproto

ExternalProject_Add(capnproto
  URL https://github.com/capnproto/capnproto/archive/refs/tags/v1.0.0.tar.gz
  URL_HASH SHA256=bcd44dde78055313a7786cb6ab020cbef19b9045b53857f90cce101c9453f715
)
