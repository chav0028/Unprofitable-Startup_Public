#-------------------------------------------------------------------------------------------------#
# Using data saves as JSON, obtained from Alpha Vantage API https://www.alphavantage.co/
# Filter the financial statements for only the columns that will be used in the neural network
# Join the data into a single data frame, and creating an interpolated version of the original
# quarterly data, to monthly and weekly data
#-------------------------------------------------------------------------------------------------#
import json # Read JSON files
import pandas # Data Frames
# pandas suggested install of `pip install pyarrow`
from pandas import json_normalize # Used to flatten JSONs (remove additional level and access columns by name directly)
import matplotlib.pyplot as plt # Used to plot the comparison of the data (quarterly, monthly, weekly)



# Allow printing the whole columns (otherwise it will be truncated as ...)
pandas.set_option('display.max_rows', None)
pandas.set_option('display.max_columns', None)



# Defines to allow turning on-off some sections of code
INITIAL_DATA_STATS_PRINT = False # Print the initial data statistics
PREVIEW_FILTERED_DATA = False # Print preview of the filtered data                   


#-------------------------------------------------------------------------------------------------#
# Read Data From JSON Files
#-------------------------------------------------------------------------------------------------#
# Cash Flow data
cashflow_json_file_path = 'Data/Selected/NetflixQuarterlyCashFlow-AlphaVantage.json'
cashflow_dataframe = pandas.read_json(cashflow_json_file_path)


# Income Statement data
income_json_file_path = 'Data/Selected/NetflixQuarterlyIncomeStatement-AlphaVantage.json'
income_dataframe = pandas.read_json(income_json_file_path)


# Stock data
stock_json_file_path = 'Data/Selected/NetflixQuarterlyStock-AlphaVantage.json'
stock_dataframe = pandas.read_json(stock_json_file_path)



#-------------------------------------------------------------------------------------------------#
# Visualize data frames
#-------------------------------------------------------------------------------------------------#
if INITIAL_DATA_STATS_PRINT:
    print(cashflow_dataframe)
    print("\n")
    print(income_dataframe) 
    print("\n")
    print(stock_dataframe)
    print("\n")
    print("\n")



#-------------------------------------------------------------------------------------------------#
# Compare number of columns and rows
#-------------------------------------------------------------------------------------------------#
if INITIAL_DATA_STATS_PRINT:    
    num_cashflow_columns = cashflow_dataframe.shape[0]
    num_cashflow_rows = cashflow_dataframe.shape[1]
    print("Quarterly Cash Flow Data has columns %s and rows %s" % (num_cashflow_columns, num_cashflow_rows))

    num_income_columns = income_dataframe.shape[0]
    num_income_rows = income_dataframe.shape[1]
    print("Quarterly Income Data has columns %s and rows %s" % (num_income_columns, num_income_rows))

    num_stock_columns = stock_dataframe.shape[0]
    num_stock_rows = stock_dataframe.shape[1]
    print("Quarterly Stock Data has columns %s and rows %s" % (num_stock_columns, num_stock_rows))
    print("\n")



#-------------------------------------------------------------------------------------------------#
# Filter data
#-------------------------------------------------------------------------------------------------#
# Cash Flow
# Filter the data just for the columns we will use
filtered_cashflow_df = cashflow_dataframe
filtered_cashflow_df = json_normalize(filtered_cashflow_df['quarterlyReports'])
cashflow_selected_columns = ['fiscalDateEnding', 'operatingCashflow', 'capitalExpenditures'] # Name of columns to query     
filtered_cashflow_df = filtered_cashflow_df[cashflow_selected_columns]
if PREVIEW_FILTERED_DATA:
    print("Filtered Cashflow")
    print(filtered_cashflow_df)
    print("\n")

