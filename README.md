# CPU-Scheduling

<b>Project Title:</b> 
CPU Scheduling Simulator

<b>Project Description:</b>
This program is designed to simulate and compare various CPU scheduling algorithms including First-Come-First-Serve (FCFS), Shortest Job First (SJF), Priority Scheduling, and Round Robin (RR). It also implements a Multilevel Queue Scheduling mechanism. This simulator helps in understanding and comparing the performance of different scheduling algorithms in terms of metrics like waiting time and turnaround time. 

<b>How to Run the Project</b>
Dependencies:
1. A C++ compiler (e.g., g++, clang++)
2. Standard Template Library (STL) 

<b>Input:</b>
1. Choose whether to use a predefined process set or enter details manually.
2. Provide the time quantum for Round Robin scheduling if required.

<b>Output:</b>
1. Details of processes after applying the most appropriate scheduling algorithm.
2. Comparison of average waiting and turnaround times for all algorithms.
3. Multilevel Queue Scheduling details.
4. Gantt Chart representation of process execution(works smoothly for non premtive processes)

<b>Theory:</b>

<u>FCFS:</u> 
Processes are executed in the order they arrive. 





Find the process with the shortest remaining burst time that has arrived. 
If the process is completed, update its completion time and remove from the remaining_processes

Concept for sjf scheduing,