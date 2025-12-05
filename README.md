# Project 5 Least Recently Used Analysis

## Student Information:

Guillermo Rojas<br>
Student ID: 008008657<br>
https://github.com/CarnivoreXD/Least-Recently-Used-Study


## Running my Project

When running from CLion IDE set the working directory for lru(this is the lru trace) and harness-lru as follows: 

| Target | Working Directory |
|--------|-------------------|
| `lru` | `$PROJECT_DIR$/src/trace-generators/lru` |
| `harness-lru` | `$PROJECT_DIR$/src/harness-lru` |

## Collaboration & Sources:

This work is primarily my own I used the following as troubleshooting resources and ideas:

-ChatGPT and Claude for debugging and understanding the project architecture. I also used this to understand JSON launch files and how to properly use them to run the project on VSCode since CLion uses .idea and CMake as the source of the configurations.

-GeeksForGeeks and StackOverflow for C++ syntax issues and questions


## Implementation Details:

The trace generators creates files we can use to test the harness by simulating an LRU cache. it uses a list to maintain O(1) order and a hashmap for O(1) lookups. The program has an access stream consisting of 12N total accesses that is from 4N unique words. To make sure it is a good and valid test the words are shuffled using a fixed seed of 23 before being written to a file as a sequence of inserts and extracts.

## Testing & Status: 

I coded everything on VScode and the way I tested my program I uploaded it to Blue ran it there with set JSON files and then ran it locally on my Windows 10 computer with a different set of JSON files, then on WSL on my system, then I finally sent it over to a mac to test my program and ran into issues where the directories werent being loaded automatically so I created an .idea folder to save the settings but was having issues but eventually settled with manually setting the working directory. For testing of my code I generated all the trace files and executed the harness to output the CSV and .txt files on all 4 systems with almost no issues (it would have been much easier with Java's JVM). I confirmed the CSV contained exactly 22 rows and that my data made sense. In the end, I imported the data into the D3 plotting tool to generate histograms which visually helped me analyze the clustering behavior.