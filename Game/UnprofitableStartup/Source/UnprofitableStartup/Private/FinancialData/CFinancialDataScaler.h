// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMinMaxScaler.h"
#include "SFinancialData.h"
#include "CFinancialDataScaler.generated.h"


UCLASS(BlueprintType, meta = (
	ToolTip = "Class to handle the scaling and unscaling of the Financial Data"))
class UCFinancialDataScaler : public UObject
{
	GENERATED_BODY()

public:
	UCFinancialDataScaler();

	/**
	* Parses a JSOn file with the Minimum and Maximum value of the financial data, and setups
	* the scaler per these values
	* @param aScalingDataFilePath The path of the JSON file (relative to Content folder) that contains the MinMax scaling data
	* @return True if the JSON file was opened and parsed (does not check that all requested fields were present)
	*/
	UFUNCTION(BlueprintCallable, Category = "Financial Data Scaler")
	bool ParseMinMaxValues(const FString& aScalingDataFilePath);

	/**
	* Return the MinMax scaled (0 to 1 range) financial data
	* @param aUnscaledData The unscaled financial data
	* @param aNanosecondscDate The date from year 0, in nanoseconds
	* @return the financial data scaled in a 0 to 1 range, except for the date that remains unscaled
	*/
	UFUNCTION(BlueprintCallable, Category = "Financial Data Scaler")
	FSFinancialData GetMinMaxScaledData(FSFinancialData aUnscaledData, float& aNanosecondscDate) const;

	/**
	* Get the unscaled Net Income value
	* @param aScaledNetIncome The scaled (0 to 1 range) net income value
	*/
	UFUNCTION(BlueprintCallable, Category = "Financial Data Scaler")
	float UnscaleNetIncome(float aScaledNetIncome) const;


private:
	/**
	* Parse a JSON object to get the min and max value of a field
	* @param aFieldName The scaled (0 to 1 range) net income value
	* @param aRootJSONObject The root JSON object containing all the fields
	* @param aScaler The field scaler that will be set, per the Min and Max values parsed
	*/
	void ParseMinMaxValueOfField(const FString& aFieldName, 
		const TSharedPtr<FJsonObject> aRootJSONObject, UCMinMaxScaler& aScaler);


	// Financial terms definitions from SFinancialData can be seen in https://www.investopedia.com/

	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerFiscalDateEnding;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerOperatingCashFlow;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerCapitalExpenditure;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerGrossProfit;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerTotalRevenue;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerCostOfRevenue;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerOperatingExpenses;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerStockValue;
	UPROPERTY(EditAnywhere)
	UCMinMaxScaler* m_scalerNetIncome;
};
