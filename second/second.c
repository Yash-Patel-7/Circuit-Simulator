#include "second.h"

// driver
int main(int argc, char **argv) {
	input_value *input_value = input_value_builder(argv[1]);
	output_value *output_value = output_value_builder(argv[1], input_value);
	circuit *circuit = circuit_builder(argv[1], input_value, output_value);
	gate **gates = circuit->gates;
	size_t num_of_gates = circuit->num_of_gates;
	size_t num_of_other_gate_permutations = circuit->num_of_other_gate_permutations;
	size_t *other_gate_indices = circuit->other_gate_indices;
	size_t num_of_other_gates = circuit->num_of_other_gates;
	outcomes *outcomes = outcomes_builder(input_value->original_num_of_inputs);
	size_t *input_permutations = outcomes->outcomes;
	size_t num_of_input_permutations = outcomes->num_of_outcomes;
	bool correct_output = false;
	for (size_t i = 0; i < num_of_other_gate_permutations + 1; i++) {
		if (i > 0 && increment_other_gate_return_skip(circuit, i - 1)) {
			continue;
		}
		bool is_success = false;
		for (size_t j = 0; j < num_of_input_permutations; j++) {
			reset_input_value(input_value, input_permutations[j]);
			for (size_t k = 0; k < num_of_gates; k++) {
				gate *gate = gates[k];
				input_output *IO = gate->input_output;
				gate_type type = gate->type;
				switch (type) {
					case OR:
						is_success = OR_gate(IO, input_value, output_value, j);
						break;
					case AND:
						is_success = AND_gate(IO, input_value, output_value, j);
						break;
					case XOR:
						is_success = XOR_gate(IO, input_value, output_value, j);
						break;
					case NOT:
						is_success = NOT_gate(IO, input_value, output_value, j);
						break;
					case DECODER:
						is_success = DECODER_gate(IO, input_value, output_value, j);
						break;
					case MULTIPLEXER:
						is_success = MULTIPLEXER_gate(IO, input_value, output_value, j);
						break;
					default:
						break;
				}
				if (!is_success) {
					break;
				}
			}
			if (!is_success) {
				break;
			}
		}
		if (!is_success) {
			reset_output_value(output_value);
			continue;
		}
		if (gen_outputs_correct(output_value)) {
			correct_output = true;
			for (size_t z = 0; z < num_of_other_gates; z++) {
				gate_type typee = gates[other_gate_indices[z]]->type;
				char *type_string = NULL;
				switch (typee) {
					case OR:
						type_string = "OR";
						break;
					case AND:
						type_string = "AND";
						break;
					case XOR:
						type_string = "XOR";
						break;
					case NOT:
						type_string = "NOT";
						break;
					case DECODER:
						type_string = "DECODER";
						break;
					case MULTIPLEXER:
						type_string = "MULTIPLEXER";
						break;
					default:
						break;
				}
				printf("G%zu %s\n", z + 1, type_string);
			}
			break;
		} else {
			reset_output_value(output_value);
		}
	}
	if (!correct_output) {
		printf("INVALID");
	}
	input_value = input_value_free(input_value, false);
	output_value = output_value_free(output_value, false);
	circuit = circuit_free(circuit, false);
	outcomes = outcomes_free(outcomes, false);
	return EXIT_SUCCESS;
}


