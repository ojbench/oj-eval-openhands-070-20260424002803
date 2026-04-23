# ifndef _SJTU_CPP_FINAL_SRC_HPP_
# define _SJTU_CPP_FINAL_SRC_HPP_

# include "cpu.hpp"
# include "task.hpp"

using namespace std;

namespace sjtu {
    // FCFS method based CPU.
    class CPU_FCFS : public CPU {
    private:
        uint current_task_index;
        
    public:
        CPU_FCFS() : CPU() {
            current_task_index = 0;
        }
        
        pair<CPUState, uint> run() override {
            // Remove completed tasks
            while (current_task_index < tasks.size() && tasks[current_task_index].time == 0) {
                current_task_index++;
            }
            
            // If no tasks available, CPU is idle
            if (current_task_index >= tasks.size()) {
                return make_pair(idle, 0);
            }
            
            // Execute current task for 1 time unit
            tasks[current_task_index].time--;
            
            // If task completed, move to next
            if (tasks[current_task_index].time == 0) {
                uint completed_task_id = tasks[current_task_index].task_id;
                current_task_index++;
                return make_pair(busy, completed_task_id);
            }
            
            return make_pair(busy, tasks[current_task_index].task_id);
        }
    };

    // SRTF method based CPU.
    class CPU_SRTF : public CPU {
    private:
        int current_task_index;
        
    public:
        CPU_SRTF() : CPU() {
            current_task_index = -1;
        }
        
        pair<CPUState, uint> run() override {
            // Remove completed tasks
            for (auto it = tasks.begin(); it != tasks.end();) {
                if (it->time == 0) {
                    it = tasks.erase(it);
                } else {
                    ++it;
                }
            }
            
            // If no tasks available, CPU is idle
            if (tasks.empty()) {
                current_task_index = -1;
                return make_pair(idle, 0);
            }
            
            // Find task with minimum remaining time (and earliest arrival if tie)
            int min_time = tasks[0].time;
            int min_index = 0;
            for (int i = 1; i < tasks.size(); i++) {
                if (tasks[i].time < min_time) {
                    min_time = tasks[i].time;
                    min_index = i;
                }
            }
            
            current_task_index = min_index;
            
            // Execute selected task for 1 time unit
            tasks[current_task_index].time--;
            
            uint task_id = tasks[current_task_index].task_id;
            
            // If task completed, it will be removed in next call
            return make_pair(busy, task_id);
        }
    };

    // priority method based CPU.
    class CPU_PRIORITY : public CPU {
    private:
        int current_task_index;
        
    public:
        CPU_PRIORITY() : CPU() {
            current_task_index = -1;
        }
        
        pair<CPUState, uint> run() override {
            // Remove completed tasks
            for (auto it = tasks.begin(); it != tasks.end();) {
                if (it->time == 0) {
                    it = tasks.erase(it);
                } else {
                    ++it;
                }
            }
            
            // If no tasks available, CPU is idle
            if (tasks.empty()) {
                current_task_index = -1;
                return make_pair(idle, 0);
            }
            
            // Find task with highest priority (lowest priority value, earliest arrival if tie)
            uint min_priority = tasks[0].priority;
            int min_index = 0;
            for (int i = 1; i < tasks.size(); i++) {
                if (tasks[i].priority < min_priority) {
                    min_priority = tasks[i].priority;
                    min_index = i;
                }
            }
            
            current_task_index = min_index;
            
            // Execute selected task for 1 time unit
            tasks[current_task_index].time--;
            
            uint task_id = tasks[current_task_index].task_id;
            
            // If task completed, it will be removed in next call
            return make_pair(busy, task_id);
        }
    };
}

# endif