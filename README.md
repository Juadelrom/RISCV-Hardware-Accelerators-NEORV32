# RISCV-Hardware-Accelerators-NEORV32
HW/SW co-design methodology on RISC-V (NEORV32) for algorithm acceleration. Using AES-128 as a case study, it explores three integration levels: pure C software, RISC-V ISA extensions (Zkne/Zknd), and dedicated hardware accelerators via CFS. Includes a custom RTL microarchitectural optimization for InvCipher.

**[Read the Full B.Sc. Thesis (PDF) / Lee la Memoria Completa del TFG](TFG_JuanManuel_Delgado_Romero.pdf)**

This repository contains the complete source code (Hardware and Software) developed for my Bachelor's Thesis (TFG) in Electronics Engineering at the **University of Seville**.


Using the **AES-128** cryptographic standard as a case study, the algorithm's implementation is approached from three different perspectives or levels. This allows the designer to choose the optimal solution based on application requirements (performance, latency, and area overhead):

1. **Software Execution (Pure C):** General-purpose implementation using only the base ISA, ideal for control applications without critical timing constraints.
2. **Acceleration via ISA Extensions:** Use of coprocessors integrated into the CPU *pipeline*, invoked through specific RISC-V cryptographic instructions (`Zkne` and `Zknd` extensions).
3. **Dedicated Hardware Acceleration (CFS):** Design of a memory-mapped cryptographic core (DSA) via the *Custom Functions Subsystem* (CFS), achieving minimal latency and enabling *on-the-fly* subkey calculation in parallel with the CPU.

### Microarchitectural Contribution
Beyond the algorithmic implementation, this project demonstrates the flexibility of the RISC-V ecosystem compared to proprietary ISAs through a **direct intervention in the processor's microarchitecture**. 

The RTL code of the cryptographic coprocessor was modified to alter the instruction decoder for `aes32dsmi` (creating a hardware *bypass* in the `InvSBox` operation). This allowed the isolated application of the `InvMixColumns` transformation to the round subkeys, reducing clock cycles during decryption by 867 cycles (1.20x), with almost zero area overhead (only a 3.57% increase in LUTs).


## Case Studies and Repository Structure
The code is organized by chapters according to the BSc Thesis document:

* **Cap4 (ISA Extensions):** Tests for the M extension (Multiplication) and Zknd/Zkne cryptographic extensions using *inline assembly*.
* **Cap5 (CFS Accelerators):** Implementation of combinational and sequential accelerators (32-bit Divider) integrated externally to the *pipeline* using a *Handshaking* protocol.
* **Cap6 (AES-128):** Implementation of the three described methodologies (Pure C, ISA Zkne/Zknd in various versions, and the hardware accelerator via CFS). Includes the RTL code for the microarchitectural contribution.
* **Cap7 (ADC Visualizer):** Design of a system decoupled from the memory bus for data acquisition from the XADC IP block (Temperature/Voltage) and multiplexed visualization.
* **Cap8 (Demonstrator):** Interactive UART-based system that evaluates accelerator performance and tests the physical HW/SW isolation through forced injection of critical CPU errors.

*Each subfolder contains its own detailed `README.md` and maintains a logical separation between domains: `\hw` (`.vhd`, `.xdc`, `.bit`, etc.) and `\sw` (C, Makefiles).*

## Technologies Used
* **Architecture:** RISC-V (RV32IM_Zkne_Zknd)
* **SoC:** [NEORV32](https://github.com/stnolting/neorv32) (v1.11.2)
* **Hardware Platform:** Digilent Nexys 4 DDR (Artix-7 FPGA)
* **Tools:** GCC Toolchain (C bare-metal), Xilinx Vivado 2024.2 (VHDL), GHDL.

## Author
**Juan Manuel Delgado Romero**
* B.Sc. in Electronics Engineering
* Escuela Politécnica Superior - Universidad of Seville (2026)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
*(Note: The base NEORV32 core and derivative fragments are governed by their original BSD 3-Clause license).*
