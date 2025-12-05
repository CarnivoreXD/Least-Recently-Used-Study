# Project 5 Batch and Drain

## Student Information:

Guillermo Rojas<br>
Student ID: 008008657<br>
https://github.com/CarnivoreXD/priority-queue-study

## Collaboration & Sources:

This work is primarily my own. I used the following resources for learning, debugging, and comparison

I used geeksforgeeks, stackoverflow, and chatGPT to debug and compare my code when I was getting errors when trying to make trace files for the batch then drain

I used chatGpt to diagnose an error where the output was not being saved as a csv result and troubleshot it that way.

I had claude help me understand how to modify the trace generator to produce both profiles because I was running into issues trying to copy the code from the huffman code.


## Implementation Details:

I started by merging the two trace generators into one file (src/trace-generators/huffman_coding/main.cpp) because originally I had two separate main() functions—one for Huffman and one for batch-then-drain—and CMake kept throwing “duplicate symbol '_main'” errors. At first I thought I had to fix CMakeLists.txt, but Claude helped me realize it was easier to just combine them into one generator that does both profiles. So now, instead of running two different programs, I run ./huffman_trace once, and it generates both trace types:   

    Huffman profile: N inserts + (N-1)×(extract, extract, insert) → total 4N–1 ops, keys 3–128 (lots of duplicates)  
    Batch-then-drain profile: N inserts with keys from [1, 2²⁰] → then N extracts → total 2N ops, almost no duplicates
     
I split the old generateTrace() into two clean functions: generateHuffmanTrace() and generateBatchThenDrainTrace(). Both use the same random seed (23) so comparisons are fair. The traces get saved to separate folders: traces/huffman_coding/ and traces/batch_then_drain/. Super simple now—no more file mess. 

Then I updated the harness (src/harness/main.cpp) so it doesn’t just run one profile at a time. Originally it only processed huffman_coding, but now I made it loop through both.

I also ran into a dumb mistake at first—I was running ./harness from inside cmake-build-debug/, but the program was looking for trace files relative to the project root. So it kept saying “Failed to open file.” Fixed it by just always running from the top level:  

## Testing & Status: 

The way I tested it was I tested everything step by step. First I generated all the traces. Then I checked to see if the files were there and it built properply and when it did I ran the harness which took a long time to run so I let it run but I closed the program without saving the results in the csv manually because it didnt record it automatically so I ran the harness again and saved the reuslts in th csv file provided. I opened the HTML plot tool and dragged my results in and the graphs popped up and I did my anaylsis on that and thought about what happened. I ran the tests twice just in case I messed up somewhere but the results stayed the same.