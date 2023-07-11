import numpy as np

def extract_values_close_to_zero(differentiated_values, threshold, positive_slope=True):
	values_close_to_zero = []

	for i in range(1, len(differentiated_values)):
		current_value = differentiated_values[i]
		previous_value = differentiated_values[i-1]

		if abs(current_value) <= threshold[i]:
			if (positive_slope and current_value >= previous_value) or (not positive_slope and current_value <= previous_value):
				values_close_to_zero.append(current_value)

	return values_close_to_zero

differentiated_values = [-0.1, -0.2, 0.3, 0.1, -0.05, 0.15, -0.25]
num_values = len(differentiated_values)

mu = 0
sigma = 0.1
threshold = np.random.normal(mu, sigma, num_values)

positive_slope = True

values_close_to_zero = extract_values_close_to_zero(differentiated_values, threshold, positive_slope)

print("Values close to zero:", values_close_to_zero)
