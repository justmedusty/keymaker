![the-keymaker-holding-a-key-in-the-matrix-reloaded-3975335721](https://github.com/user-attachments/assets/3e738f58-edef-422e-a65c-27448e543e43)

# How to Build and Use

To build:
git clone https://github.com/justmedusty/keymaker.git or download the source
cd keymaker
cmake .
make
(optional) sudo cp keymaker /bin

./keymaker num_bits quality(high/low)

high will poll /dev/random while low (not really low quality just lowER) will poll /dev/urandom

Keys are output with 0x prefix in hexademical format, meant to go hand in hand with my cypher OTP tool.