# Income
filtered_income_df = income_dataframe
filtered_income_df = json_normalize(filtered_income_df['quarterlyReports'])
income_selected_columns = ['grossProfit', 'totalRevenue', 'costOfRevenue', 'operatingExpenses', 'netIncome'] # Name of columns to query     
filtered_income_df = filtered_income_df[income_selected_columns]
if PREVIEW_FILTERED_DATA: 
    print("Filtered Income")
    print(filtered_income_df)
    print("\n")

# Stock
filtered_stock_df = stock_dataframe
filtered_stock_df = json_normalize(filtered_stock_df['Quarterly Time Series'])
stock_selected_columns = ['1. open'] # Name of columns to query     
filtered_stock_df = filtered_stock_df[stock_selected_columns]
filtered_stock_df.rename(columns={'1. open': 'StockValue'}, inplace=True) # Rename the column for easier identification
if PREVIEW_FILTERED_DATA: 
    print("Filtered Stock")
    print(filtered_stock_df)
    print("\n")



#-------------------------------------------------------------------------------------------------#
# Join data into a single data frame
#-------------------------------------------------------------------------------------------------#
# Convert rows to columns, and columns to rows. 
# This way the data series will be rows, and the properties columns
filtered_cashflow_df = filtered_cashflow_df.transpose()
filtered_income_df = filtered_income_df.transpose()
filtered_stock_df = filtered_stock_df.transpose()

# Join together all the dataframes
joint_financial_data = pandas.concat([filtered_cashflow_df, filtered_income_df, filtered_stock_df], axis=0)

# Transpose data again to properly have rows and columns
joint_financial_data = joint_financial_data.transpose()

# Round data to 2 decimals for readability
joint_financial_data = joint_financial_data.round(2) 


# Sort the data from oldest to newst
date_column_name = 'fiscalDateEnding'
joint_financial_data[date_column_name] = pandas.to_datetime(joint_financial_data[date_column_name])
joint_financial_data = joint_financial_data.sort_values(by=date_column_name, ascending=True)


# Visualize the end data result
print("Quarterly Data")
num_joint_columns = joint_financial_data.shape[0]
num_joint_rows = joint_financial_data.shape[1]
print("Quarterly data has columns %s and rows %s \n" % (num_joint_columns, num_joint_rows))
print(joint_financial_data.head(1))
print("\n")



#-------------------------------------------------------------------------------------------------#
# Interpolation function
#-------------------------------------------------------------------------------------------------#
# To have more data to train the neural network, interpolate the quarterly data
# into monthly or weekly data. So the values between quarters will be interpolated
# in the months/weeks between those quarters
def interpolate_financial_data(a_source_financial_data, a_date_column_name, a_target_frequency):
    # To be able to interpolate the data,
    # ensure all fields except the date are numerics
    # Separate the date from the other data being converted to numeric
    date_financial_data = a_source_financial_data[a_date_column_name]
    numeric_financial_data = a_source_financial_data.drop(columns=[a_date_column_name])

    # Convert data to numeric
    numeric_financial_data = numeric_financial_data.apply(pandas.to_numeric, errors='coerce')

    # Rejoin the date with the now numeric data
    interpolated_financial_data = pandas.concat([date_financial_data, numeric_financial_data], axis=1)
    interpolated_financial_data.columns = a_source_financial_data.columns # Preserve the column names

    # Set the date column as the index
    # so that it is used to determine the interpolation range
    interpolated_financial_data[a_date_column_name] = pandas.to_datetime(interpolated_financial_data[a_date_column_name])
    interpolated_financial_data.set_index(a_date_column_name, inplace=True)

    if a_target_frequency == 'monthly':
        # Interpolate the quarterly data to months
        interpolated_financial_data = interpolated_financial_data.resample('ME').interpolate()
    elif a_target_frequency == 'weekly':
        # Just interpolating the data to weekly directly does not produce
        # good results (though it can be improved when source of interpolation is monthly instead
        #  of quarterly)
        # So to get more accurate data first interpolate the data to daily, then to weekly
        daily_data = interpolated_financial_data.resample('D').interpolate()
        interpolated_financial_data = daily_data.resample('W').interpolate()
    else:
        raise ValueError("Target frequency should be either 'monthly' or 'weekly'")
    
    # Round data to 2 decimals for readability
    interpolated_financial_data = interpolated_financial_data.round(2) 

    # Reset the index to make 'fiscalDateEnding' a column again
    interpolated_financial_data.reset_index(inplace=True)

    # Ensure the data is sorted from oldest to newest
    interpolated_financial_data[date_column_name] = pandas.to_datetime(interpolated_financial_data[date_column_name])
    interpolated_financial_data = interpolated_financial_data.sort_values(by=date_column_name, ascending=True)

    return interpolated_financial_data



