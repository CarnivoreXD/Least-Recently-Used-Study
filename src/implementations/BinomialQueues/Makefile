binomialQueue.x: BQnode.o BinomialQueue.o InvertedListDictionary.o SmallIntMixedOperations.o main.o BinomialQueueTester.o
	g++ -std=c++17 BQnode.o BinomialQueue.o InvertedListDictionary.o SmallIntMixedOperations.o main.o BinomialQueueTester.o -o binomialQueue.x

BQnode.o: BQnode.hpp BQnode.cpp 
	g++ -std=c++17 -c BQnode.cpp -o BQnode.o

BinomialQueue.o: BinomialQueue.cpp BinomialQueue.hpp BQnode.hpp
	g++ -std=c++17 -c BinomialQueue.cpp -o BinomialQueue.o

InvertedListDictionary.o: InvertedListDictionary.cpp InvertedListDictionary.hpp
	g++ -std=c++17 -c InvertedListDictionary.cpp -o InvertedListDictionary.o

SmallIntMixedOperations.o: SmallIntMixedOperations.cpp SmallIntMixedOperations.hpp InvertedListDictionary.hpp
	g++ -std=c++17 -c SmallIntMixedOperations.cpp -o SmallIntMixedOperations.o

BinomialQueueTester.o: BinomialQueueTester.cpp BinomialQueueTester.hpp BinomialQueue.hpp \
  BQnode.hpp SmallIntMixedOperations.hpp InvertedListDictionary.hpp
	g++ -std=c++17 BinomialQueueTester.cpp -o BinomialQueueTester.o -c

main.o: main.cpp BinomialQueue.hpp BQnode.hpp SmallIntMixedOperations.hpp InvertedListDictionary.hpp
	g++ -std=c++17 -c main.cpp -o main.o

clean:
	rm -fr *.o *~ *.x
