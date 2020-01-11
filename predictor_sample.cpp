#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
#include<vector>

using namespace std;

int main(int argc, char *argv[]) {
	// Open file for reading
    ifstream infile(argv[1]);
    //each line of the file was a branch instruction, containing the branch's address, target address (both in hexadecimal), and the correct prediction (taken/T, not taken/NT)
	string addr, behavior, target;
	//open file for writing
	ofstream outfile(argv[2]);
	int total=0;//total number of branches (required to check accuracy)
	string ** table = new string*[30];//a 2D array of string arrays to store my predictions 
    //for example, table[2] through table[8] stores the predictions of my one bit predictor, and table[9] through table[15] stores the two bit predictions
    //I needed several tables for each predictor due to the project's requirements to test the predictors with different table sizes (16 entries, 32 entries, .. to 2048 entries)
    //These different table sizes result in different accuracies 
    //if our table can hold 16 values, then all addresses that end with the same 4 bits map to the same index in the table (as 2^4 = 16)
    //Another way of thinking about this is that every address must map to some index in the table, but due to limited size, different addresses will map to the same index of the table
    //So, some addresses might be accessed for the first time, but due to being mapped to a location that has already been accessed, may use another addresses's prediction
    for(int i = 0; i< 30; i++){
		table[i] = new string[2048];//each array can store a maximum of 2048 entries, so I just gave each table the maximum for convience
	}
	for(int r = 0; r<30; r++){//nested loop used to initialize our prediction tables
		for(int c=0; c<2048; c++){
			if (r>8 && r<16) table[r][c] = "11";//for the two bit predictor, I was instructed to start at state 11 - strongly taken - so I initalized all values in table[9] - table[15] as "11"
		}
	}
	while(infile >> addr >> behavior >> target) {//loop through the input, and call twoBitBimodal with various table sizes of 16 through 2048 with table indexes 9 through 15
		//two bit bimodal, occupies correct[9-15], table[9-15]
		tableSize = 16;
		for(int j = 0; j< 7; j ++){
			twoBitBimodal(addr, behavior, table, correct, tableSize, currTable);
            //So, for line x of the input, I first call the method with table size 16, table index 9, then record the result in an array
			tableSize *= 2;
			currTable++;
            //After recording the result, I increment the table size and index, and run the method on line x with these new parameters
            //the method is called with our: branch address in hex, a reference to our table array (which contains all prediction tables, and can be edited as it is a reference),
            //a reference to an array used to keep track of how many predictions we have gotten right for this specific case, our table size, and our current table in our table 2D array
		}
        total++;//number of branch instructions tested increased by 1
    }
	return 0;
}

void twoBitBimodal(string addr, string behavior, string** (&table), int (&correct)[30], int tableSize, int currTable){
	int index = bimodalIndex(tableSize, addr);//first, I call a method to determine how many bits of our address to use as an index into the table
	twoBitPredict(behavior, index, currTable, table, correct);//next call the method that handles the actual prediction
    //twoBitPredict takes the actual behavior of the branch (T or NT), the index we just computed, the current table in our table 2D array, a reference to our table 2D array,
    //and a reference to our array used to keep track of how many predictions we have gotten right 
}

int bimodalIndex(int tableSize, string addr){
	int digits = log2(tableSize);//number of bits to use as index
	string lsb;
	string binaryAddr = toBinary(addr);//translate branch address in hex to binary
	for(int i= binaryAddr.length()-digits; i<binaryAddr.length(); i++){//loop through the last "digit" characters
		lsb+=binaryAddr[i];//concatinate our string that will be the index into the table
	}
	return stoi(lsb, nullptr, 2);//return an int that is our index to the table
}

void twoBitPredict(string behavior, long index, int currTable, string** (&table), int (&correct)[30]){
    //finally, the method that checks our current prediction against the actual, determines if its correct, and updates our predictor if necessary
	if((behavior == "T" && (table[currTable][index] == "11" || table[currTable][index] == "10")) || (behavior == "NT" && (table[currTable][index] == "00" || table[currTable][index] == "01"))){
		//long if statement that is true if our prediction is right
        correct[currTable] = correct[currTable]+1;//add 1 to number of correct predictions for this branch predictor
		if(table[currTable][index] == "10" || table[currTable][index] == "11"){//if weakly or strongly taken,
			table[currTable][index] = "11";//update predictor (move state) to strongly taken (as our prediction was taken)
		}
		else{//if weakly or strongly not taken,
			table[currTable][index] = "00";//update predictor (move state) to strongly not taken (as our prediction was not taken)
		}
	}else{//our prediction was wrong, so we need to update our predictor/move states accordingly
		if(table[currTable][index] == "11"){//predicted strongly taken, was not taken
			table[currTable][index] = "10";//go to weakly taken
		}else if(table[currTable][index] == "10"){//predicted weakly taken, was not taken
			table[currTable][index] = "01";//go to weakly not taken
		}else if(table[currTable][index] == "01"){//predicted weakly not taken, was taken
			table[currTable][index] = "10";//go to weakly taken
		}else{//predicted strongly not taken, was taken
			table[currTable][index] = "01";//go to weakly taken
		}
	}
}

//some things that went well:
//I was able to reuse this method for a similar predictor later on by adding a new parameter
//successfully got the correct number of predictions on all inputs, and got a 100 on the project
//debugged as I wrote code, not after writing all of it, to incrementally check my progress and fix issues as they arose 

//some things I wish I did differently:
//used vectors instead of arrays, with simpler and cleaner initilization of values 
//found a cleaner way of using my arrays instead of my 2D array system - I find it a little clunky 
//like my second point, clean up a little more in terms of my methods - I feel like I passed a lot of parameters which can seem confusing to others







