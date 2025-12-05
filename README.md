# Project 5 Least Recently Used Analysis

## Student Information:

Guillermo Rojas<br>
Student ID: 008008657<br>
https://github.com/CarnivoreXD/Least-Recently-Used-Study


## Running my Project

When running from CLion IDE

1. Open the project in CLion
2. Go to Run -> Edit Configurations
3. Set working directories for the following targets as stated:

| Target | Working Directory |
|--------|-------------------|
| `lru` | `$PROJECT_DIR$/src/trace-generators/lru` |
| `harness-lru` | `$PROJECT_DIR$/src/harness-lru` |

4. Click Apply -> Ok
5. Select 'lru' from the run dropdown (this is the trace generator)
6. Select Run
7. Select 'harness-lru' from the run dropdown
8. Select Run

This will generate the trace files for the LRU and then it runs the harness and outputs the files to 
csvs/hashmapProfile.csv and maps/*.txt respectively



## Collaboration & Sources:

This work is primarily my own I used the following as troubleshooting resources and ideas:

-ChatGPT and Claude for debugging and understanding the project architecture. I also used this to understand JSON files and how to properly use them to run the project on VSCode since CLion uses .idea and CMake as the source of the configurations

-GeeksForGeeks and StackOverflow for C++ syntax issues and questions

## Project 


## Implementation Details:

I started by merging the two trace generators into one file (src/trace-generators/huffman_coding/main.cpp) because originally I had two separate main() functions—one for Huffman and one for batch-then-drain—and CMake kept throwing “duplicate symbol '_main'” errors. At first I thought I had to fix CMakeLists.txt, but Claude helped me realize it was easier to just combine them into one generator that does both profiles. So now, instead of running two different programs, I run ./huffman_trace once, and it generates both trace types:   

    Huffman profile: N inserts + (N-1)×(extract, extract, insert) → total 4N–1 ops, keys 3–128 (lots of duplicates)  
    Batch-then-drain profile: N inserts with keys from [1, 2²⁰] → then N extracts → total 2N ops, almost no duplicates
     
I split the old generateTrace() into two clean functions: generateHuffmanTrace() and generateBatchThenDrainTrace(). Both use the same random seed (23) so comparisons are fair. The traces get saved to separate folders: traces/huffman_coding/ and traces/batch_then_drain/. Super simple now—no more file mess. 

Then I updated the harness (src/harness/main.cpp) so it doesn’t just run one profile at a time. Originally it only processed huffman_coding, but now I made it loop through both.

I also ran into a dumb mistake at first—I was running ./harness from inside cmake-build-debug/, but the program was looking for trace files relative to the project root. So it kept saying “Failed to open file.” Fixed it by just always running from the top level:  

## Testing & Status: 

The way I tested it was I tested everything step by step. First I generated all the traces. Then I checked to see if the files were there and it built properply and when it did I ran the harness which took a long time to run so I let it run but I closed the program without saving the results in the csv manually because it didnt record it automatically so I ran the harness again and saved the reuslts in th csv file provided. I opened the HTML plot tool and dragged my results in and the graphs popped up and I did my anaylsis on that and thought about what happened. I ran the tests twice just in case I messed up somewhere but the results stayed the same.