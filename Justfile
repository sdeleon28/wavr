config:
  cmake -Bbuild -H"."

build:
  cmake --build build/ --target wavr

run:
  ./build/wavr > out_samples.txt
  cat out_samples.txt | grep LEFT | cut -d":" -f2 > left.txt
  cat out_samples.txt | grep RIGHT | cut -d":" -f2 > right.txt
  python play.py
