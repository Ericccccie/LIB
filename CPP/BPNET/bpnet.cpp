#include "stdafx.h"
#include "bpnet.h"

BP_NET::BP_NET() {
	input_dim = 0;
	output_dim = 0;
	hidden_dim = 0;
	hidden_layer_dim = 0;
	learn_rate = 0;
	// input
	for (int i = 0; i < input_dim; i++)
		input_layer[i] = nullptr;
	// hidden
	for (int i = 0; i < hidden_layer_dim; i++) {
		for (int j = 0; j < hidden_dim; j++)
			hidden_layer[i][j] = nullptr;
	}
	// output
	for (int i = 0; i < output_dim; i++) {
		output_layer[i] = nullptr;
	}
	
	init();
}

void BP_NET::init() {
	srand(time(0));
	error = MAX_ERROR;
}

void BP_NET::new_net() {
	if (input_dim == 0) throw logic_error("Must set net attr first !");
	// input
	for (int i = 0; i < input_dim; i++) {
		input_layer[i] = new INPUT_NODE();
		for (int j = 0; j < hidden_dim; j++) {
			input_layer[i]->weight.push_back(get_11_random());
			input_layer[i]->delta_sum.push_back(0);
		}
	}
	// hidden
	for (int i = 0; i < hidden_layer_dim; i++) {
		if (i != hidden_layer_dim - 1) {
			for (int j = 0; j < hidden_dim; j++) {
				hidden_layer[i][j] = new HIDDEN_NODE();
				hidden_layer[i][j]->bias = get_11_random();
				for (int k = 0; k < hidden_dim; k++) {
					hidden_layer[i][j]->weight.push_back(get_11_random());
					hidden_layer[i][j]->w_delta_sum.push_back(0);
				}
			}
		}
		else {
			for (int j = 0; j < hidden_dim; j++) {
				hidden_layer[i][j] = new HIDDEN_NODE();
				hidden_layer[i][j]->bias = get_11_random();
				for (int k = 0; k < output_dim; k++) {
					hidden_layer[i][j]->weight.push_back(get_11_random());
					hidden_layer[i][j]->w_delta_sum.push_back(0);
				}
			}
		}
	}
	// output
	for (int i = 0; i < output_dim; i++) {
		output_layer[i] = new OUTPUT_NODE();
		output_layer[i]->bias = get_11_random();
	}
}

void BP_NET::delete_net() {
	// input
	for (int i = 0; i < input_dim; i++) {
		if (input_layer[i]) {
			delete input_layer[i];
			input_layer[i] = nullptr;
		}
	}
	// hidden
	for (int i = 0; i < hidden_layer_dim; i++) {
		for (int j = 0; j < hidden_dim; j++) {
			if(hidden_layer[i][j]) {
				delete hidden_layer[i][j];
				hidden_layer[i][j] = nullptr;
			}
		}
	}
	// output
	for (int i = 0; i < output_dim; i++) {
		if(output_layer[i]) {
			delete output_layer[i];
			output_layer[i] = nullptr;
		}
	}
}

void BP_NET::save_net(const char* filename) {
	ofstream f(filename);
	// attr
	f << input_dim << " " << output_dim << " "
		<< hidden_dim << " " << hidden_layer_dim << " "
		<< learn_rate << endl;
	
	// weight
	// input layer
	for (int i = 0; i < input_dim; i++) {
		for (int j = 0; j < hidden_dim; j++)
			f << input_layer[i]->weight[j] << " ";
	}
	f << endl;

	// hidden layer
	for (int i = 0; i < hidden_layer_dim; i++) {
		if (i == hidden_layer_dim - 1) {
			for (int j = 0; j < hidden_dim; j++) {
				// bias
				f << hidden_layer[i][j]->bias << " ";
				// weight
				for (int k = 0; k < output_dim; k++)
					f << hidden_layer[i][j]->weight[k] << " ";
			}
		}
		else {
			for (int j = 0; j < hidden_dim; j++) {
				// bias
				f << hidden_layer[i][j]->bias << " ";
				// weight
				for (int k = 0; k < hidden_dim; k++)
					f << hidden_layer[i][j]->weight[k] << " ";
			}
		}
	}
	f << endl;

	// output layer
	for (int i = 0; i < output_dim; i++)
		f << output_layer[i]->bias << " ";
	f.close();
}

void BP_NET::restore_net(const char* filename) {
	delete_net();

	ifstream f(filename);
	// attr
	f >> input_dim >> output_dim
		>> hidden_dim >> hidden_layer_dim
		>> learn_rate;
	
	new_net();

	// weight
	// input layer
	for (int i = 0; i < input_dim; i++) {
		for (int j = 0; j < hidden_dim; j++)
			f >> input_layer[i]->weight[j];
	}

	// hidden layer
	for (int i = 0; i < hidden_layer_dim; i++) {
		if (i == hidden_layer_dim - 1) {
			for (int j = 0; j < hidden_dim; j++) {
				// bias
				f >> hidden_layer[i][j]->bias;
				// weight
				for (int k = 0; k < output_dim; k++)
					f >> hidden_layer[i][j]->weight[k];
			}
		}
		else {
			for (int j = 0; j < hidden_dim; j++) {
				// bias
				f >> hidden_layer[i][j]->bias;
				// weight
				for (int k = 0; k < hidden_dim; k++)
					f >> hidden_layer[i][j]->weight[k];
			}
		}
	}

	// output layer
	for (int i = 0; i < output_dim; i++)
		f >> output_layer[i]->bias;
	f.close();
}