#-------------------------------------------------------------------------------------------------#
# Interpolated quarterly data to monthly
#-------------------------------------------------------------------------------------------------#

# Interpolate the quarterly data to months
monthly_financial_data = interpolate_financial_data(joint_financial_data, date_column_name, 'monthly')

# Visualize the end data result
print("Monthly Data")
num_monthly_columns = monthly_financial_data.shape[0]
num_monthly_rows = monthly_financial_data.shape[1]
print("Monthly data has columns %s and rows %s" % (num_monthly_columns, num_monthly_rows))
print(monthly_financial_data.head(1))
print("\n")



#-------------------------------------------------------------------------------------------------#
# Interpolated quarterly data to weekly
#-------------------------------------------------------------------------------------------------#

# Interpolate the quarterly data to weeks
weekly_financial_data = interpolate_financial_data(joint_financial_data, date_column_name, 'weekly')

# Visualize the end data result
print("Weekly Data")
num_weekly_columns = weekly_financial_data.shape[0]
num_weekly_rows = weekly_financial_data.shape[1]
print("Weekly data has columns %s and rows %s" % (num_weekly_columns, num_weekly_rows))
print(weekly_financial_data.head(1))
print("\n")



#-------------------------------------------------------------------------------------------------#
# Evaluate the data
#-------------------------------------------------------------------------------------------------#
# The quarterly data (original data) current store values as objects
# so convert them to numeric values to be able to compare them against other data
variable_to_evaluate = 'StockValue'
quaterly_data = joint_financial_data
quaterly_data[variable_to_evaluate] = pandas.to_numeric(quaterly_data[variable_to_evaluate], errors='coerce')


# Check the value ranges of the data
print(f"Value Ranges of {variable_to_evaluate}" )
print("Quaterly Data:", joint_financial_data[variable_to_evaluate].min(), "-", joint_financial_data[variable_to_evaluate].max())
print("Monthly Data:", monthly_financial_data[variable_to_evaluate].min(), "-", monthly_financial_data[variable_to_evaluate].max())
print("Weekly Data:", weekly_financial_data[variable_to_evaluate].min(), "-", weekly_financial_data[variable_to_evaluate].max())


# Plot the data
plt.plot(quaterly_data[date_column_name], quaterly_data[variable_to_evaluate], label='Quarterly Data', marker='o')
plt.plot(monthly_financial_data[date_column_name], monthly_financial_data[variable_to_evaluate], label='Interpolated Monthly Data')
plt.plot(weekly_financial_data[date_column_name], weekly_financial_data[variable_to_evaluate], label='Interpolated Weekly Data')

# Add legend and labels
plt.xlabel(date_column_name)
plt.ylabel(variable_to_evaluate)
plt.title('Quarterly vs Monthly vs Weekly')
plt.legend()

# Show plot
plt.show()



#-------------------------------------------------------------------------------------------------#
# Save joint data to CSV file
#-------------------------------------------------------------------------------------------------#
# Save the quarterly data                             
joint_financial_data.to_csv('Data/Output/quarterlyFinancialData.csv', index=False)

# Save the interpolated monthly data
monthly_financial_data.to_csv('Data/Output/monthlyFinancialData.csv', index=False)

# Save the interpolated weekly data
weekly_financial_data.to_csv('Data/Output/weeklyFinancialData.csv', index=False)