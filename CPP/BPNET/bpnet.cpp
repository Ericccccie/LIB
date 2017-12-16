#include "bpnet.h"

BP_NET::BP_NET() {
	// input
	for (int i = 0; i < INPUT_DIM; i++)
		input_layer[i] = nullptr;
	// hidden
	for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
		for (int j = 0; j < HIDDEN_DIM; j++)
			hidden_layer[i][j] = nullptr;
	}
	// output
	for (int i = 0; i < OUTPUT_DIM; i++) {
		output_layer[i] = nullptr;
	}
	
	init();
}

void BP_NET::init() {
	srand(time(0));
	error = MAX;
	new_net();
}

void BP_NET::new_net() {
	// input
	for (int i = 0; i < INPUT_DIM; i++) {
		input_layer[i] = new INPUT_NODE();
		for (int j = 0; j < HIDDEN_DIM; j++) {
			input_layer[i]->weight.push_back(get_11_random());
			input_layer[i]->delta_sum.push_back(0);
		}
	}
	// hidden
	for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
		if (i != HIDDEN_LAYER_DIM - 1) {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				hidden_layer[i][j] = new HIDDEN_NODE();
				hidden_layer[i][j]->bias = get_11_random();
				for (int k = 0; k < HIDDEN_DIM; k++) {
					hidden_layer[i][j]->weight.push_back(get_11_random());
					hidden_layer[i][j]->w_delta_sum.push_back(0);
				}
			}
		}
		else {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				hidden_layer[i][j] = new HIDDEN_NODE();
				hidden_layer[i][j]->bias = get_11_random();
				for (int k = 0; k < OUTPUT_DIM; k++) {
					hidden_layer[i][j]->weight.push_back(get_11_random());
					hidden_layer[i][j]->w_delta_sum.push_back(0);
				}
			}
		}
	}
	// output
	for (int i = 0; i < OUTPUT_DIM; i++) {
		output_layer[i] = new OUTPUT_NODE();
		output_layer[i]->bias = get_11_random();
	}
}

void BP_NET::delete_net() {
	// input
	for (int i = 0; i < INPUT_DIM; i++) {
		if (input_layer[i]) {
			delete input_layer[i];
			input_layer[i] = nullptr;
		}
	}
	// hidden
	for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
		for (int j = 0; j < HIDDEN_DIM; j++) {
			if(hidden_layer[i][j]) {
				delete hidden_layer[i][j];
				hidden_layer[i][j] = nullptr;
			}
		}
	}
	// output
	for (int i = 0; i < OUTPUT_DIM; i++) {
		if(output_layer[i]) {
			output_layer[i] = new OUTPUT_NODE();
			output_layer[i] = nullptr;
		}
	}
}

void BP_NET::forward_prop() {
	// hidden layers
	// i for the number of layers
	for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
		if (i == 0) {
			// j for the number of node in one layer
			for (int j = 0; j < HIDDEN_DIM; j++) {
				double sum = 0;
				// k for the number of node in the previous layer
				for (int k = 0; k < INPUT_DIM; k++)
					sum += input_layer[k]->value * input_layer[k]->weight[j];
				sum += hidden_layer[i][j]->bias;
				hidden_layer[i][j]->value = sigmoid(sum);
			}
		}
		else {
			// j for the number of node in one layer
			for (int j = 0; j < HIDDEN_DIM; j++) {
				double sum = 0;
				// k for the number of node in the previous layer
				for (int k = 0; k < HIDDEN_DIM; k++)
					sum += hidden_layer[i - 1][k]->value * hidden_layer[i - 1][k]->weight[j];
				sum += hidden_layer[i][j]->bias;
				hidden_layer[i][j]->value = sigmoid(sum);
			}
		}
	}
	// output layer
	// i for the number of node in output layer
	for (int i = 0; i < OUTPUT_DIM; i++) {
		double sum = 0;
		// j for the number of node in the last hidden layer
		for (int j = 0; j < HIDDEN_DIM; j++)
			sum += hidden_layer[HIDDEN_LAYER_DIM - 1][j]->value * hidden_layer[HIDDEN_LAYER_DIM - 1][j]->weight[i];
		sum += output_layer[i]->bias;
		output_layer[i]->value = sigmoid(sum);
	}
}

