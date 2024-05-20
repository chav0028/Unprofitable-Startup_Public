#-------------------------------------------------------------------------------------------------#
# Parse financial data and use it to build and train a Neural Network to predict the Net Income
# The Neural Network is built using PyTorch, and its performance is then evaluated
# using different metrics.
# This files outputs the Neural Network as an ONNX file, and the scaling data (the different
# min and max values used to scale the data for the neural network) as a JSON file
#-------------------------------------------------------------------------------------------------#
from math import fabs
import os
from pickle import TRUE # Used to find current file path

import torch # Requires installing pyTorch "pip install torch"
import torch.nn as nn # Used to build the neural network 
import torch.optim as optim # Used for optimizer function of neural networks 
import torch.onnx # Used to export neural network as an ONNX file
from torch.utils.data import TensorDataset, DataLoader # Used to generate DataLoaders

import pandas as pd # Used for data frames
import random # Set fixed random seed

from sklearn.preprocessing import MinMaxScaler # Used to scale data, requires installing "pip install scikit-learn"
from sklearn.model_selection import train_test_split # Used to split the data into training and evaluation sets
from sklearn.metrics import mean_absolute_error, mean_absolute_percentage_error, mean_squared_error # Function used to evaluate neural network

import matplotlib.pyplot as plt # Used for plotting the data

import netron # Visualize neural networks 

import numpy as np # Use numPy arrays for neural network evaluation

import json # Export the scaling values as a JSON



# Allow printing the whole columns (otherwise it will be truncated as ...)
pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)


#-------------------------------------------------------------------------------------------------#
# Settings
#-------------------------------------------------------------------------------------------------#
# Defines to allow turning on-off some sections of code
FINANCIAL_DATA_TIME = "Monthly" # "Quaterly", "Monthly" or "Weekly", the time intervals of the financial data used to train the neural network

# Filte the financial data by date range,
# this only filters the data that has already been read from the CSV file
FILTER_FINANCIAL_DATA_TIME_RANGE = True # Enable filtering the data
FILTERED_DATA_START_DATE = "2013-03-31" # date format: "YYYY-MM-DD"
FILTERED_DATA_END_DATE = "2023-12-31" # date format: "YYYY-MM-DD"

SHOW_ACTUAL_PREDICTED_GRAPH = True # Show a graph of the actual vs predicted values

EXPORT_ONNX_FILE = True # Export the neural network as an ONNX file
EXPORT_SCALING_DATA = True # Export the scaling data as a JSON file

PRINT_FINAL_TESTING_DATA_ROW_PREDICTION = False # Predict the Net Income of the last row of the evaluation data
VERBOSE_PRINTING = False # Print additional information to the console



#-------------------------------------------------------------------------------------------------#
# Set random seets to fixed value to reproduce results
#-------------------------------------------------------------------------------------------------#
random_seed_value = 12345
torch.manual_seed(random_seed_value)    
random.seed(random_seed_value)



#-------------------------------------------------------------------------------------------------#
# Functions Debugging
#-------------------------------------------------------------------------------------------------#
def print__data_info(a_data, a_data_label):
    num_rows = a_data.shape[0]
    num_columns = a_data.shape[1]
    print(a_data_label)
    # Visualize the data 
    print(f"Data has {num_rows} rows and {num_columns} columns")   
    # Visualize the first row of the data
    print(f"First Row of {a_data_label}")
    print(a_data.head(1))
    print("\n")


#-------------------------------------------------------------------------------------------------#
# Read data from CSV file
#-------------------------------------------------------------------------------------------------#
if (FINANCIAL_DATA_TIME == "Quaterly"):
    financial_data = pd.read_csv("Data/Output/quarterlyFinancialData.csv", index_col=0)
elif (FINANCIAL_DATA_TIME == "Monthly"):
    financial_data = pd.read_csv("Data/Output/monthlyFinancialData.csv", index_col=0)
elif (FINANCIAL_DATA_TIME == "Weekly"):
    financial_data = pd.read_csv("Data/Output/weeklyFinancialData.csv", index_col=0)
else:
    print("Unspecified financial data time, defaulting to Monthly")
    FINANCIAL_DATA_TIME = "Monthly"
    financial_data = pd.read_csv("Data/Output/monthlyFinancialData.csv", index_col=0)

