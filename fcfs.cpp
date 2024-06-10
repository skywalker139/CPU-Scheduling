#include<bits/stdc++.h>
using namespace std;
struct process{
    string pid;
    int arrival_time;
    int burst_time;
    int starting_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    int completion_time;

};
void gantt_chart(vector<process> processes_set){
    set<int> timestamps;
    vector<string> pid;
    for(auto it:processes_set){
        timestamps.insert(it.starting_time);
        timestamps.insert(it.completion_time);
        pid.push_back(it.pid);
    }
    for(auto it:timestamps){
        cout<<it<<"\t"<<it+1;
    }
    cout<<endl;
    for(auto it:pid){
        cout<<"  "<<it<<" "<<"|";
    }
}
vector<process> fcfs(vector<process> processes_set){
    int t=0;
    for(auto process: processes_set){ //process = elements of p from main, process=p1 p2 p3...
        
    }
}

int main(){
    return 0;
}