// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "FinancialData/CFinancialDataScaler.h"

#include "JsonUtilities.h" // Used to deserialize JSON

static const FString& M_MIN_JSON = "min";
static const FString& M_MAX_JSON = "max";


UCFinancialDataScaler::UCFinancialDataScaler() :
	m_scalerFiscalDateEnding(NewObject<UCMinMaxScaler>()),
	m_scalerOperatingCashFlow(NewObject<UCMinMaxScaler>()),
	m_scalerCapitalExpenditure(NewObject<UCMinMaxScaler>()),
	m_scalerGrossProfit(NewObject<UCMinMaxScaler>()),
	m_scalerTotalRevenue(NewObject<UCMinMaxScaler>()),
	m_scalerCostOfRevenue(NewObject<UCMinMaxScaler>()),
	m_scalerOperatingExpenses(NewObject<UCMinMaxScaler>()),
	m_scalerStockValue(NewObject<UCMinMaxScaler>()),
	m_scalerNetIncome(NewObject<UCMinMaxScaler>())
{
}

bool UCFinancialDataScaler::ParseMinMaxValues(const FString& aScalingDataFilePath)
{
	if (aScalingDataFilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No JSON file for scaling data has been specified"));
		return false;
	}

	// Make the path relative to the project content directory
	const FString projectFilePath = FPaths::ProjectContentDir() + aScalingDataFilePath;

	// Load the JSON file as a string
	FString stringJSON;
	if (FFileHelper::LoadFileToString(stringJSON, *projectFilePath))
	{
		TSharedPtr<FJsonObject> JSONObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JSONReader = TJsonReaderFactory<>::Create(stringJSON);

		// Deserialize the JSON into a JSON object
		if (FJsonSerializer::Deserialize(JSONReader, JSONObject) && JSONObject.IsValid())
		{
			// Setup the minimum and maximum value of all the fields' MinMax Scalers
			ParseMinMaxValueOfField("fiscalDateEnding",		JSONObject, *m_scalerFiscalDateEnding);
			ParseMinMaxValueOfField("operatingCashflow",	JSONObject, *m_scalerOperatingCashFlow);
			ParseMinMaxValueOfField("capitalExpenditures",	JSONObject, *m_scalerCapitalExpenditure);
			ParseMinMaxValueOfField("grossProfit",			JSONObject, *m_scalerGrossProfit);
			ParseMinMaxValueOfField("totalRevenue",			JSONObject, *m_scalerTotalRevenue);
			ParseMinMaxValueOfField("costOfRevenue",		JSONObject, *m_scalerCostOfRevenue);
			ParseMinMaxValueOfField("operatingExpenses",	JSONObject, *m_scalerOperatingExpenses);
			ParseMinMaxValueOfField("netIncome",			JSONObject, *m_scalerNetIncome);
			ParseMinMaxValueOfField("StockValue",			JSONObject, *m_scalerStockValue);
			
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Error deserializing JSON file %s"), *projectFilePath);
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error reading JSON file %s"), *projectFilePath);
		return false;
	}

	return false;
}

FSFinancialData UCFinancialDataScaler::GetMinMaxScaledData(FSFinancialData aUnscaledData, float& aNanosecondscDate) const
{
	// Convert the data to nanoseconds, since Neural Network cannot take pure dates
	float unscaledNanoSeconds = static_cast<float>(aUnscaledData.m_date.ToUnixTimestamp());

	FSFinancialData scaledData = aUnscaledData;

	if (IsValid(m_scalerFiscalDateEnding) == false ||
		IsValid(m_scalerOperatingCashFlow) == false ||
		IsValid(m_scalerCapitalExpenditure) == false ||
		IsValid(m_scalerGrossProfit) == false ||
		IsValid(m_scalerTotalRevenue) == false ||
		IsValid(m_scalerCostOfRevenue) == false ||
		IsValid(m_scalerOperatingExpenses) == false ||
		IsValid(m_scalerStockValue) == false ||
		IsValid(m_scalerNetIncome) == false
		)
	{
		UE_LOG(LogTemp, Error, TEXT("UCFinancialDataScaler has not been initialised, exiting function"));
		aNanosecondscDate = unscaledNanoSeconds;
		return scaledData;
	}

	// Convert the data to nanoseconds, the format expected by the Neural Network
	// scale it by MinMax

	// Scale all the data using the previously saved min and max values
	aNanosecondscDate = m_scalerFiscalDateEnding->ScaleValue(unscaledNanoSeconds);
	scaledData.m_operatingCashFlow = m_scalerOperatingCashFlow->ScaleValue(aUnscaledData.m_operatingCashFlow);
	scaledData.m_capitalExpenditure = m_scalerCapitalExpenditure->ScaleValue(aUnscaledData.m_capitalExpenditure);
	scaledData.m_grossProfit = m_scalerGrossProfit->ScaleValue(aUnscaledData.m_grossProfit);
	scaledData.m_totalRevenue = m_scalerTotalRevenue->ScaleValue(aUnscaledData.m_totalRevenue);
	scaledData.m_costOfRevenue = m_scalerCostOfRevenue->ScaleValue(aUnscaledData.m_costOfRevenue);
	scaledData.m_operatingExpenses = m_scalerOperatingExpenses->ScaleValue(aUnscaledData.m_operatingExpenses);
	scaledData.m_stockValue = m_scalerStockValue->ScaleValue(aUnscaledData.m_stockValue);
	scaledData.m_netIncome = m_scalerNetIncome->ScaleValue(aUnscaledData.m_netIncome);

	return scaledData;
}

float UCFinancialDataScaler::UnscaleNetIncome(float aScaledNetIncome) const
{
	if (IsValid(m_scalerNetIncome) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UCFinancialDataScaler has not been initialised, exiting function"));
		return aScaledNetIncome;
	}

	return m_scalerNetIncome->UnscaleValue(aScaledNetIncome);
}

void UCFinancialDataScaler::ParseMinMaxValueOfField(const FString& aFieldName,
	const TSharedPtr<FJsonObject> aRootJSONObject, UCMinMaxScaler& aScaler)
{
	const TSharedPtr<FJsonObject> fieldJSONObject = aRootJSONObject->GetObjectField(aFieldName);
	if (aRootJSONObject.IsValid())
	{
		// Parse the minimum and maximum value
		float minValue = fieldJSONObject->GetNumberField(M_MIN_JSON);
		float maxValue = fieldJSONObject->GetNumberField(M_MAX_JSON);

		// Set the scaler minimum and maximum value
		aScaler.SetMinMaxValues(minValue, maxValue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Scaling data does not have JSON field for %s"), *aFieldName);
	}
}