# The first column in the data is considered as an index, and not a regular column
# so specify it in place so that it can be handled as a normal column
financial_data.reset_index(inplace=True)

# Visualize the read data result
if VERBOSE_PRINTING:
    print__data_info(financial_data, "Unscaled Data")
    


#-------------------------------------------------------------------------------------------------#
# Scale data
#-------------------------------------------------------------------------------------------------#
date_column_name = "fiscalDateEnding"  

if FILTER_FINANCIAL_DATA_TIME_RANGE:
    # Filter the data by date
    financial_data[date_column_name] = pd.to_datetime(financial_data[date_column_name]) # Convert date column to datetime
    financial_data = financial_data[(financial_data[date_column_name] >= FILTERED_DATA_START_DATE) & (financial_data[date_column_name] <= FILTERED_DATA_END_DATE)]

    # Visualize the filtered data
    if VERBOSE_PRINTING:
        print(f"Earliest date: {financial_data[date_column_name].min()}")
        print(f"Latest date: {financial_data[date_column_name].max()}") 
        print("\n")


        print__data_info(financial_data, "Filtered Data")




#-------------------------------------------------------------------------------------------------#
# Scale data
#-------------------------------------------------------------------------------------------------#
# Scale the data with MinMax to a 0 to 1 range, except the date to not affect the sequential time series
inputs_minMax_scaler = MinMaxScaler()
output_minMax_scaler = MinMaxScaler()

# Do not scale the dates of the time series, but instead
# convert it to a numeric UNIX timestamp 
if date_column_name in financial_data.columns: # Only if the data has a date column
    unscaled_dates_column = pd.to_datetime(financial_data[date_column_name]) # Get the dates as datetime
    timestamp_dates = unscaled_dates_column.astype("int64")  # Convert the datetime to a integer
    timestamp_dates = timestamp_dates.div(10**9) # Divide the integer by the number of nanoseconds in a second 
else:
    print("No date columns in the data")

# Convert the date to nanoseconds
if date_column_name in financial_data.columns:
    scaled_input = financial_data.drop(columns=[date_column_name]) # Remove the date column
    scaled_input = pd.concat([timestamp_dates, pd.DataFrame(scaled_input)], axis=1)
else:
    scaled_input = financial_data

# Separate the output column form the input data
output_column_name = "netIncome"
scaled_input = scaled_input.drop(columns=[output_column_name])

# Scale the inputs
scaled_input = inputs_minMax_scaler.fit_transform(scaled_input) # Scale the data to 0 to 1 range

# Scale the output separately
unscaled_output = financial_data[output_column_name]
unscaled_output = unscaled_output.values.reshape(-1, 1) # Reshape the data into an acceptable format for the scaler
scaled_output = output_minMax_scaler.fit_transform(unscaled_output)

# Preserve columns names
input_column_names = list(financial_data.columns)
input_column_names.remove(output_column_name) # Remove output column
scaled_input = pd.DataFrame(scaled_input, columns=input_column_names)


# Visualize data
if VERBOSE_PRINTING:
    print("Scaled Data")
    print("Output First Row")
    print(scaled_output[0]) # Print the first row of the output data
    print("Input First Row")
    print(scaled_input.head(1)) # Print the first row of the input data
    print("\n")



#-------------------------------------------------------------------------------------------------#
# Prepare data
#-------------------------------------------------------------------------------------------------#

# Split the data into training and evaluation sets
# train_test_split splits data into X,Y tranining and evaluation sets
training_input, evaluation_input, training_output, evaluation_output = train_test_split(
        scaled_input.values, 
        scaled_output, 
        test_size = 0.2, # When float percentage of data being used for tests, if Int the number of samples
        random_state = random_seed_value, # Fixed seed for reproducible results
        shuffle = False # Because of the time series data, preserve its order
    )

# Prepare data for Neural Network by converting into a PyTorch Tensor.
# This serve as an multi-dimensional array of single data type elements,
# with optimisations for GPU/Parallel processing
# For this case store the data in Float32 multidimensional arrays
training_input_tensor = torch.tensor(training_input, dtype=torch.float32)
training_output_tensor = torch.tensor(training_output, dtype=torch.float32)
evaluation_input_tensor = torch.tensor(evaluation_input, dtype=torch.float32)
evaluation_output_tensor = torch.tensor(evaluation_output, dtype=torch.float32)



