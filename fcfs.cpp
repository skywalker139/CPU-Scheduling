#include<bits/stdc++.h>
using namespace std;
struct process{
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int starting_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    int completion_time;

};
void gantt_chart(vector<process> processes_set){
    set<int> timestamps;
    vector<int> pid;
    for(auto it:processes_set){
        timestamps.insert(it.starting_time);
        timestamps.insert(it.completion_time);
        pid.push_back(it.pid);
    }
    for(auto it:timestamps){
        cout<<it<<"\t";
    }
    cout<<endl;
    for(auto it:pid){
        cout<<"   P";
        cout<<it;
        cout<<"   "<<"|";
    }
}


void sort_process_set(vector<process> &processes_set) {
    for (size_t i = 0; i < processes_set.size(); ++i) {
        for (size_t j = i + 1; j < processes_set.size(); ++j) {
            if (processes_set[i].arrival_time < processes_set[j].arrival_time ||
                (processes_set[i].arrival_time == processes_set[j].arrival_time && processes_set[i].pid < processes_set[j].pid)) {
                swap(processes_set[i], processes_set[j]);
            }
        }
    }
}

vector<process> fcfs(vector<process> processes_set){
    sort_process_set(processes_set);
    int time=0;
    int total_waiting_time=0, total_turnaround_time=0;
    
    for(auto process: processes_set){ //process = elements of p from main, process=p1 p2 p3...
        if(time<=process.arrival_time){
            //implies cpu is waiting for next task which might arrive after time+x time later, time+x will be arrival time of next process, eg. t=3, next arrival time=4, t=4=starting time
            time = process.arrival_time;
            process.starting_time = process.arrival_time;}

        else if(time>process.arrival_time){
            //t=4 but arrival time for next process is 1, since fifo algo is followed, process with arrival time 1 will be implemented after process with arr time 0 and start time will be 4 
            process.starting_time=time;}

        process.completion_time = process.arrival_time + process.burst_time;
        process.waiting_time = process.starting_time - process.arrival_time;
        process.turnaround_time = process.completion_time - process.starting_time;
        time = time + process.burst_time;
        
        total_waiting_time += process.waiting_time;
        total_turnaround_time += process.turnaround_time;
    }
    float average_waiting_time = total_waiting_time/(processes_set.size());
    float average_turnaround_time = total_turnaround_time/(processes_set.size());
}

vector<process> sjf(vector<process> processes_set){
    //shortest job first algo, tree structure

}

int main(){
    vector<process> process_set{
        {1, 0, 8, 1, 0, 0, 0, 0, 0},
        {2, 1, 4, 2, 0, 0, 0, 0, 0},
        {3, 2, 9, 3, 0, 0, 0, 0, 0},
        {4, 3, 5, 4, 0, 0, 0, 0, 0}
    };
    sort_process_set(process_set);
    gantt_chart(process_set);
    return 0;
}