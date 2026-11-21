# ENGR 2304 — P09 Project Icarus (Simulation)

**Student:** Justin Nwajei  
**Course:** ENGR 2304, Section 3 — Fall 2026  
**Assignment:** P09 — GitHub Repository & Shared Coding

This Arduino Uno/Tinkercad project accepts a desired pressure setting, simulates a compressor regulating pressure, and starts a ten-second countdown after the target pressure is reached. An LED represents the compressor; a second LED represents a *simulated* launch event. **No real launch, ignition, valve, or compressor hardware is controlled.**

## Hardware configuration (Tinkercad)

| Part | Arduino connection | Purpose |
|---|---|---|
| Potentiometer (10 kΩ typical) | Outer terminals to **5V** and **GND**; center wiper to **A0** | Simulated pressure sensor, scaled 0–120 PSI |
| Compressor LED + 220 Ω resistor | **D8 → resistor → LED anode**, LED cathode to **GND** | Lit while compressor is needed |
| Countdown-complete LED + 220 Ω resistor | **D9 → resistor → LED anode**, LED cathode to **GND** | Lit for two seconds at simulated completion |
| Push button | **D7 → button → GND** | Abort; uses built-in `INPUT_PULLUP` |
| Serial Monitor | **9600 baud** and **Newline** | Enter desired pressure and view status |

## How to run

1. Open **Tinkercad Circuits**, create an Arduino Uno circuit, and wire the components as above.
2. Switch **Code** to **Text**. Paste `2026FA-ENGR-2304-3-P09-NWAJEI-JUSTIN.ino`.
3. Start simulation, open the Serial Monitor, and select **9600 baud** and a newline terminator.
4. Enter a target of **10 to 100 PSI**. **Unprefixed values are binary**; `0x` indicates hexadecimal.
5. Turn the potentiometer to increase the simulated pressure. D8 turns off when the target is reached; the ten-second countdown starts. At zero, D9 lights briefly.
6. Press **ABORT** to stop, or reduce pressure sufficiently during countdown to test the pressure-loss abort. If pressure is not reached within 60 seconds, the simulation times out.

### Valid input examples

| Serial input | Interpreted as | Target pressure |
|---|---|---|
| `1010000` | binary | 80 PSI |
| `0x50` | hexadecimal | 80 PSI |
| `101010` | binary | 42 PSI |
| `0x2A` | hexadecimal | 42 PSI |
| `80` | **invalid**: neither binary digits nor prefixed hex | rejected |

## Collaboration (P09 requirement)

Two **actual classmates** must each propose code edits in a separate pull request. The student reviews and merges both requests into the `main` branch. Use `COLLABORATION.md` to organize the invitations and collect genuine PR links. Do not claim a contribution until the relevant merge is visible in GitHub history.

## Submission status

- Arduino sketch and README prepared.
- Tinkercad circuit/link and actual simulation evidence: https://www.tinkercad.com/things/l3Ska7caBnv/editel?returnTo=%2Fdashboard&sharecode=OIwwjcHWe0a9nmf_j0GfsUJ_hnDedw9o-PeQjFYcYCw
- Classmate invitations, two proposed revisions, and merges: **to be completed**.

**Repository:** https://github.com/jnwajei22/2026FA-ENGR-2304-3-NWAJEI-JUSTIN