#-------------------------------------------------------------------------------------------------#
# Build the Neural Network
#-------------------------------------------------------------------------------------------------#
# https://machinelearningmastery.com/develop-your-first-neural-network-with-pytorch-step-by-step/
# Class that inherits from nn.Module
class NetIncomeNN(nn.Module):
    
    # Intialize layers in init(constructor) function
    def __init__(self, input_size, output_size):
        super(NetIncomeNN, self).__init__() # Call parent class nn.Module init 
        # Create hidden layers
        # https://docs.kanaries.net/topics/Python/nn-linear
        hidden_layer1_output = 18
        hidden_layer2_output = 36
        self.hidden_layer1 = nn.Linear(input_size, hidden_layer1_output) # Set first linear layer
        self.hidden_layer2 = nn.Linear(hidden_layer1_output, hidden_layer2_output) # Set second linear layer
        self.hidden_layer3 = nn.Linear(hidden_layer2_output, output_size) # Set third linear layer   
        
        # Activation functions
        # https://machinelearningmastery.com/choose-an-activation-function-for-deep-learning/
        self.activation_function_1to2 = nn.ReLU()
        self.activation_function_2to3 = nn.ReLU()

    # forward is the defined function to process input when the neural network is run
    def forward(self, x):
        x = self.hidden_layer1(x)
        x = self.activation_function_1to2(x)
        x = self.hidden_layer2(x)
        x = self.activation_function_2to3(x)
        x = self.hidden_layer3(x)
        return x

# Get the number of inputs and outputs
input_size = scaled_input.shape[1]  # Number of input features
output_size =  1 if len(scaled_output.shape) == 1 else scaled_output.shape[1]  # Number of outputs, if there is only 1 (special case array) account for it

# Create the instance of the neural network
netIncome_neural_net = NetIncomeNN(input_size,  output_size)



#-------------------------------------------------------------------------------------------------#
# Train the Neural Network
#-------------------------------------------------------------------------------------------------#
print(f"------------------------Training------------------------")

# Define loss function, used to measure the target with the predicted output,
# practically a function to determine how accurate the prediction will be
# https://towardsdatascience.com/loss-functions-and-their-use-in-neural-networks-a470e703f1e9
# Since data has no  noticeable outliers, and is a regression problem, use Mean Squared Error (MSE)
loss_function = nn.MSELoss()

# The optimizer is the algorithm used to adjust the neural network weights
# Adam tends to be a generally good option for this (based on a gradient descent algorithm)
# https://www.kdnuggets.com/2022/12/tuning-adam-optimizer-parameters-pytorch.html
optimizer = optim.Adam(netIncome_neural_net.parameters(), lr=0.015)

num_epochs = 250 # How many times the entire dataset will be pased through the model
batch_size = 25 # The amount of samples that will be passed to the model at once  
epoch_loss_print_frequency = 5 # How often to print the loss

# Use a dataloader, to account for the data not being divisible by
# the batch size (final batch may be smaller) 
training_dataset = TensorDataset(training_input_tensor, training_output_tensor)
training_dataloader = DataLoader(training_dataset, batch_size=batch_size, shuffle=False, drop_last = False)

# Train data a certain number of times (epochs) at a certain
# number of samples (batch size) at a time
for epoch in range(num_epochs):
    # Obtain batches from dataloader to account for possible "leftovers" from batch
    for input_batch, output_batch in training_dataloader:
        # Reshape the target batch to match the shape of the 
        # predicted tensor of torch.Size([10, 1])
        output_batch = output_batch.view(-1, 1)        

        # Predict the result using the neural network model (forward pass)
        predicted_output = netIncome_neural_net(input_batch)
        
        # Measure how accurate the prediction was
        loss = loss_function(predicted_output, output_batch)
        
        # Reset the optimizer gradients
        optimizer.zero_grad()
        
        # backpropagation, compute the optimizer gradients per data model
        loss.backward()
        
        # Update optimizer weights for next pass
        optimizer.step()
    # Print loss (how accurate prediction is) every 5 epochs
    if epoch % epoch_loss_print_frequency == 0 or epoch == num_epochs - 1:
        print(f"Epoch {epoch}, has loss of: {loss}")
        


