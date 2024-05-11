# CO-24_Binary_Nexus
# MOM
Date:11-5-24<br>
Members:Ikshitha,Preethika
Decisions:Implemented SIMD unit, cleaned the code.
Date:10-5-24<br>
Members:Ikshitha,Preethika
Decisions:Debug for L2,was unable to solve the problem hence reverted back to TBP.Worked on implementation of SIMD unit.
Date:9-5-24<br>
Members:Ikshitha,Preethika
Decisions: Encorporated an L2 cache(non-inclusive).Worked on debugging as the number of cycles were wrong.
Date:8-5-24<br>
Members:Ikshitha,Preethika
Decisions: Decided for the implentation of TBP,L2 level cache ans SIMD unit.Implemented TBP.
Date:9-4-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Debugging
Date:8-4-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Debugging
Date:7-4-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Debugging, Implementation considering memory access for instruction fetch 
Date:4-4-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Implemented cache replacment policies,encorporated cache in pipeling
Date:3-4-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Configured cache class <br>
Date:31-3-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Set input file for cache_config<br>
Date:24-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Again tried on la, changed the data type of RAM from int to char, so that int is stored as four bytes..,the value in the four bytes is concatinated and converted to int while loading the address or word..implemented selection sort..finally seen that same memory is used for both the cores.  
Date:23-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Worked on implementation of la , we were trying to load addresses from the memory into the register..but didn't work and went with lw instead for storing the base address of the array and implemented bubble sort.<br>  
Date:21-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Updated prilimnary instructions,implemented jal,jalr,labels, branch instructions<br>
Date:20-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Updated prilimnary instructions.
Date:18-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Implemented prilimnary instructions<br>
Date:17-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Implemented prilimnary instructions<br>
Date:13-2-24<br>
Members:Ikshitha,Preethika<br>
Decisions:Had a change of mind and decided to go with c++ to have better performance.<br>
Date:12-2-24
Members:Ikshitha,Preethika<br>
Decisions:Decided the language to be python, looked at seniors project for reference.


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