void BP_NET::forward_prop() {
	// hidden layers
	// i for the number of layers
	for (int i = 0; i < hidden_layer_dim; i++) {
		if (i == 0) {
			// j for the number of node in one layer
			for (int j = 0; j < hidden_dim; j++) {
				double sum = 0;
				// k for the number of node in the previous layer
				for (int k = 0; k < input_dim; k++)
					sum += input_layer[k]->value * input_layer[k]->weight[j];
				sum += hidden_layer[i][j]->bias;
				hidden_layer[i][j]->value = sigmoid(sum);
			}
		}
		else {
			// j for the number of node in one layer
			for (int j = 0; j < hidden_dim; j++) {
				double sum = 0;
				// k for the number of node in the previous layer
				for (int k = 0; k < hidden_dim; k++)
					sum += hidden_layer[i - 1][k]->value * hidden_layer[i - 1][k]->weight[j];
				sum += hidden_layer[i][j]->bias;
				hidden_layer[i][j]->value = sigmoid(sum);
			}
		}
	}
	// output layer
	// i for the number of node in output layer
	for (int i = 0; i < output_dim; i++) {
		double sum = 0;
		// j for the number of node in the last hidden layer
		for (int j = 0; j < hidden_dim; j++)
			sum += hidden_layer[hidden_layer_dim - 1][j]->value * hidden_layer[hidden_layer_dim - 1][j]->weight[i];
		sum += output_layer[i]->bias;
		output_layer[i]->value = sigmoid(sum);
	}
}

void BP_NET::backward_prop() {
	// backward propagation on output layer
	// -- compute delta
	for (int i = 0; i < output_dim; i++) {
		double tmp_error = fabs(output_layer[i]->value - output_layer[i]->exp_value);
		// Error = \Sigma_{0}^{output_dim - 1} 1/2 * error^2;
		error += tmp_error * tmp_error / 2;
		// pure math
		output_layer[i]->delta = (output_layer[i]->value - output_layer[i]->exp_value)
			* (1 - output_layer[i]->value) * output_layer[i]->value;
	}

	// backward propagation on hidden layer
	// -- compute delta
	// i for the number of hidden layer
	for (int i = hidden_layer_dim - 1; i >= 0; i--)	{
		if (i == hidden_layer_dim - 1)	{
			// j for the number of node in one layer
			for (int j = 0; j < hidden_dim; j++) {
				double sum = 0;
				// magic math part
				for (int k = 0; k < output_dim; k++)
					sum += output_layer[k]->delta * hidden_layer[i][j]->weight[k];
				hidden_layer[i][j]->delta = sum * (1 - hidden_layer[i][j]->value) * hidden_layer[i][j]->value;
			}
		}
		else {
			// j for the number of node in one layer
			for (int j = 0; j < hidden_dim; j++) {
				double sum = 0;
				// magic math part
				for (int k = 0; k < hidden_dim; k++)
					sum += hidden_layer[i + 1][k]->delta * hidden_layer[i][j]->weight[k];
				hidden_layer[i][j]->delta = sum * (1 - hidden_layer[i][j]->value) * hidden_layer[i][j]->value;
			}
		}
	}

	// backward propagation on input layer
	// -- update weight delta sum
	for (int i = 0; i < input_dim; i++) {
		for (int j = 0; j < hidden_dim; j++)
			input_layer[i]->delta_sum[j] += input_layer[i]->value * hidden_layer[0][j]->delta;
	}

	// backward propagation on hidden layer
	// -- update weight delta sum & bias delta sum
	for (int i = 0; i < hidden_layer_dim; i++) {
		if (i == hidden_layer_dim - 1) {
			for (int j = 0; j < hidden_dim; j++) {
				hidden_layer[i][j]->b_delta_sum += hidden_layer[i][j]->delta;
				for (int k = 0; k < output_dim; k++) {
					hidden_layer[i][j]->w_delta_sum[k] += hidden_layer[i][j]->value * output_layer[k]->delta;
				}
			}
		}
		else {
			for (int j = 0; j < hidden_dim; j++) {
				hidden_layer[i][j]->b_delta_sum += hidden_layer[i][j]->delta;
				for (int k = 0; k < hidden_dim; k++)
					hidden_layer[i][j]->w_delta_sum[k] += hidden_layer[i][j]->value * hidden_layer[i + 1][k]->delta;
			}
		}
	}

	// backward propagation on output layer
	// -- update bias delta sum
	for (int i = 0; i < output_dim; i++)
		output_layer[i]->delta_sum += output_layer[i]->delta;
}