#-------------------------------------------------------------------------------------------------#
# Evaluate the Neural Network
#-------------------------------------------------------------------------------------------------#
print(f"------------------------Evaluation------------------------")

# Testing data
testing_dataset = TensorDataset(evaluation_input_tensor, evaluation_output_tensor)
testing_dataloader = DataLoader(testing_dataset, batch_size=batch_size, shuffle=False, drop_last = False)

# Ensure neural network model is in evaluation mode
netIncome_neural_net.eval()

# Compare predicted results with actual results   
predicted_outputs = []
actual_outputs = []

# Disable gradient calculations, as an optimisation, since these
# are only used for backward propagation when training
with torch.no_grad():

    # Use the testing dataset (not seen by the Neural Network during training)
    # to do predictions and measure their accuracy
    for input_data, target_output in testing_dataloader:
        # Use the Neural network model to do a prediction
        predicted_output = netIncome_neural_net(input_data)
        # Add the predicted and actual result to the lists
        predicted_outputs.extend(predicted_output)
        actual_outputs.extend(target_output)

# Inverse transform the scaled outputs to their original scale
predicted_outputs_unscaled = output_minMax_scaler.inverse_transform(predicted_outputs)
actual_outputs_unscaled = output_minMax_scaler.inverse_transform(actual_outputs)

if VERBOSE_PRINTING:
    print("Predicted Outputs:")
    print(predicted_outputs_unscaled)
    print("Actual Outputs:")
    print(actual_outputs_unscaled)
    print("\n")


# Evaluate difference of predicted and actual results with different metrics
# https://www.datacamp.com/tutorial/pytorch-tutorial-building-a-simple-neural-network-from-scratch
# https://machinelearningmastery.com/time-series-forecasting-performance-measures-with-python/
# https://www.statology.org/mse-vs-rmse/
# https://towardsdatascience.com/forecast-kpi-rmse-mae-mape-bias-cdc5703d242d
metrics_decimal_precision = 2

# Mean Forecast Error (MFE, also known as forecast bias)
# The average of the real value minus the predicted value
# Measures how different the predicted value is from the real one
mean_forecast_error = [actual_outputs_unscaled[i]-predicted_outputs_unscaled[i] for i in range(len(actual_outputs_unscaled))]
mean_forecast_error = sum(mean_forecast_error) * 1.0/len(actual_outputs_unscaled)
mean_forecast_error = round(mean_forecast_error[0], metrics_decimal_precision)
print(f"Mean Forecast Error (MFE): {mean_forecast_error}")

# Mean Absolute Error (MAE)
# Average of the forecast error absolute values
mae = mean_absolute_error(actual_outputs_unscaled, predicted_outputs_unscaled)
mae = round(mae, metrics_decimal_precision)
print(f"Mean Absolute Error (MAE): {mae}")

# Mean Absolute Percentage Error (MAPE)
# Average of the forecast error absolute values
mape = mean_absolute_percentage_error(actual_outputs_unscaled, predicted_outputs_unscaled)
mape = round(mape, metrics_decimal_precision)
print(f"Mean Absolute Percentage Error (MAPE): {mape * 100}%")

# Mean Squared Error (MSE)
# Average of the squared forecast error values
# Difference between predicted and actual value, the lower the better
mse = mean_squared_error(actual_outputs_unscaled, predicted_outputs_unscaled)
mse = round(mse, metrics_decimal_precision)
print(f"Mean Squared Error (MSE): {mse}")

# Root Mean Squared Error (RMSE)
# The squar root of the MSE
# The square root of the difference between predicted and actual values, the lower the better
rmse = np.sqrt(mse)
rmse = round(rmse, metrics_decimal_precision)
print(f"Root Mean Squared Error (RMSE): {rmse}")


