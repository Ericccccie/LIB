#pragma once
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

#define MAX_INPUT_DIM			1000
#define MAX_OUTPUT_DIM			1000
#define MAX_HIDDEN_LAYER_DIM	100
#define MAX_HIDDEN_DIM			100
#define MAX_ERROR				100
#define DEFAULT_PATH			"bp_net_config.txt"

/*
 * usage:
 * // prepare data
 * vector<double> samplein;
 * vector<double> sampleout;
 * samplein.push_back(0); 
 * samplein.push_back(0); 
 * sampleout.push_back(0);
 *
 * BP_NET::SAMPLE sample;
 * sample.input = samplein[i];
 * sample.exp_output = sampleout[i];
 * 
 * vector<BP_NET::SAMPLE> sample_group;
 * sample_group.push_back(sample);
 *
 * // using BP NET
 * BP_NET bp_net;
 * bp_net.set_attr(
 *		100,		// input_dim
 *		10,			// output_dim,
 *		3,			// hidden_dim,
 *		4,			// hidden_layer_dim,
 *		0.1			// learn_rate
 * );
 * bp_net.new_net();
 * bp_net.train(sampleGroup, 1000);	// train 1000 times
 * bp_net.save_net("bp_net_config.txt");
 * bp_net.delete_net();
 *
 * // restore
 * BP_NET r_net;
 * r_net.restore_net("bp_net_config.txt");
 * ...
*/

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
	void new_net();
	void delete_net();
	void save_net(const char* filename = DEFAULT_PATH);
	void restore_net(const char* filename = DEFAULT_PATH);
	
	void set_attr(
		int input_dim,
		int output_dim,
		int hidden_dim,
		int hidden_layer_dim,
		double learn_rate
	);
	void set_input(vector<double> sample_input);
	void set_output(vector<double> sample_exp_output);
	void train(vector<SAMPLE> samples, double threshold);
	void train(vector<SAMPLE> samples, int times, bool show = false);
	vector<double> predict(vector<double> input, bool normalize = false);

private:
	int input_dim;
	int output_dim;
	int hidden_dim;
	int hidden_layer_dim;
	double learn_rate;

	double error;
	int sample_num;
	INPUT_NODE* input_layer[MAX_INPUT_DIM];
	OUTPUT_NODE* output_layer[MAX_OUTPUT_DIM];
	HIDDEN_NODE* hidden_layer[MAX_HIDDEN_LAYER_DIM][MAX_HIDDEN_DIM];

	void init();

	void forward_prop();
	void backward_prop();
	void update_weight();

	double get_11_random();
	double sigmoid(double x);
	vector<double> normalize(vector<double> &vec);
};