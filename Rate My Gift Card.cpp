#include <unordered_map>
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

//Use of unordered map data structure
struct RateMyGiftCardMap
{
	//Sub class container object for products
	struct product
	{
		string ID = "";
		string name = "";
		string price = "";
		double rating = 0;
	};

	//Sub class container object to take parsed ratings
	struct rating
	{
		int rateCount = 0;
		double rateScore = 0;
	};

	//Class for product vector sorting
	struct sortVector
	{
		bool operator() (const product& p1, const product& p2)
		{
			if (p1.rating != p2.rating)
				return p1.rating > p2.rating;
			return p1.name > p2.name;
		}
	};

	//Data structures for ratings and the gift cards
	unordered_map<string, rating> ratingsMap;
	unordered_map<string, product> giftCardsMap;

	//Searches through the unordered map of products, saves them, and sorts them in a vector for output.
	vector<product> searchMap(string name)
	{
		vector<product> output;
		for (auto i : giftCardsMap)
		{
			if (i.second.name.find(name) != string::npos)
			{
				output.push_back(i.second);
			}
		}
		sort(output.begin(), output.end(), sortVector());
		return output;
	}

	//Constructor to parse list of products and places them into unordered map
	RateMyGiftCardMap()
	{
		//Initializing variables
		string line;

		//Opening files
		ifstream cardRatings;
		cardRatings.open("ratings.csv");

		ifstream giftCards;
		giftCards.open("products.txt");

		//Parses ratings, cumulates the ratings and counts the amount of ratings
		while (getline(cardRatings, line))
		{
			string getID = line.substr(0, line.find(','));
			line.erase(0, line.find(',') + 1);
			line.erase(0, line.find(',') + 1);
			double getRating = stod(line.substr(0, line.find(',')));

			ratingsMap[getID].rateCount++;
			ratingsMap[getID].rateScore += getRating;
		}

		//Parsing list of products, takes the ID, name, and price. Also calculates average rating from the ratings map
		while (getline(giftCards, line))
		{
			string getID = line.substr(0, line.find('|'));
			line.erase(0, line.find('|') + 1);
			string getName = line.substr(0, line.find('|'));
			line.erase(0, line.find('|') + 1);
			string getPrice = line.substr(0, line.find('|'));

			giftCardsMap[getID].ID = getID;
			giftCardsMap[getID].name = getName;
			giftCardsMap[getID].price = getPrice;

			giftCardsMap[getID].rating = ratingsMap[getID].rateScore / ratingsMap[getID].rateCount;
		}

		//Close files
		giftCards.close();
		cardRatings.close();
	}
};

//Use of priority queue as data structure
struct RateMyGiftCardQueue
{
	//Product subclass
	struct product
	{
		string ID = "";
		string name = "";
		string price = "";
		double rating = 0;

		product(string _ID, string _name, string _price, double _rating)
		{
			ID = _ID;
			name = _name;
			price = _price;
			rating = _rating;
		}
	};

	//Rating subclass
	struct rating
	{
		int rateCount = 0;
		double rateScore = 0;
	};

	//Priority queue comparison class
	struct compareQueue
	{
		bool operator()(const product& p1, const product& p2)
		{
			if (p1.rating != p2.rating)
				return p1.rating < p2.rating;
			return p1.name < p2.name;
		}
	};

	//Initializing data structures
	unordered_map<string, rating> ratingsMap;
	priority_queue<product, vector<product>, compareQueue> giftCardsQueue;
	vector<product> giftCardsSorted;

	//Searches priority queue sorted gift cards popped into vector
	vector<product> searchQueue(string name)
	{
		vector<product> output;
		for (auto i : giftCardsSorted)
		{
			if (i.name.find(name) != string::npos)
			{
				output.push_back(i);
			}
		}
		return output;
	}

	//Constructor to parse list of products and places them into priority queue
	RateMyGiftCardQueue()
	{
		//Initializing variables
		string line;

		//Opening files
		ifstream cardRatings;
		cardRatings.open("ratings.csv");

		ifstream giftCards;
		giftCards.open("products.txt");

		//Parsing ratings
		while (getline(cardRatings, line))
		{
			string getID = line.substr(0, line.find(','));
			line.erase(0, line.find(',') + 1);
			line.erase(0, line.find(',') + 1);
			double getRating = stod(line.substr(0, line.find(',')));

			ratingsMap[getID].rateCount++;
			ratingsMap[getID].rateScore += getRating;
		}

		//Parsing product information and calculating rating average
		while (getline(giftCards, line))
		{
			string getID = line.substr(0, line.find('|'));
			line.erase(0, line.find('|') + 1);
			string getName = line.substr(0, line.find('|'));
			line.erase(0, line.find('|') + 1);
			string getPrice = line.substr(0, line.find('|'));
			double rate = ratingsMap[getID].rateScore / ratingsMap[getID].rateCount;

			product* push = new product(getID, getName, getPrice, rate);

			giftCardsQueue.push(*push);
		}

		//Pops ordered products into queue
		while (!giftCardsQueue.empty())
		{
			product productPointer = giftCardsQueue.top();
			giftCardsSorted.push_back(productPointer);
			giftCardsQueue.pop();
		}

		//Closing files
		giftCards.close();
		cardRatings.close();
	}
};

