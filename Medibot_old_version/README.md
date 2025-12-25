This folder contains the initial, fully functional version of the Medibot firmware. It is compilable and runs on the hardware, but represents an earlier testing approach.

## File: `MEDIBOT__initial_approach.ino`

*   **Status:** Functional / Compilable.
*   **Behavior:**
    *   This version executes a measurement cycle that proceeds even if the current phase was not completed.
    *   Uses a simpler algorithm for SpO2
*   **Why we upgraded:**
    *   The blocking nature of this code meant the system was unresponsive during measurements.
    *   The newer **State Machine** approach (in the main folder) allows a better user interface.
    *   The main version also features improved accuracy for sensor readings.
