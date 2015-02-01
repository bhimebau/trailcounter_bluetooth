// Neural Network in C
// Author: Nhan Do
// Date: 02/01/2015: initial commit

#include<stdio.h>


//*********************** INITIALIZATION *********************/
#define NUM_INPUT_NODE 3
#define NUM_HIDDEN_NODE 2
#define NUM_OUTPUT_NODE 3

// values of inputs
int INPUTS[NUM_INPUT_NODE]
= {0,0,0};

// values of hidden layers
int HIDDENS[NUM_HIDDEN_NODE]
= { 0};

// values of outputs
int OUTPUTS[NUM_OUTPUT_NODE]
= {0};

//weights of Input layer to hidden layer
int WEIGHTS_IH[NUM_INPUT_NODE][NUM_HIDDEN_NODE]
= { 1, 2, 3, 4, 5, 6 }; 

//weights of hidden layer to Output layer
int WEIGHTS_HO[NUM_HIDDEN_NODE][NUM_OUTPUT_NODE]
= { 5, 6, 3, 4, 2, 5 };

// pointer to Index at input array: keep track of circular array
int P_INPUT = 0;
//*********************** end of INITIALIZATION *****************/


//*********************** HELPING FUNCTIONS *********************/
void InitializeWeights(){
	//FILE* input = fopen("input_weights.txt");	
}

void resetOutput(){
	// reset Hidden layer and Output layer to 0;
	int i = 0;
	for (i = 0; i < NUM_HIDDEN_NODE; i++){
		HIDDENS[i] = 0;
	}
	for (i = 0; i < NUM_OUTPUT_NODE; i++){
		OUTPUTS[i] = 0;
	}
}
void UpdateInput(int next_input){
	// write the next_input to the last element of array
	INPUTS[NUM_INPUT_NODE - P_INPUT - 1 ] = next_input;
	P_INPUT++;
	if (P_INPUT == NUM_INPUT_NODE){
		P_INPUT = 0;
	}
}

void EvaluateNet(int next_input){
	int i = 0;
	int j = 0;
	int count = 0;
	// update the input circular array
	UpdateInput(next_input);
	resetOutput();

	// propagate to the hidden layer
	for (i = 0; i < NUM_INPUT_NODE; i++){
		for (j = 0; j < NUM_HIDDEN_NODE; j++){
			// find the index of the right weight for input[i]
			int index = i + P_INPUT;
			if ( index == NUM_INPUT_NODE){
				index = index - NUM_INPUT_NODE;
			}
			HIDDENS[j] += INPUTS[i] * WEIGHTS_IH[index][j];			
		}
	}

	// propagate to the output layer
	for (i = 0; i < NUM_HIDDEN_NODE; i++){
		for (j = 0; j < NUM_OUTPUT_NODE; j++){
			OUTPUTS[j] += HIDDENS[i] * WEIGHTS_HO[i][j];
		}
	}
}

void PrintOutput(){
	int i = 0;
	printf("Print output layers: \n");
	for (i = 0; i < NUM_OUTPUT_NODE; i++){
		printf("%d\n", OUTPUTS[i]);
	}
	printf("\n");
}
//*********************** end of HELPING FUNCTIONS *****************/




//*********************** MAIN PROGRAM *********************/
int main(){
	EvaluateNet(30);
	PrintOutput();

	EvaluateNet(20);
	PrintOutput();

	EvaluateNet(10);
	PrintOutput();

	EvaluateNet(30);
	PrintOutput();

	EvaluateNet(20);
	PrintOutput();

	EvaluateNet(10);
	PrintOutput();

	return 0;
}

// Test data
/*
TEST 1: input test set
 Input: 10  20  30
 int WEIGHTS_IH[NUM_INPUT_NODE][NUM_HIDDEN_NODE]= { 1, 2, 3, 4, 5, 6 };
 int WEIGHTS_HO[NUM_HIDDEN_NODE][NUM_OUTPUT_NODE] = { 5, 6, 3, 4, 2, 5 };
 Output : 2220  1880  2060


TEST 2: Sequence test set
 Initial input = {0,0,0}
 Evaluate this sequence: 30, 20, 10, 30, 20, 10
 Expected Output layer:
 390 300 390
 1190 980 1130
 2220 1880 2060
 1680 1400 1580
 1680 1400 1580
 2220 1880 2060


*/