int main()
{
	//This is our title. Yay!!!
	cout << " __________         __              _____            ________.__  _____  __    _________                  .___" << endl;
	cout << " \\______   \\_____ _/  |_  ____     /     \\ ___.__.  /  _____/|__|/ ____\\/  |_  \\_   ___ \\_____ _______  __| _/" << endl;
	cout << "  |       _/\\__  \\\\   __\\/ __ \\   /  \\ /  <   |  | /   \\  ___|  \\   __\\\\   __\\ /    \\  \\/\\__  \\\\_  __ \\/ __ | " << endl;
	cout << "  |    |   \\ / __ \\|  | \\  ___/  /    Y    \\___  | \\    \\_\\  \\  ||  |   |  |   \\     \\____/ __ \\|  | \\/ /_/ | " << endl;
	cout << "  |____|_  /(____  /__|  \\___  > \\____|__  / ____|  \\______  /__||__|   |__|    \\______  (____  /__|  \\____ | " << endl;
	cout << "         \\/      \\/          \\/          \\/\\/              \\/                          \\/     \\/           \\/ " << endl;

	//Start the program with a continuous loop until users want to exit out.
	cout << endl << endl;
	int option = 1;

	while (option == 1)
	{
		cout << "Search For Your Gift Card" << endl;
		string input;
		//Get entire line for gift cards such as "American Eagle"
		getline(cin, input);
		cout << endl;
		cout << "What Data Structure Would You Like To Search With" << endl;
		cout << "1. Map" << endl;
		cout << "2. Priority Queue" << endl;

		//Choose data structure
		string dataStructure;
		getline(cin, dataStructure);
		cout << endl;

		//Make sure they choose valid option such as 1 or 2
		if (dataStructure != "1" && dataStructure != "2")
		{
			cout << "Not A Valid Option" << endl;
			continue;
		}

		//Map outputs
		if (dataStructure == "1")
		{
			//Start the time for building the map of input products
			auto startMap = high_resolution_clock::now();

			//Create map data structure
			RateMyGiftCardMap mapStructure;
			//Parse input for the following input
			auto outMap = mapStructure.searchMap(input);

			int loop = 0;

			//Determines how many products to print out. If over 10, cap at 10. Otherwise, just the size of map
			if (outMap.size() > 10)
				loop = 10;
			else
				loop = outMap.size();

			//If no giftcards are found
			if (loop == 0)
			{
				cout << "No Gift Cards Found!" << endl;
				continue;
			}

			cout << "Top " << loop << " Rated Cards (Map)" << endl;

			//Prints out the following products
			for (int i = 0; i < loop; i++)
			{
				cout << outMap[i].name << "\n   Rating: " << outMap[i].rating << "\n   Price: ";
				if (outMap[i].price.size() == 0)
					cout << "No Price" << endl << endl;
				else
					cout << outMap[i].price << endl << endl;
			}

			//Stop the time for the procedure
			auto stopMap = high_resolution_clock::now();
			//Calculates the difference in stop-start time
			auto durationMap = duration_cast<milliseconds>(stopMap - startMap);

			//Print time difference
			cout << "Building and searching for the top products with \"" << input << "\" took " << endl;
			cout << durationMap.count() << " milliseconds using an unordered map." << endl;
		}
		//Queue outputs
		else
		{
			//Start the time for building the queue of input products
			auto startQueue = high_resolution_clock::now();

			//Create queue data structure
			RateMyGiftCardQueue queueStructure;
			//Parse input for the following input
			auto outQueue = queueStructure.searchQueue(input);

			int loop = 0;

			//Determines how many products to print out. If over 10, cap at 10. Otherwise, just the size of queue
			if (outQueue.size() > 10)
				loop = 10;
			else
				loop = outQueue.size();

			//If no giftcards are found
			if (loop == 0)
			{
				cout << "No Gift Cards Found!" << endl;
				continue;
			}

			cout << "Top " << loop << " Rated Cards (Queue)" << endl;

			//Prints out the following products
			for (int i = 0; i < loop; i++)
			{
				cout << outQueue[i].name << "\n   Rating: " << outQueue[i].rating << "\n   Price: ";
				if (outQueue[i].price.size() == 0)
					cout << "No Price" << endl << endl;
				else
					cout << outQueue[i].price << endl << endl;
			}

			//Stop the time for the procedure
			auto stopQueue = high_resolution_clock::now();
			//Calculates the difference in stop-start time
			auto durationQueue = duration_cast<milliseconds>(stopQueue - startQueue);

			//Print time difference
			cout << "Building and searching for the top products with \"" << input << "\" took " << endl;
			cout << durationQueue.count() << " milliseconds using a priority queue." << endl;
		}

		//Ask for next query
		cout << endl;
		cout << "What Would You Like To Do Next?" << endl;
		cout << "1. Search For A New Card" << endl;
		cout << "2. Quit" << endl;

		string nextOption;
		getline(cin, nextOption);

		//Make sure their option is valid
		while (nextOption != "1" && nextOption != "2")
		{
			cout << "Not A Valid Option" << endl << endl;
			cout << "What Would You Like To Do Next?" << endl;
			cout << "1. Search For A New Card" << endl;
			cout << "2. Quit" << endl;
			getline(cin, nextOption);
		}

		//Quit
		if (nextOption == "2")
			break;

		cout << endl;
	}

	return 0;
}