# Plot the predicted and actual value
if SHOW_ACTUAL_PREDICTED_GRAPH:
    # Convert the values to list to use in the graphs
    predicted_list = [value[0] for value in predicted_outputs_unscaled]
    actual_list = [value[0] for value in actual_outputs_unscaled]

    # Number of data entries that will be plotted
    indices = range(len(predicted_list))

    # Plot the predicted and actual outputs
    plt.figure(figsize=(10, 10)) # Create the graph window

    # Plot the data
    plt.plot(indices, predicted_list, label="Predicted", color="blue")
    plt.plot(indices, actual_list, label="Actual", color="orange")

    # Set information of the graph
    plt.xlabel(f"{FINANCIAL_DATA_TIME} Financial Record")
    plt.ylabel("Net Income ($ billions)")
    plt.title("Predicted vs Actual Net Income")

    # Show the graph
    plt.legend() # Show the labels of each line
    plt.grid(True) # Show background grid
    plt.show()



#-------------------------------------------------------------------------------------------------#
# Export as an ONNX model
#-------------------------------------------------------------------------------------------------#
if EXPORT_ONNX_FILE:
    print(f"------------------------Export------------------------")

    # Ensure neural network model is in evaluation mode
    netIncome_neural_net.eval()

    # Placeholder input for the model.
    # The values do not matter, but the shape (number of features, should match actual results)
    placeholder_input = evaluation_input_tensor[:1, :]

    # Set the path for the exported file
    script_directory = os.path.dirname(os.path.abspath(__file__))
    onnx_filename = "Data/Output/netIncome_neural_net.onnx"
    onnx_file_path = script_directory + "/" + onnx_filename

    # Export the neural network model as an ONNx
    torch.onnx.export(netIncome_neural_net, placeholder_input, onnx_file_path, verbose=True, opset_version=15)

    print(f"Neural network exported to {onnx_file_path}")

    # Visualize the ONNX file using netron
    netron.start(onnx_file_path)



#-------------------------------------------------------------------------------------------------#
# Export scaling values
#-------------------------------------------------------------------------------------------------#
# To be able to scale the values from and to C++, or the runtime component, export the data values
# needed to MinMax scale both the input and the output

if EXPORT_SCALING_DATA:
    # Get the Min and Max values for both the input and the output
    # Input
    input_mins = inputs_minMax_scaler.data_min_.tolist()
    input_maxs = inputs_minMax_scaler.data_max_.tolist()

    # Output
    output_min = output_minMax_scaler.data_min_[0]
    output_max = output_minMax_scaler.data_max_[0]


    # Get the columns name for all the data input that was scaled
    data_column_names = list(scaled_input.columns)

    # Go through each input, and create a JSON object, with both min and max,
    # for each column
    column_min_max = {}
    for i, name in enumerate(data_column_names):
        column_min_max[name] = {"min": input_mins[i], "max": input_maxs[i]}

    # Create the output JSON object wiht Min and Max
    column_min_max[output_column_name] = {"min": output_min, "max": output_max}


    # Save column names and min-max values to JSON
    json_file_name = "Data/Output/scaling_data_information.json"
    with open(json_file_name, "w") as json_file:
        json.dump(column_min_max, json_file, indent=4) # Indent used to format/beautify JSON output file

    # Print path where the file was exported
    json_full_path = script_directory + "/" + json_file_name
    print(f"Scaling Data exported to {json_full_path}")
    


#-------------------------------------------------------------------------------------------------#
# Check Neural Network Predictions
#-------------------------------------------------------------------------------------------------#
# Right now hardcoded to the last (latest) row of data, this can be useful to test against Unreal Engine
# if the values used are accurate
if PRINT_FINAL_TESTING_DATA_ROW_PREDICTION:
    # Get the data to test
    last_row_input = evaluation_input_tensor[-1].reshape(1, -1) 

    # Use Neural network to do the prediction
    with torch.no_grad():
        predicted_net_income = netIncome_neural_net(last_row_input)

    # Unscale the output
    unscaled_input = inputs_minMax_scaler.inverse_transform(last_row_input)
    predicted_net_income_unscaled = output_minMax_scaler.inverse_transform(predicted_net_income)

    # Print the predicted net income
    print("Input values (Unscaled):", unscaled_input)
    print("Predicted Net Income (Unscaled):", predicted_net_income_unscaled.item())