# **Lonely Trader: Pokémon 3rd Gen Spoof Trader for GBA**

A project that enables Pokémon trading between FireRed/LeafGreen and Ruby/Sapphire/Emerald using a single Game Boy Advance (GBA) and an RP2040-based development board (e.g., Raspberry Pi Pico, RP2040 Zero, etc.).
This solution eliminates the need for a second GBA or a second game cartridge, making it an inexpensive and straightforward way to trade your Pokémon.
Note: While this tool supports trade evolutions, mail attached to Pokémon will not be transferred.

## **How It Works**
The RP2040 board acts as a spoofed GBA in a link cable connection, simulating the second console during the trading process. The project uses the link cable's multiplayer mode for communication between the GBA and the RP2040.

I really have no idea what I'm doing when I made this but was able to pull it off despite not knowing anything about micro controllers so don't judge my code. If it works it works. ;)
Don't try this on GBC otherwise you'll toast RP2040.

Specially thanks to Goppier!
