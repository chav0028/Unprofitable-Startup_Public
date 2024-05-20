// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Business/CFinancialNeuralNetwork.h"
#include "FinancialData/CFinancialDataScaler.h"

UCFinancialNeuralNetwork::UCFinancialNeuralNetwork() :
	m_dataScaler(NewObject<UCFinancialDataScaler>()),
	m_simulationNN(nullptr)
{
}

void UCFinancialNeuralNetwork::SetModel(UNeuralNetwork* aNeuralNetworkModel, const FString& aScalingDataFilePath)
{
	m_simulationNN = aNeuralNetworkModel;

	if (m_simulationNN != nullptr)
	{
		// Specify the neural network to run on the GPU
		m_simulationNN->SetDeviceType(ENeuralDeviceType::GPU);
	}

	// Setup the data scaler to the new Neural Network
	m_dataScaler->ParseMinMaxValues(aScalingDataFilePath);
}

float UCFinancialNeuralNetwork::RunModel(FSFinancialData aFinancialInput)
{
	// Check Neural Network has been set
	if (m_simulationNN == nullptr || m_simulationNN->IsLoaded() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot run Neural Network Model, it has not been set to valid value"));
		return 0.0f;
	}

	// Scale the inputs to a range accepted by the Neural Network
	float timeNanoSeconds = 0.0f;
	FSFinancialData scaledData = m_dataScaler->GetMinMaxScaledData(aFinancialInput, timeNanoSeconds);

	TArray<float> inputNN;

	// Add the variables to the array, in the order in which they were set 
	// in the Neural Network model
	inputNN.Emplace(timeNanoSeconds);
	inputNN.Emplace(scaledData.m_operatingCashFlow);
	inputNN.Emplace(scaledData.m_capitalExpenditure);
	inputNN.Emplace(scaledData.m_grossProfit);
	inputNN.Emplace(scaledData.m_totalRevenue);
	inputNN.Emplace(scaledData.m_costOfRevenue);
	inputNN.Emplace(scaledData.m_operatingExpenses);
	inputNN.Emplace(scaledData.m_stockValue);

	// Set the Neural Network and run it
	m_simulationNN->SetInputFromArrayCopy(inputNN);
	m_simulationNN->Run();

	// Get the output from the Neural Network
	float scaledOutput = 0.0f;
	TArray<float> outputNN = m_simulationNN->GetOutputTensor().GetArrayCopy<float>();

	// If it produced an output
	if (outputNN.IsEmpty() == false)
	{
		// Unscale it
		scaledOutput = m_dataScaler->UnscaleNetIncome(outputNN[0]);

		// The NN should only produce one output, check this is the case
		if (outputNN.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("The Neural Network Model is outputting more than 1 value"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Neural Network Model did not produce any output"));
	}

	return scaledOutput;
}