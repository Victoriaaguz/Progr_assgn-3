CPSC 351
Propject: Memory Management SImulation

Members: Victoria Guzman - victoriaaguz@csu.fullerton.edu

Programming Lang: 
The program was written in C++ 

How to Execute the program: 

1. Place the input file `in1.txt` in the same directory as the program file. 

![alt text](image-1.png)

2. Open a terminal and navigate to the directory containing the files.

3. Compile the program using the following command:

   `g++ -std=c++11 -o Mem-manager Mem-manager.cpp`

![alt text](image.png)

4. Run the program using:

  ` ./Mem-manager `

![alt text](image-2.png)

5. The program will generate three output files:

- out1.txt: For page size = 100
- out2.txt: For page size = 200
- out3.txt: For page size = 400

![alt text](image-3.png)


Example Test:
- Input: in1.txt
- Execution Command: ./Mem-manager
- Output: Check `out1.txt`, `out2.txt`, and `out3.txt` for detailed results.

Special notes: 
1. I included error handling to detect malformed input files.
2. The program outputs detailed memory maps after each event.
3. Average turnaround time is displayed at the end of each output file.