void BP_NET::backward_prop() {
	// backward propagation on output layer
	// -- compute delta
	for (int i = 0; i < OUTPUT_DIM; i++) {
		double tmp_error = fabs(output_layer[i]->value - output_layer[i]->exp_value);
		// Error = \Sigma_{0}^{OUTPUT_DIM - 1} 1/2 * error^2;
		error += tmp_error * tmp_error / 2;
		// pure math
		output_layer[i]->delta = (output_layer[i]->value - output_layer[i]->exp_value)
			* (1 - output_layer[i]->value) * output_layer[i]->value;
	}

	// backward propagation on hidden layer
	// -- compute delta
	// i for the number of hidden layer
	for (int i = HIDDEN_LAYER_DIM - 1; i >= 0; i--)	{
		if (i == HIDDEN_LAYER_DIM - 1)	{
			// j for the number of node in one layer
			for (int j = 0; j < HIDDEN_DIM; j++) {
				double sum = 0;
				// magic math part
				for (int k = 0; k < OUTPUT_DIM; k++)
					sum += output_layer[k]->delta * hidden_layer[i][j]->weight[k];
				hidden_layer[i][j]->delta = sum * (1 - hidden_layer[i][j]->value) * hidden_layer[i][j]->value;
			}
		}
		else {
			// j for the number of node in one layer
			for (int j = 0; j < HIDDEN_DIM; j++) {
				double sum = 0;
				// magic math part
				for (int k = 0; k < HIDDEN_DIM; k++)
					sum += hidden_layer[i + 1][k]->delta * hidden_layer[i][j]->weight[k];
				hidden_layer[i][j]->delta = sum * (1 - hidden_layer[i][j]->value) * hidden_layer[i][j]->value;
			}
		}
	}

	// backward propagation on input layer
	// -- update weight delta sum
	for (int i = 0; i < INPUT_DIM; i++) {
		for (int j = 0; j < HIDDEN_DIM; j++)
			input_layer[i]->delta_sum[j] += input_layer[i]->value * hidden_layer[0][j]->delta;
	}

	// backward propagation on hidden layer
	// -- update weight delta sum & bias delta sum
	for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
		if (i == HIDDEN_LAYER_DIM - 1) {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				hidden_layer[i][j]->b_delta_sum += hidden_layer[i][j]->delta;
				for (int k = 0; k < OUTPUT_DIM; k++) {
					hidden_layer[i][j]->w_delta_sum[k] += hidden_layer[i][j]->value * output_layer[k]->delta;
				}
			}
		}
		else {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				hidden_layer[i][j]->b_delta_sum += hidden_layer[i][j]->delta;
				for (int k = 0; k < HIDDEN_DIM; k++)
					hidden_layer[i][j]->w_delta_sum[k] += hidden_layer[i][j]->value * hidden_layer[i + 1][k]->delta;
			}
		}
	}

	// backward propagation on output layer
	// -- update bias delta sum
	for (int i = 0; i < OUTPUT_DIM; i++)
		output_layer[i]->delta_sum += output_layer[i]->delta;
}

void BP_NET::update_weight() {
	// backward propagation on input layer
	// -- update weight
	for (int i = 0; i < INPUT_DIM; i++) {
		for (int j = 0; j < HIDDEN_DIM; j++)
			input_layer[i]->weight[j] -= LEARN_RATE * input_layer[i]->delta_sum[j] / sample_num;
	}

	// backward propagation on hidden layer
	// -- update weight & bias
	for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
		if (i == HIDDEN_LAYER_DIM - 1) {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				// bias
				hidden_layer[i][j]->bias -= LEARN_RATE * hidden_layer[i][j]->b_delta_sum / sample_num;
				// weight
				for (int k = 0; k < OUTPUT_DIM; k++)
					hidden_layer[i][j]->weight[k] -= LEARN_RATE * hidden_layer[i][j]->w_delta_sum[k] / sample_num;
			}
		}
		else {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				// bias
				hidden_layer[i][j]->bias -= LEARN_RATE * hidden_layer[i][j]->b_delta_sum / sample_num;
				// weight
				for (int k = 0; k < HIDDEN_DIM; k++)
					hidden_layer[i][j]->weight[k] -= LEARN_RATE * hidden_layer[i][j]->w_delta_sum[k] / sample_num;
			}
		}
	}

	// backward propagation on output layer
	// -- update bias
	for (int i = 0; i < OUTPUT_DIM; i++)
		output_layer[i]->bias -= LEARN_RATE * output_layer[i]->delta_sum / sample_num;
}

void BP_NET::set_input(vector<double> sample_input) {
	for (int i = 0; i < INPUT_DIM; i++)
		input_layer[i]->value = sample_input[i];
}

void BP_NET::set_output(vector<double> sample_out) {
	for (int i = 0; i < OUTPUT_DIM; i++)
		output_layer[i]->exp_value = sample_out[i];
}

void BP_NET::train(vector<SAMPLE> samples, double threshold) {
	sample_num = samples.size();

	while (error > threshold) {
		//cout << "training error: " << error << endl;
		error = 0;
		// init delta sum
		for (int i = 0; i < INPUT_DIM; i++)
			input_layer[i]->delta_sum.assign(input_layer[i]->delta_sum.size(), 0);
		for (int i = 0; i < HIDDEN_LAYER_DIM; i++) {
			for (int j = 0; j < HIDDEN_DIM; j++) {
				hidden_layer[i][j]->w_delta_sum.assign(hidden_layer[i][j]->w_delta_sum.size(), 0);
				hidden_layer[i][j]->b_delta_sum = 0;
			}
		}
		for (int i = 0; i < OUTPUT_DIM; i++)
			output_layer[i]->delta_sum = 0;

		// tain multiple samples at once
		for (int iter = 0; iter < sample_num; iter++) {
			set_input(samples[iter].input);
			set_output(samples[iter].exp_output);
			forward_prop();
			backward_prop();
		}

		update_weight();
	}
}

vector<double> BP_NET::predict(vector<double> input) {
	vector<double> ans;
	set_input(input);
	forward_prop();
	for (int i = 0; i < OUTPUT_DIM; i++)
		ans.push_back(output_layer[i]->value);
	return ans;
}

double BP_NET::get_11_random() {
	return ((2.0 * rand() / RAND_MAX) - 1);
}

double BP_NET::sigmoid(double x) {
	return (1.0 / (1 + exp(-x)));
}