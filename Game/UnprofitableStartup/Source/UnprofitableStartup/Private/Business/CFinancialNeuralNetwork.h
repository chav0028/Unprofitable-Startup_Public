// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "NeuralNetwork.h"
#include "FinancialData/SFinancialData.h"
#include "CFinancialNeuralNetwork.generated.h"

class UCFinancialDataScaler;

UCLASS(BlueprintType,
	meta = (ToolTip = "Neural Network wrapper to make inferences on the Net Income of a company"))
class UCFinancialNeuralNetwork : public UNeuralNetwork
{
	GENERATED_BODY()
	
public:
	UCFinancialNeuralNetwork();

	/**
	* Sets the Neural Network Model to be used
	* @param aNeuralNetworkModel The Neural Network Model (obtained from ONNX file) to be set
	* @param aNeuralNetworkModel The path of the scaling data for the neural network model (path relative to Content folder)
	*/
	UFUNCTION(BlueprintCallable, Category = "Financial Neural Network")
	void SetModel(UNeuralNetwork* aNeuralNetworkModel, const FString& aScalingDataFilePath);

	/**
	* Run the Neural Network model, and from the parameters infer the net income
	* @param aFinancialInput The unscaled financial data that will be inputted to the Neural Network
	* @return the unscaled net income inferred by the Neural Network
	*/
	UFUNCTION(BlueprintCallable, Category = "Financial Neural Network")
	float RunModel(FSFinancialData aFinancialInput);

private:
	
	// UPROPERTY helps ensure that this pointer is not automatically garbage collected
	// https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Objects/Optimizations/
	UPROPERTY(VisibleInstanceOnly)
	UCFinancialDataScaler* m_dataScaler;

	// Transient properties are always initialized to zero and are not serialized to disk
	UPROPERTY(Transient)
	UNeuralNetwork* m_simulationNN;
};
