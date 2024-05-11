# CO-24_Binary_Nexus
# MOM
### MEMBERS 
-**Ikshitha** , **Preethika**
### May 11, 2024
- **Decisions:** 
  - Implemented SIMD unit.
  - Cleaned the code.

### May 8-10, 2024
- **Decisions:** 
  - Decided to implement TBP (to be phased out), L2 level cache, and SIMD unit.
  - Implemented TBP.
  - Debugged for L2 cache, but couldn't resolve the issue, so reverted back to TBP.
  - Worked on implementing SIMD unit.

### April 7-9, 2024
- **Decisions:** 
  - Debugging and implementation considering memory access for instruction fetch.
  - Encorporated an L2 cache (non-inclusive).
  - Worked on debugging as the number of cycles were wrong.

### March 31, 2024
- **Decisions:** 
  - Set input file for cache configuration.

### March 3-4, 2024
- **Decisions:** 
  - Configured cache class.
  - Implemented cache replacement policies.
  - Incorporated cache into the pipeline.

### February 24, 2024
- **Decisions:** 
  - Made attempts with 'la' (load address) instruction, but it didn't work out.
  - Changed the data type of RAM from `int` to `char` to store `int` as four bytes.
  - Implemented selection sort.
  - Discovered that both cores were using the same memory.

### February 23, 2024
- **Decisions:** 
  - Worked on loading addresses from memory into registers.
  - Implemented `lw` for storing the base address of the array.
  - Implemented bubble sort.

### February 20-21, 2024
- **Decisions:** 
  - Updated preliminary instructions.
  - Implemented `jal`, `jalr`, labels, and branch instructions.

### February 17-18, 2024
- **Decisions:** 
  - Implemented preliminary instructions.

### February 13, 2024
- **Decisions:** 
  - Opted for C++ instead of Python for better performance.

### February 12, 2024
- **Decisions:** 
  - Chose Python as the language for the project.
  - Studied senior projects for reference.
  - Converted the project documentation to Markdown format.




# Multi-Core Simulator

## Phase 1: Initial Development

In this phase, the focus is on setting up the basic structure of the simulator and implementing support for fundamental RISC-V instructions. The two compute units will operate independently without communication.Focuses on basic parsing and output.

### Features Implemented
- Two processors with independent execution capability.
- Shared memory access for both processors.
- Support for basic RISC-V instructions:
  - ADD/SUB
  - BRANCH INSTRUCTIONS
  - UNCONDITIONALL JUMPS
  - LW/SW

### Supported Platforms
Any programming language can be used for developing the simulator.

## Getting Started
1. Clone the repository to your local machine.
2. Execute the simulator using the provided command-line interface.

## Running Example Programs
Included in the repository are assembly programs demonstrating bubble sort and selection sort algorithms for individual processors. Instructions for running these programs are provided in the respective README files.


---

Now, let's continue with Phase 2.
Certainly, here's the revised section with the correct tense:

---

## Phase 2: Pipelining and Instruction Latency

In this phase, the simulator developed in Phase 1 is extended to incorporate pipelining with data forwarding. Additionally, the user is given the option to specify instruction latencies and enable/disable forwarding. The simulator outputs statistics such as the number of stalls and the Instructions Per Cycle (IPC) at the end of execution. Furthermore, a basic branch prediction function is implemented.

### Features Implemented
- Pipelining with data forwarding.
- User-configurable instruction latencies.
- Option to enable/disable forwarding.
- Output of statistics including stalls and IPC.
- Basic branch prediction function (`predict()`)(Static  branch prediction ).

### Instruction Latency Configuration
Users can specify the latency for each arithmetic instruction. The interface for this feature allows users to set custom latencies based on their requirements.

### Branch Prediction
The `predict()` function supports basic branch prediction strategies such as  "always not taken." This function handles mispredictions during simulation.

## Usage
1. Run the simulator with the desired configuration settings.
2. Optionally enable forwarding and specify instruction latencies.
3. Execute programs and observe the output statistics including stalls and IPC.
4. Evaluate the performance of different configurations and branch prediction strategies.

---

To extend the README for Phase 3, we'll include the new features and requirements as follows:

---


## Phase 3: Cache Integration

In this phase, the simulator developed in Phase 2 is extended to incorporate cache functionality. Memory accesses will first search for the address in the cache, fetching data from main memory on a cache miss. This introduces variable latency for memory instructions, impacting overall performance.

### Features Implemented
- Integration of cache functionality.
- Variable latency for memory instructions.
- Simulation of a shared cache with two different replacement policies (e.g., LRU and one other).
- Input parameters including cache size, block size, associativity, access latency, and main memory access time.
- Output of statistics including stalls, cache miss rate, and IPC.

### Cache Configuration
The simulator allows users to configure cache parameters such as size, block size, associativity, and replacement policy. These parameters can be provided in a separate input file.

### Shared Cache
The cache is designed to cache both instructions and data, serving both cores in the multi-core environment. An instruction fetch is also considered a memory access, introducing variable latency to the Instruction Fetch (IF) stage.

## Usage
1. Configure cache parameters and input settings.
2. Run the simulator with the specified configuration.
3. Execute programs and observe the output statistics including stalls, cache miss rate, and IPC.
4. Analyze the impact of cache configuration on performance and optimize as needed.


---
To incorporate the SIMD unit and the two-bit branch predictor into the existing simulator, let's update the README for Phase 4:

---


## Phase 4: SIMD Unit and Two-Bit Branch Predictor Integration

In this phase, the simulator extends its functionality by integrating a SIMD (Single Instruction, Multiple Data) unit into the processor. Additionally, a two-bit branch predictor is implemented to enhance branch prediction accuracy.

### Features Implemented
- Integration of a SIMD unit into the processor.
- Implementation of SIMD instructions such as `add_vector` and `mul_vector`.
- Addition of a two-bit branch predictor for improved branch prediction.
- Modification of the existing codebase to support the new features.

### SIMD Unit
The SIMD unit enables the execution of SIMD instructions, allowing parallel processing of multiple data elements with a single instruction.

### SIMD Instructions
Examples of SIMD instructions include:
- `add_vector`: Perform element-wise addition on vectors.
- `mul_vector`: Perform element-wise multiplication on vectors.
- Additional SIMD instructions can be implemented based on project requirements.

### Two-Bit Branch Predictor
The two-bit branch predictor enhances branch prediction accuracy by maintaining a history of the last two outcomes of a branch instruction.

## Usage
1. Ensure the simulator is updated with the latest changes incorporating the SIMD unit and branch predictor.
2. Execute programs containing SIMD instructions and branch instructions.
3. Analyze the performance of the simulator with the new features enabled.
4. Evaluate the accuracy of branch prediction using the two-bit branch predictor.


---
