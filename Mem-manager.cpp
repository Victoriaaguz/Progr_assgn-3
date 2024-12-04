#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <iomanip> 

struct Process {
    int id;
    int arrivalTime;
    int lifetime; 
    int memoryRequirement;
    std::vector<int> addressSpace;
}; 

struct MemoryBlock {
    int start, end; //memory range 
    int processId; // -1 indicates a free block
};

class MemoryManager {
    int memorySize;
    int pageSize; 
    std::vector<MemoryBlock> memoryMap;

public: 
    MemoryManager(int size, int page) : memorySize(size), pageSize(page) {
        memoryMap.push_back({0, memorySize - 1, -1}); //Entire memory is initially free
    }
    
    bool allocate(Process &process) {
        int totalPages = (process.memoryRequirement + pageSize - 1) / pageSize;// calucate pages needed
        int requiredMemory = totalPages * pageSize;
        int startIndex = -1;

        //Find a free block large enough 
        for (size_t i = 0; i < memoryMap.size(); ++i) {
            if (memoryMap[i].processId == -1 && (memoryMap[i].end - memoryMap[i].start + 1) >= requiredMemory) {
                startIndex = i;
                break;
            }
        }
        if (startIndex == -1) return false; //No sufficient free memory 

        MemoryBlock &block = memoryMap[startIndex];
        int allocatedStart = block.start;
        int allocatedEnd = allocatedStart + requiredMemory - 1;

        //update memory map 
        if (allocatedEnd < block.end) {
            memoryMap.insert(memoryMap.begin() + startIndex + 1, {allocatedEnd + 1, memoryMap[startIndex].end, -1});
        }
        block.end = allocatedEnd;
        block.processId = process.id;

        return true; 
    }
    void release(int processId) {
        for(auto &block : memoryMap){
            if (block.processId == processId){
                block.processId = -1; //Mark as free
            }
        }
        mergeFreeBlocks();
    }
    void mergeFreeBlocks(){
        for (size_t i = 0; i < memoryMap.size() - 1; ){
            if (memoryMap[i].processId == -1 && memoryMap[i + 1].processId == -1){
                memoryMap[i].end = memoryMap[i + 1].end;
                memoryMap.erase(memoryMap.begin() + i + 1);
            } else {
                ++i;
            }
        }
    }

    void display(std::ofstream &outfile) {
        for (const auto &block: memoryMap) {
            std::cout << block.start << "-" << block.end << ": ";
            if (block.processId == -1) {
                std::cout << "Free Frame(s)";
            } else {
                std::cout << "Process " << block.processId;
            }
            std::cout << std::endl;

        }
    }
};

void simulateMemoryManagement(const std::string &inputFile, const std::string &outputFile, int pageSize){
    int memorySize = 2000;

    //Read input file 
    std::ifstream infile(inputFile);
    std::ofstream outfile(outputFile);

    if (!infile || !outfile) {
        std:: cerr << "Error opening file.\n";
        return; 
    }

    int n; 
    infile >> n;
    std::vector<Process> processes; 

    for(int i = 0; i < n; ++i) {
        Process p; 
        infile >> p.id >> p.arrivalTime >> p.lifetime;
        int numPieces; 
        infile >> numPieces;
        p.memoryRequirement = 0; 
        for (int j = 0; j < numPieces; ++j) {
            int piece; 
            infile >> piece; 
            p.memoryRequirement += piece; 
        }
        processes.push_back(p);
    }
    MemoryManager mm(memorySize, pageSize);
    std::queue<Process> inputQueue;
    int time = 0; 
    double totalTurnaroundTime = 0; 
    int completedProcesses = 0;

    while (time <= 100000  && (!inputQueue.empty() || completedProcesses < processes.size())){
        //Handles process arrivals
        for (auto &p : processes){
            if(p.arrivalTime == time){
                outfile << "t = " << time << ": Process " << p.id << " arrives\n";
                inputQueue.push(p);


                //show input queue 
                outfile << "       Input Queue: [";
                std::queue<Process> tempQueue = inputQueue;
                while (!tempQueue.empty()){
                    outfile << tempQueue.front().id << " ";
                    tempQueue.pop();
                }
                outfile << "]\n";
            }
        }
        // Allocate memory for prcesses in the queue
        if (!inputQueue.empty()){
            Process &frontProcess = inputQueue.front();
            if (mm.allocate(frontProcess)){
                outfile << "       MM moves Process " << frontProcess.id << " to memory\n";
                inputQueue.pop();

                //show updated input queue 
                outfile << "       Input Queue: [";
                std::queue<Process> tempQueue = inputQueue;
                while (!tempQueue.empty()){
                    outfile << tempQueue.front().id << " ";
                    tempQueue.pop();
                }
                outfile <<"]\n";

                //show memory map 
                outfile << "       Memory Map:\n";
                mm.display(outfile);
            }
        }

        // Check for process completion 
        for (auto &p : processes) {
            if (time == p.arrivalTime + p.lifetime){
                mm.release(p.id);
                outfile << "t = " << time << " : Process " << p.id << " completes\n";

                // Show memory map
                outfile << "       Memory Map:\n";
                mm.display(outfile);
                
                //caluate turnaround time 
                totalTurnaroundTime += (time - p.arrivalTime);
                completedProcesses ++;
            }
        }
        ++time; // increment simiulation clock 
    }
    // caluate and display averge turnarounf time 
    if (completedProcesses > 0) {
        double averageTurnaroundTime = totalTurnaroundTime / completedProcesses;
        outfile << "\nAverage Turnaround Time: "
                << std::fixed << std::setprecision(2)
                << averageTurnaroundTime << "\n";
    } else {
        outfile << "\nNo processes were completed. \n";
    }
}

int main() {
    // reads in1.txt file and creates the output file to the size 
    simulateMemoryManagement("in1.txt", "out1.txt", 100);
    simulateMemoryManagement("in1.txt", "out2.txt", 200);
    simulateMemoryManagement("in1.txt", "out3.txt", 400);

    std::cout << "Simulation completed. Output files generated: out1.txt, out2.txt, out3.txt\n";

    return 0;
}