void BP_NET::update_weight() {
	// backward propagation on input layer
	// -- update weight
	for (int i = 0; i < input_dim; i++) {
		for (int j = 0; j < hidden_dim; j++)
			input_layer[i]->weight[j] -= learn_rate * input_layer[i]->delta_sum[j] / sample_num;
	}

	// backward propagation on hidden layer
	// -- update weight & bias
	for (int i = 0; i < hidden_layer_dim; i++) {
		if (i == hidden_layer_dim - 1) {
			for (int j = 0; j < hidden_dim; j++) {
				// bias
				hidden_layer[i][j]->bias -= learn_rate * hidden_layer[i][j]->b_delta_sum / sample_num;
				// weight
				for (int k = 0; k < output_dim; k++)
					hidden_layer[i][j]->weight[k] -= learn_rate * hidden_layer[i][j]->w_delta_sum[k] / sample_num;
			}
		}
		else {
			for (int j = 0; j < hidden_dim; j++) {
				// bias
				hidden_layer[i][j]->bias -= learn_rate * hidden_layer[i][j]->b_delta_sum / sample_num;
				// weight
				for (int k = 0; k < hidden_dim; k++)
					hidden_layer[i][j]->weight[k] -= learn_rate * hidden_layer[i][j]->w_delta_sum[k] / sample_num;
			}
		}
	}

	// backward propagation on output layer
	// -- update bias
	for (int i = 0; i < output_dim; i++)
		output_layer[i]->bias -= learn_rate * output_layer[i]->delta_sum / sample_num;
}

void BP_NET::set_attr(int input_dim, int output_dim, int hidden_dim, int hidden_layer_dim, double learn_rate) {
	this->input_dim = input_dim;
	this->output_dim = output_dim;
	this->hidden_dim = hidden_dim;
	this->hidden_layer_dim = hidden_layer_dim;
	this->learn_rate = learn_rate;
}

void BP_NET::set_input(vector<double> sample_input) {
	for (int i = 0; i < input_dim; i++)
		input_layer[i]->value = sample_input[i];
}

void BP_NET::set_output(vector<double> sample_out) {
	for (int i = 0; i < output_dim; i++)
		output_layer[i]->exp_value = sample_out[i];
}

void BP_NET::train(vector<SAMPLE> samples, double threshold) {
	if (input_layer[0] == nullptr) throw logic_error("Must call method new_net() first !");
	
	sample_num = samples.size();

	while (error > threshold) {
		//cout << "training error: " << error << endl;
		error = 0;
		// init delta sum
		for (int i = 0; i < input_dim; i++)
			input_layer[i]->delta_sum.assign(input_layer[i]->delta_sum.size(), 0);
		for (int i = 0; i < hidden_layer_dim; i++) {
			for (int j = 0; j < hidden_dim; j++) {
				hidden_layer[i][j]->w_delta_sum.assign(hidden_layer[i][j]->w_delta_sum.size(), 0);
				hidden_layer[i][j]->b_delta_sum = 0;
			}
		}
		for (int i = 0; i < output_dim; i++)
			output_layer[i]->delta_sum = 0;

		// train multiple samples at once
		for (int iter = 0; iter < sample_num; iter++) {
			set_input(samples[iter].input);
			set_output(samples[iter].exp_output);
			forward_prop();
			backward_prop();
		}

		update_weight();
	}
}

void BP_NET::train(vector<SAMPLE> samples, int times, bool show) {
	if (input_layer[0] == nullptr) throw logic_error("Must call method new_net() first !");

	sample_num = samples.size();
	int _times = times;

	while (_times--) {
		if (show && _times % 500 == 0)
			cout << "Done " << times - _times << " times of training" << endl;
		//cout << "training error: " << error << endl;
		error = 0;
		// init delta sum
		for (int i = 0; i < input_dim; i++)
			input_layer[i]->delta_sum.assign(input_layer[i]->delta_sum.size(), 0);
		for (int i = 0; i < hidden_layer_dim; i++) {
			for (int j = 0; j < hidden_dim; j++) {
				hidden_layer[i][j]->w_delta_sum.assign(hidden_layer[i][j]->w_delta_sum.size(), 0);
				hidden_layer[i][j]->b_delta_sum = 0;
			}
		}
		for (int i = 0; i < output_dim; i++)
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

vector<double> BP_NET::predict(vector<double> input, bool normalize) {
	vector<double> ans;
	set_input(input);
	forward_prop();
	for (int i = 0; i < output_dim; i++)
		ans.push_back(output_layer[i]->value);
	if(normalize) return this->normalize(ans);
	return ans;
}

double BP_NET::get_11_random() {
	return ((2.0 * rand() / RAND_MAX) - 1);
}

double BP_NET::sigmoid(double x) {
	return (1.0 / (1 + exp(-x)));
}

vector<double> BP_NET::normalize(vector<double> &vec) {
	vector<double> n;
	int size = vec.size();
	double sum = 0;
	for (int i = 0; i < size; i++)
		sum += vec[i];
	for (int i = 0; i < size; i++)
		n.push_back(vec[i] / sum);
	return n;
}