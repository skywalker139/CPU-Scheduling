#include<bits/stdc++.h>
using namespace std;
struct process{
    int pid;
    int arrival_time=0;
    int burst_time;
    int priority;
    int queue_priority=0;
    int starting_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    int completion_time;
    int remaining_burst_time=0; //for sjf scheduling
    int last_execution_time=0; //priority schedulling


    bool operator==(const process& other) const{
        return pid == other.pid;}

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

void process_details(const vector<process>& processes_set) {
    cout<<"PID\tArrival\tBurst\tStart\tCompletion\tWaiting\tTurnaround\tPriority\n";
    for (const auto& process : processes_set) {
        cout<<"P"<< process.pid <<"\t"<< process.arrival_time <<"\t"<< process.burst_time<<"\t"<<process.starting_time <<"\t"
                  << process.completion_time << "\t\t" 
                  << process.waiting_time << "\t" 
                  << process.turnaround_time << "\t" 
                  << process.priority << "\n";
    }
}

vector<process> deepcopy_processes(const vector<process>& processes_set){
    return processes_set;
}

void sort_process_set(vector<process> &processes_set){
    for (size_t i = 0; i < processes_set.size(); ++i) {
        for (size_t j = i + 1; j < processes_set.size(); ++j) {
            if (processes_set[i].arrival_time < processes_set[j].arrival_time ||
                (processes_set[i].arrival_time == processes_set[j].arrival_time && processes_set[i].pid < processes_set[j].pid)) {
                swap(processes_set[i], processes_set[j]);
            }
        }
    }
}

float average_waiting_time(const vector<process>& processes_set){
    int total_waiting_time = 0;
    for (const auto& process: processes_set){
        total_waiting_time += process.waiting_time;
    }
    return static_cast<float>(total_waiting_time)/processes_set.size();
}
float average_turnaround_time(const vector<process>& processes_set){
    int total_turnaround_time = 0;
    for (const auto& process : processes_set){
        total_turnaround_time += process.turnaround_time;
    }
    return static_cast<float>(total_turnaround_time)/processes_set.size();
}

vector<process> fcfs(vector<process>& processes_set){
    sort_process_set(processes_set);
    int time=0;
    
    for(auto& process: processes_set){ //process = elements of p from main, process=p1 p2 p3...
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
    }

    return processes_set;
}

vector<process> sjf(vector<process>& processes_set){
    sort_process_set(processes_set);
    int time=0;

    vector<process> remaining_processes= processes_set;
    
    for(auto& process: processes_set){
        process.remaining_burst_time=process.burst_time;}

    while (!remaining_processes.empty()) {
        process* current_process = nullptr;

        for (auto& process : remaining_processes) {
            if (process.arrival_time <= time) {
                if (current_process == nullptr || process.remaining_burst_time < current_process->remaining_burst_time) {
                    current_process = &process;
                }
            }
        }

        if (current_process == nullptr) {
            time++;
            continue;
        }

        if (current_process->remaining_burst_time == current_process->burst_time) {
            for (auto& process: processes_set) {
                if (process.pid == current_process->pid) {
                    process.starting_time = time;
                    break;
                }
            }
        }

        current_process->remaining_burst_time--;
        time++;

        if (current_process->remaining_burst_time == 0) {
            for (auto& process : processes_set) {
                if (process.pid == current_process->pid) {
                    process.completion_time = time;
                    process.turnaround_time = process.completion_time - process.arrival_time;
                    process.waiting_time = process.turnaround_time - process.burst_time;                   
                    break;
                }
            }

            remaining_processes.erase(remove(remaining_processes.begin(), remaining_processes.end(), *current_process), remaining_processes.end());
        }
    }
    return processes_set;
}

void update_priorities(vector<process>& processes, int aging_factor){
    for (auto& process: processes){
        if (process.remaining_burst_time > 0){
            process.priority += aging_factor;}
    }
}

vector<process> priority(vector<process>& processes_set, int aging_factor=1){    //defualt 1
    sort_process_set(processes_set);
    int time = 0;

    vector<process> remaining_processes = processes_set;

    for (auto& process : processes_set) {
        process.remaining_burst_time = process.burst_time;
    }

    while (!remaining_processes.empty()) {
        process* current_process = nullptr;
        update_priorities(remaining_processes, aging_factor);       

        for (auto& process : remaining_processes) {
            if (process.arrival_time <= time) {
                
                if (process.priority > 0 && time - process.last_execution_time >= 3) {   //aging for processes halted more than 3 times
                    process.priority--;
                    process.last_execution_time = time;
                }
                
                if (current_process == nullptr || process.priority < current_process->priority) {
                    current_process = &process;
                }
            }
        }

        if (current_process == nullptr) {
            time++;
            continue;
        }

        if (current_process->remaining_burst_time == current_process->burst_time) {
            current_process->starting_time = time;
        }

        current_process->remaining_burst_time--;
        time++;

        if (current_process->remaining_burst_time == 0) {
            current_process->completion_time = time;
            current_process->turnaround_time = current_process->completion_time - current_process->arrival_time;
            current_process->waiting_time = current_process->turnaround_time - current_process->burst_time;

            remaining_processes.erase(remove(remaining_processes.begin(), remaining_processes.end(), *current_process), remaining_processes.end());
        } else {
            current_process->last_execution_time = time;
        }
    }
    return processes_set;
}

vector<process> round_robin(vector<process>& processes_set, int time_quantum){
    sort_process_set(processes_set);
    int time = 0;
    
    vector<int> ready_queue;

    for (auto& process : processes_set) {
        process.remaining_burst_time = process.burst_time;
    }

    size_t index = 0;
    while (index < processes_set.size() || !ready_queue.empty()){
        
        while (index < processes_set.size() && processes_set[index].arrival_time <= time) {
            ready_queue.push_back(index);
            index++;
        }

        if (ready_queue.empty()){
            time++;
        }

        int current_index = ready_queue.front();
        ready_queue.erase(ready_queue.begin());
        process& current_process = processes_set[current_index];

        if(current_process.remaining_burst_time == current_process.burst_time) {
            current_process.starting_time = time;
        }

        int execution_time = min(time_quantum, current_process.remaining_burst_time);
        current_process.remaining_burst_time -= execution_time;
        time += execution_time;

        while(index < processes_set.size() && processes_set[index].arrival_time <= time) {
            ready_queue.push_back(index);
            index++;
        }

        if(current_process.remaining_burst_time > 0){
            ready_queue.push_back(current_index);
        }
        else{
            current_process.completion_time = time;
            current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
            current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;
        }
    }
    return processes_set;
}

void multilevel_queue(vector<process>& processes_set, int time_quantum){
    vector<process> high_priority_queue;
    vector<process> low_priority_queue;

    for (auto& p: processes_set) {
        p.remaining_burst_time = p.burst_time;
        if (p.queue_priority == 0) {
            high_priority_queue.push_back(p);
        } else {
            low_priority_queue.push_back(p);
        }

    fcfs(low_priority_queue);
    round_robin(high_priority_queue, time_quantum);

    cout<<"For Low Priority Queue:"<<endl;
    process_details(low_priority_queue);
    cout<<endl;
    cout<<"Average Turnaround time for the process is: "<<average_turnaround_time(low_priority_queue)<<endl;
    cout<<"Average Waiting time for the process is: "<<average_waiting_time(low_priority_queue)<<endl<<endl;
    
    
    cout<<"For High Priority Queue:"<<endl;
    process_details(high_priority_queue);
    cout<<"Average Turnaround time for the process is: "<<average_turnaround_time(high_priority_queue)<<endl;
    cout<<"Average Waiting time for the process is: "<<average_waiting_time(high_priority_queue)<<endl;


    }
}

string compare_scheduling_algorithms(const vector<process>& processes, int time_quantum){
    vector<process> fcfs_processes = deepcopy_processes(processes);
    fcfs(fcfs_processes);
    float fcfs_avg_waiting_time = average_waiting_time(fcfs_processes);

    vector<process> sjf_processes = deepcopy_processes(processes);
    sjf(sjf_processes);
    float sjf_avg_waiting_time = average_waiting_time(sjf_processes);

    vector<process> rr_processes = deepcopy_processes(processes);
    round_robin(rr_processes, time_quantum);
    float rr_avg_waiting_time = average_waiting_time(rr_processes);

    vector<process> priority_processes = deepcopy_processes(processes);
    priority(priority_processes);
    float priority_avg_waiting_time = average_waiting_time(fcfs_processes);

    cout << "FCFS Average Waiting Time: " << fcfs_avg_waiting_time << endl;
    cout << "SJF Average Waiting Time: " << sjf_avg_waiting_time << endl;
    cout << "Round Robin Average Waiting Time: " << rr_avg_waiting_time << endl;
    cout << "Priority Average Waiting Time: " << priority_avg_waiting_time << endl;

    float min_avg_waiting_time = min({fcfs_avg_waiting_time, sjf_avg_waiting_time, rr_avg_waiting_time,priority_avg_waiting_time});
    if (min_avg_waiting_time == fcfs_avg_waiting_time){
        return "FCFS";
    } else if (min_avg_waiting_time == sjf_avg_waiting_time){
        return "SJF";
    } else if (min_avg_waiting_time == rr_avg_waiting_time){
        return "Round Robin";
    } else{
        return "Priority";
    }
}

int main(){
    vector<process> predefined_process_set{
        {1, 0, 8, 1, 0, 0, 0, 0, 0, 0},
        {2, 1, 4, 2, 0, 0, 0, 0, 0, 1},
        {3, 2, 9, 3, 0, 0, 0, 0, 0, 2},
        {4, 3, 5, 4, 0, 0, 0, 0, 0, 2}
    };
    vector<process> process_set;
    char choice;
    cout<<"Press Y to execute predefined process set and N for entering details of processes manually..... ";
    cin>>choice;
    if(choice=='N' || choice=='n') {process_set = predefined_process_set;}
    else if (choice=='n' || choice=='N'){
        int num_processes;
        cout << "Enter the number of processes: ";
        cin >> num_processes;
        for (int i = 0; i < num_processes; i++) {
            process p;
            p.pid = i + 1;
            cout << "Enter arrival time for process " << p.pid << ": ";
            cin >> p.arrival_time;
            cout << "Enter burst time for process " << p.pid << ": ";
            cin >> p.burst_time;
            cout << "Enter priority for process " << p.pid << ": ";
            cin >> p.priority;
            cout << "Enter queue priority for process " << p.pid << ": ";
            cin >> p.queue_priority;
            p.starting_time = 0;
            p.waiting_time = 0;
            p.response_time = 0;
            p.turnaround_time = 0;
            p.completion_time = 0;
            p.remaining_burst_time = p.burst_time;
            p.last_execution_time = 0;
            process_set.push_back(p);
        }
    }
    else if (choice=='y' || choice=='Y'){
        process_set = predefined_process_set;
    }
    
    int time_quantum;
    cout<<"Enter Time Quantum (Enter 1 if not required): ";
    cin>>time_quantum;
    
    string system_process = compare_scheduling_algorithms(process_set, time_quantum);
    cout<<system_process<<"scheduling has the shortest average waiting time, so it is most appropriate criteria for cup to execute.\n Process details after"<<system_process<<" scheduling is: "<<endl;

    vector<process> completed_process_set;
    if(system_process=="FCFS"){
        completed_process_set = fcfs(process_set);
    }
    else if (system_process=="SJF"){ completed_process_set = sjf(process_set);}
    else if (system_process=="Round Robin"){ completed_process_set = round_robin(process_set, time_quantum);}
    else if (system_process=="Priority"){ completed_process_set = priority(process_set);}
    
    process_details(completed_process_set);
    multilevel_queue(process_set, time_quantum);

    
    gantt_chart(completed_process_set);

    cout<<endl<<"Thank you";
    return 0;
}