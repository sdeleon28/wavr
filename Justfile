config:
  cmake -Bbuild -H"."

build:
  cmake --build build/ --target wavr

run:
  # Read input.wav and write reconstructed WAV to out.wav
  ./build/wavr