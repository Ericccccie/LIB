#pragma once
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <cmath>
using namespace std;

#define INPUT_DIM 			2
#define HIDDEN_DIM 			4
#define HIDDEN_LAYER_DIM 	1
#define OUTPUT_DIM 			1
#define LEARN_RATE 			0.9
#define MAX					100

class BP_NET {
public:
	struct INPUT_NODE {
		double value;
		vector<double> weight, delta_sum;
	};
	struct OUTPUT_NODE {
		double value;
		double delta;
		double exp_value;
		double bias;
		double delta_sum;
	};
	struct HIDDEN_NODE {
		double value;
		double delta;
		double bias;
		double b_delta_sum;
		vector<double> weight, w_delta_sum;
	};
	struct SAMPLE {
		vector<double> input, exp_output;
	};

	BP_NET();
	void init();
	void new_net();
	void delete_net();
	
	void set_input(vector<double> sample_input);
	void set_output(vector<double> sample_exp_output);
	void train(vector<SAMPLE> samples, double threshold);
	vector<double> predict(vector<double> input);

private:
	double error;
	int sample_num;
	INPUT_NODE* input_layer[INPUT_DIM];
	OUTPUT_NODE* output_layer[OUTPUT_DIM];
	HIDDEN_NODE* hidden_layer[HIDDEN_LAYER_DIM][HIDDEN_DIM];

	void forward_prop();
	void backward_prop();
	void update_weight();

	double get_11_random();
	double sigmoid(double x);
};